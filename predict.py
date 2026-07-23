#!/usr/bin/env python

# This is the Python 3.12 version of this file.  
# (Tested in Python 3.12.7 with Anaconda, but should work in other Python versions, and certainly more recent versions.)

# Modules from the Python standard library.
from datetime import datetime, timezone, timedelta
import time as timelib
import math
import sys
import os
import socket
import logging
import traceback
import calendar
import optparse
import ctypes
import signal
import subprocess
import statsd
import tempfile
import shutil
import bisect
import simplejson as json
import re
import httplib2

import boto3
from botocore import UNSIGNED
from botocore.config import Config
import xarray as xr
import pandas as pd

BUCKET = "noaa-gfs-bdp-pds"
VARS = ("HGT", "UGRD", "VGRD", "TMP", "VVEL")
    
_s3 = boto3.client("s3", region_name="us-east-1",
                   config=Config(signature_version=UNSIGNED))

# handle both predict.py's
filepath = os.path.dirname(os.path.abspath(__file__))
if filepath.endswith('predict'):
    from py_variables import *
else:
    sys.path.append(os.path.join(filepath, 'predict'))
    from py_variables import *

# determine OS: darwin = Mac, other forms of win = Windows
OS_IS_WINDOWS = False
if 'win' in sys.platform.lower():
    OS_IS_WINDOWS = True

if 'darwin' in sys.platform.lower():
    OS_IS_WINDOWS = False
    
pred_binary = ''

statsd.init_statsd({'STATSD_BUCKET_PREFIX': 'habhub.predictor'})

# We use Pydap from http://pydap.org/.
import pydap.client, pydap.exceptions
# from pydap.exceptions import ClientError
# Unfortunately the NOAA GFS file servers appear to prohibit the use of a cache, so the below 2 lines are commented out
# import pydap.lib                      					#cache
# pydap.lib.CACHE = "/tmp/pydap-cache/"    					#cache

# horrid, horrid monkey patching to force
# otherwise broken caching from dods server
# this is really, really hacky
# 
# from pydap.util import http as pyhttp						#cache
# def fresh(response_headers, request_headers):					#cache
#     cc = pyhttp.httplib2._parse_cache_control(response_headers)		#cache
#     if cc.has_key('no-cache'):						#cache
#         return 'STALE'							#cache
#     return 'FRESH'								#cache
# pydap.util.http.httplib2._entry_disposition = fresh

def fresh(response_headers, request_headers):
    cc = httplib2._parse_cache_control(response_headers)
    if cc.has_key('no-cache'):
        return 'STALE'
    return 'FRESH'
httplib2._entry_disposition = fresh


def print_int_linenum(signum, frame):
    log.info('Process interrupted by SIGINT.  Currently at line:')
    log.info(frame.f_lineno)

def print_term_linenum(signum, frame):
    log.info('Process received signal SIGTERM.  Currently at line:')
    log.info(frame.f_lineno)

def print_quit_linenum(signum, frame):
    log.info('Process received signal SIGQUIT.  Currently at line:')
    log.info(frame.f_lineno)

def print_alrm_linenum(signum, frame):
    log.info('Process received signal SIGALRM.  Currently at line:')
    log.info(frame.f_lineno)

def print_abrt_linenum(signum, frame):
    log.info('Process received signal SIGABRT.  Currently at line:')
    log.info(frame.f_lineno)

signal.signal(signal.SIGINT, print_int_linenum)
signal.signal(signal.SIGTERM, print_term_linenum)
if not OS_IS_WINDOWS:
    signal.signal(signal.SIGQUIT, print_quit_linenum)
    signal.signal(signal.SIGALRM, print_alrm_linenum)
    signal.signal(signal.SIGABRT, print_abrt_linenum)

# Output logger format
log = logging.getLogger('main')
log_formatter = logging.Formatter('%(levelname)s: %(message)s')
console = logging.StreamHandler()
console.setFormatter(log_formatter)
log.addHandler(console)
exit_code = 0
gfs_dir = b''

progress_f = ''
progress = {
    'run_time': '',
    'gfs_percent': 0,
    'gfs_timeremaining': '',
    'gfs_complete': False,
    'gfs_timestamp': '',
    'pred_running': False,
    'pred_complete': False,
    'warnings': False,
    'pred_output': [],
    'error': '',
    }

def update_progress(**kwargs):
    global progress_f
    global progress
    for arg in kwargs:
        progress[arg] = kwargs[arg]
    try:
        progress_f.truncate(0)
        progress_f.seek(0)
        progress_f.write(json.dumps(progress))
        progress_f.flush()
        os.fsync(progress_f.fileno())
    except IOError:
        global log
        log.error('Could not update progress file')

@statsd.StatsdTimer.wrap('time')
def main():
    """
    The main program routine.
    """

    print('starting predict.py Python code ...')

    statsd.increment('run')

    # Set up our command line options
    parser = optparse.OptionParser()
    parser.add_option('-d', '--cd', dest='directory',
        help='change to, and run in, directory DIR',
        metavar='DIR')
    parser.add_option('--fork', dest='fork', action="store_true",
            help='detach the process and run in the background')
    parser.add_option('--alarm', dest='alarm', action="store_true",
            help='setup an alarm for 10 minutes time to prevent hung processes')
    parser.add_option('--redirect', dest='redirect', default='/dev/null',
            help='if forking, file to send stdout/stderr to', metavar='FILE')
    parser.add_option('-t', '--timestamp', dest='timestamp',
        help='search for dataset covering the POSIX timestamp TIME \t[default: now]', 
        metavar='TIME', type='int',
        # default=calendar.timegm(datetime.datetime.now(datetime.UTC).timetuple()))
        default=calendar.timegm(datetime.now(timezone.utc).timetuple()))
    parser.add_option('-v', '--verbose', action='count', dest='verbose',
        help='be verbose. The more times this is specified the more verbose.', default=False)
    parser.add_option('-p', '--past', dest='past',
        help='window of time to save data is at most HOURS hours in past [default: %default]',
        metavar='HOURS',
        type='int', default=3)
    parser.add_option('-f', '--future', dest='future',
        help='window of time to save data is at most HOURS hours in future [default: %default]',
        metavar='HOURS',
        type='int', default=9)
    parser.add_option('--hd', dest='hd', action="store_true",
            help='use higher definition GFS data (default: no)')
    parser.add_option('--fastsim', dest='fastsim', action="store_true",
            help='use fast-generated fake wind fields, instead of real NOAA GFS data (default: no)')
    parser.add_option('--preds', dest='preds_path',
            help='path that contains uuid folders for predictions [default: %default]',
            default='./predict/preds/', metavar='PATH')
#    parser.add_option('--preds', dest='preds_path',
#            help='path that contains uuid folders for predictions [default: %default]',
#            default='./preds/', metavar='PATH')

    group = optparse.OptionGroup(parser, "Location specifiers",
        "Use these options to specify a particular tile of data to download.")
    group.add_option('--lat', dest='lat',
        help='tile centre latitude in range (-90,90) degrees north [default: %default]',
        metavar='DEGREES',
        type='float', default=52)
    group.add_option('--lon', dest='lon',
        help='tile centre longitude in degrees east [default: %default]',
        metavar='DEGREES',
        type='float', default=0)
    group.add_option('--latdelta', dest='latdelta',
        help='tile radius in latitude in degrees [default: %default]',
        metavar='DEGREES',
        type='float', default=5)
    group.add_option('--londelta', dest='londelta',
        help='tile radius in longitude in degrees [default: %default]',
        metavar='DEGREES',
        type='float', default=5)
    parser.add_option_group(group)

    #group = optparse.OptionGroup(parser, "Tile specifiers",
        #"Use these options to specify how many tiles to download.")
    #group.add_option('--lattiles', dest='lattiles',
        #metavar='TILES',
        #help='number of tiles along latitude to download [default: %default]',
        #type='int', default=1)
    #group.add_option('--lontiles', dest='lontiles',
        #metavar='TILES',
        #help='number of tiles along longitude to download [default: %default]',
        #type='int', default=1)
    #parser.add_option_group(group)

    (options, args) = parser.parse_args()

    # Check we got a UUID in the arguments
    if len(args) != 1:
        log.error('Exactly one positional argument should be supplied (uuid).')
        statsd.increment('error')
        sys.exit(1)

    if options.directory:
        os.chdir(options.directory)

    if options.fork:
        detach_process(options.redirect)

    if options.alarm:
        setup_alarm()

    # Path to predictor binary
    global pred_binary
    if OS_IS_WINDOWS:
        # Windows
        if options.fastsim:
            pred_binary = './pred_src/pred_fastsim.exe'
        else:
            pred_binary = './pred_src/pred_StationKeep.exe'
    else:
        # probably Linux or Mac
        if options.fastsim:
            pred_binary = './pred_src/pred_fastsim'
        else:
            pred_binary = './pred_src/pred_StationKeep'

    uuid = args[0]
    uuid_path = options.preds_path + "/" + uuid + "/"

    # Check we're not already running with this UUID
    for line in os.popen('ps xa'):
        process = " ".join(line.split()[4:])
        if process.find(uuid) > 0:
            pid = int(line.split()[0])
            if pid != os.getpid():
                statsd.increment('duplicate')
                log.error('A process is already running for this UUID, quitting.')
                sys.exit(1)

    # Make the UUID directory if non existant
    if not os.path.exists(uuid_path):
        os.mkdir(uuid_path, 0o770)

    # Open the progress.json file for writing, creating it and closing again to flush
    global progress_f
    global progress
    try:
        progress_f = open(uuid_path+"progress.json", "w+")
        update_progress(
            gfs_percent=0,
            gfs_timeremaining="Please wait...",
            run_time=str(int(timelib.time())))
    except IOError:
        log.error('Error opening progress.json file')
        statsd.increment('error')
        sys.exit(1)
    
    # Check the predictor binary exists
    if not os.path.exists(pred_binary):
        log.error('Predictor binary does not exist.')
        statsd.increment('error')
        sys.exit(1)

    # Check the latitude is in the right range.
    if (options.lat < -90) | (options.lat > 90):
        log.error('Latitude %s is outside of the range (-90,90).')
        statsd.increment('error')
        sys.exit(1)

    # Check the delta sizes are valid.
    if (options.latdelta <= 0.5) | (options.londelta <= 0.5):
        log.error('Latitiude and longitude deltas must be at least 0.5 degrees.')
        statsd.increment('error')
        sys.exit(1)

    if options.londelta > 180:
        log.error('Longitude window sizes greater than 180 degrees are meaningless.')
        statsd.increment('error')
        sys.exit(1)

    # We need to wrap the longitude into the right range.
    options.lon = canonicalise_longitude(options.lon)

    # How verbose are we being?
    if options.verbose > 0:
        log.setLevel(logging.INFO)
    if options.verbose > 1:
        log.setLevel(logging.DEBUG)
    if options.verbose > 2:
        logging.basicConfig(level=logging.INFO)
    if options.verbose > 3:
        logging.basicConfig(level=logging.DEBUG)

    pred_in_progress = False
    pred_process = None
    pred_output = []
    if options.fastsim:
        do_fastsim(options.lat, options.lon, options.timestamp, options, pred_in_progress, pred_process, pred_output, uuid_path)
    else:
        download_and_process_wind_data(options.lat, options.lon, options.timestamp, options, pred_in_progress, pred_process, pred_output, uuid_path)

    if exit_code == 1:
        # Hard error from the predictor. Tell the javascript it completed, so that it will show the trace,
        # but pop up a 'warnings' window with the error messages
        log.info('predict.py exit code is 1.  Exiting...')
        update_progress(pred_running=False, pred_complete=True, warnings=True, pred_output=pred_output)
        statsd.increment('success_serious_warnings')
    elif pred_output:
        # Soft error (altitude too low error, typically): pred_output being set forces the debug
        # window open with the messages in
        update_progress(pred_running=False, pred_complete=True, pred_output=pred_output)
        statsd.increment('success_minor_warnings')
    else:
        log.info('The predictor pred.exe executable exit_code = %s' % exit_code )
        assert exit_code == 0
        update_progress(pred_running=False, pred_complete=True)
        statsd.increment('success')  
 
    copy_flight_path(uuid_path)

    if not options.fastsim:
        shutil.rmtree(gfs_dir)

def _isobaric_mb(level_str):
    parts = level_str.split()
    if len(parts) != 2 or parts[1] != "mb":
        return False
    try:
        float(parts[0])
        return True
    except ValueError:  
        return False
       

def _byte_ranges_from_idx(idx_text):
    """
    Parse a GFS .idx file. Return list of (start, end) byte ranges for the
    5 variables
    """
    lines = [l for l in idx_text.splitlines() if l.strip()]
    # Parse each line into (start_byte, var, level)
    parsed = []
    starts = []
    for l in lines:
        f = l.split(":")
        #f[0]=msgnum f[1]=startbyte f[2]=d=date f[3]=VAR f[4]=LEVEL f[5]=fcst
        start = int(f[1])
        starts.append(start)
        parsed.append((start, f[3], f[4]))
    ranges = []
    for i, (start, var, level) in enumerate(parsed):
        if var in VARS and _isobaric_mb(level):
            end = starts[i + 1] - 1 if i + 1 < len(starts) else None
            ranges.append((start, end))
    return ranges

def _fetch_grib_subset(key, out_path):
    """Download the GRIB messages for `key` into out_path."""
    idx = _s3.get_object(Bucket=BUCKET, Key=key + ".idx")["Body"].read().decode()
    ranges = _byte_ranges_from_idx(idx)
    with open(out_path, "wb") as fh:
        for start, end in ranges:
            rng = f"bytes={start}-{end}" if end is not None else f"bytes={start}-"
            body = _s3.get_object(Bucket=BUCKET, Key=key, Range=rng)["Body"].read()
            fh.write(body)
        
                        
def _grib_key(run, fxx):
    day = run.strftime("%Y%m%d")
    hh = run.strftime("%H")
    return f"gfs.{day}/{hh}/atmos/gfs.t{hh}z.pgrb2.1p00.f{fxx:03d}"


def _key_exists(key):
    r = _s3.list_objects_v2(Bucket=BUCKET, Prefix=key, MaxKeys=1)
    return any(o["Key"] == key for o in r.get("Contents", []))
        

def open_forecast_hour(run, fxx, tmpdir="/tmp/gfs_s3"):
    """
    Fetch one forecast hour as an xarray.Dataset
    variables gh,u,v,t,w
    Returns None if the file isn't available yet.
    """
    key = _grib_key(run, fxx)
    if not _key_exists(key):
        return None
    os.makedirs(tmpdir, exist_ok=True)
    out_path = os.path.join(tmpdir, f"subset_{run:%Y%m%d%H}_f{fxx:03d}.grib2")
    _fetch_grib_subset(key, out_path)
            
    dss = xr.open_dataset(
        out_path, engine="cfgrib",
        backend_kwargs={"filter_by_keys": {"typeOfLevel": "isobaricInhPa"},
                        "indexpath": ""},
    )
    ds = dss.sortby("isobaricInhPa", ascending=False)
    return ds



def do_fastsim(thelat, thelon, thetime, options, pred_in_progress, pred_process, pred_output, uuid_path):
    global exit_code

    command = [pred_binary, '-vv', '-o', uuid_path+'flight_path.csv', uuid_path+'scenario.ini']
    log.info('The command is:')
    log.info(command)
    # pred_process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    pred_process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    log.info('Command has been sent')
    # sys.stdout.flush()
    # pred_process.stdout.flush()

    while True:
        if exit_code == 1:
            return

        line = pred_process.stdout.readline()
        if not line:
            break
        if line == b'':
            break
    
        # pass through
        # sys.stdout.write(line)
        # the required Python 3 obfuscation of the above line ...
        sys.stdout.write(line.decode(sys.stdout.encoding))
        sys.stdout.flush()

    copy_flight_path(uuid_path)

    if exit_code != 1:
        exit_code = pred_process.wait()


def download_and_process_wind_data(thelat, thelon, thetime, options, pred_in_progress, pred_process, pred_output, uuid_path):
    # log.debug('Using cache directory: %s' % pydap.lib.CACHE)				#cache
    global exit_code
    global gfs_dir

    # time_to_find = datetime.datetime.utcfromtimestamp(thetime)
    time_to_find = datetime.fromtimestamp(thetime, tz=timezone.utc)
    # utcoffset = datetime.timedelta(hours = 7.0)
    # time_to_find -= utcoffset

    log.info('Looking for latest dataset which covers %s' % time_to_find.ctime())
    try:
        dataset = dataset_for_time(time_to_find, options.hd)
    except:
        log.info('Could not locate a dataset for the requested time.')
        statsd.increment('no_dataset')
        statsd.increment('error')
        exit_code = 1
        return
#        sys.exit(1)

#    dataset_times = map(timestamp_to_datetime, dataset.time)
#    dataset_timestamps = map(datetime_to_posix, dataset_times)
#    dataset_times = list(map(timestamp_to_datetime, dataset.time))
#    dataset_timestamps = list(map(datetime_to_posix, dataset_times))
    dataset_times = [t for (t, ds) in dataset]
    dataset_timestamps = [int(pd.Timestamp(t).timestamp()) for t in dataset_times]


    log.info('Found appropriate dataset:')
    log.info('    Start time: %s (POSIX %s)' % \
        (dataset_times[0].ctime(), dataset_timestamps[0]))
    log.info('      End time: %s (POSIX %s)' % \
        (dataset_times[-1].ctime(), dataset_timestamps[-1]))

    # print('dataset.lat = ', dataset.lat)
    # print('dataset[lat][:] = ', dataset['lat'][:])
    # print('list(dataset.lat) = ', list(dataset.lat))
    # print('dataset[lat][:].data = ', dataset['lat'][:].data)

    # log.info('      Latitude: %s -> %s' % (min(dataset.lat), max(dataset.lat)))
    # log.info('     Longitude: %s -> %s' % (min(dataset.lon), max(dataset.lon)))
    # log.info('      Latitude: %s -> %s' % (min(list(dataset.lat)), max(list(dataset.lat))))
    # log.info('     Longitude: %s -> %s' % (min(list(dataset.lon)), max(list(dataset.lon))))

#    for dlat in range(0,options.lattiles):
#        for dlon in range(0,options.lontiles):
    window = ( \
            thelat, options.latdelta, \
            thelon, options.londelta)

#    gfs_dir = "/var/www/cusf-standalone-predictor/gfs/"
    if gfs_dir == b'':
        gfs_dir = os.path.join(ROOT_DIR, "gfs")
        gfs_dir = tempfile.mkdtemp(dir=gfs_dir)

    gfs_filename = "gfs_%(time)_%(lat)_%(lon)_%(latdelta)_%(londelta).dat"
    output_format = os.path.join(gfs_dir, gfs_filename)

    write_file_return_code = write_file(output_format, dataset, \
            window, \
            (time_to_find - timedelta(hours=options.past)).replace(tzinfo=None), \
            (time_to_find + timedelta(hours=options.future)).replace(tzinfo=None))
            # time_to_find - datetime.timedelta(hours=options.past), \
            # time_to_find + datetime.timedelta(hours=options.future))
    if write_file_return_code == 1:
        log.debug('write_file_return_code is 1')
        exit_code = 1
        return

    # no cache to purge, since the NOAA GFS wind file server prohibits use of a cache
    # purge_cache()							#cache
    log.info('Done downloading this set of wind data files')
    if pred_in_progress:
        haventSaidWeveSentItYet = True
        while True:
            pred_process.stdin.write(b'D\n')    # tell the pred subprocess that the download is done, by sending a big D to the stdin of the pred subprocess
            pred_process.stdin.flush()
            if haventSaidWeveSentItYet:
                log.info('Sent a big D to the stdin of the pred subprocess')
                haventSaidWeveSentItYet = False
            breakOutOfMainLoop = False
            # pred_process.stdout.seek(0)            # ensure we start at the beginning -- actually, can't do this with stdout, so this is commented out
            while True:
                line = pred_process.stdout.readline()
                if not line or line == b'':
                    break
                sys.stdout.write(line.decode(sys.stdout.encoding))
                if b'INFO: The pred C code got' in line:
                    log.info('Message received')
                    breakOutOfMainLoop = True
                    break
            if breakOutOfMainLoop:
                break

    
    update_progress(gfs_percent=100, gfs_timeremaining='Done', gfs_complete=True, pred_running=True)

    if options.alarm:
        alarm_flags = ["-a120"]
    else:
        alarm_flags = []

    if not pred_in_progress:
        command = [pred_binary, '-i', gfs_dir, '-vv', '-o', uuid_path+'flight_path.csv', uuid_path+'scenario.ini']
        log.info('The command is:')
        log.info(command)
        pred_process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        # pred_process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        pred_in_progress = True

    # pred_process.stdout.seek(0)            # ensure we start at the beginning -- actually, can't do this with stdout, so this is commented out

    copy_flight_path(uuid_path)

    while True:
        if exit_code == 1:
            return

        line = pred_process.stdout.readline()
        if not line:
            break
        if line == b'':
            break

        # pass through
        # sys.stdout.write(line)
        # the required Python 3 obfuscation of the above line ...
        sys.stdout.write(line.decode(sys.stdout.encoding))

        pattern = re.compile(r"WARNING: Attempting to download additional wind data centred at (\d+\.\d+), (\d+\.\d+) and starting at (\d+)")
        patmatch = pattern.match(line.decode('utf-8')) 
        if patmatch is not None:
            newlatcen    = float(patmatch.group(1))
            newloncen    = float(patmatch.group(2))
            newtimestart = int(patmatch.group(3))
            if thelat != newlatcen or thelon != newloncen or thetime != newtimestart:
                log.info("Python code now attempting to download additional wind data centred at lat: ")
                log.info(newlatcen)
                log.info(" lon: ")
                log.info(newloncen)
                log.info(" and starting at: ")
                log.info(newtimestart)
                pred_output  = ["Attempting to download additional wind data centred at lat = {0}, lon = {1} and starting at {2}."
                                .format(newlatcen, newloncen, newtimestart),
                                "Now attempting to download additional wind data...",
                                ""] + pred_output
                download_and_process_wind_data(newlatcen, newloncen, newtimestart, options, pred_in_progress, pred_process, pred_output, uuid_path)
                if exit_code == 1:
                    return

        # if ("WARN" in line or "ERROR" in line) and len(pred_output) < 10:
        # more required Python 3 obfuscation ...
        if (b'WARN' in line or b'ERROR' in line) and len(pred_output) < 10:
            pred_output.append(line.strip())

    if exit_code != 1:
        exit_code = pred_process.wait()



def copy_flight_path(uuid_path):
    if OS_IS_WINDOWS:
        copy_path = os.path.join(ROOT_DIR, "predict")
    else:
        copy_path = '/tmp'            

    if os.path.exists(uuid_path+'flight_path.csv'):
        copy_path = os.path.join(copy_path, 'flight_path.csv')
        log.info('Copying file:')
        log.info(uuid_path+'flight_path.csv')
        log.info('to file:')
        log.info(copy_path)
        shutil.copyfile(uuid_path+'flight_path.csv', copy_path)


def purge_cache():
    """
    Purge the pydap cache (if set).
    """

    if pydap.lib.CACHE is None:
        return

    log.info('Purging PyDAP cache.')

    for file in os.listdir(pydap.lib.CACHE):
        log.debug('   Deleting %s.' % file)
        os.remove(pydap.lib.CACHE + file)

# NEW: mainly some variable name changes (eg. hgtprs is gh etc.): Amine's code
def write_file(output_format, dataset_list, window, mintime, maxtime):
    """
    dataset_list: list of (valid_datetime, xarray.Dataset) tuples.
    Each ds is 3D: (isobaricInhPa, latitude, longitude), vars gh,u,v,w,t.
    Writes one .dat file per forecast hour whose valid time is in [mintime, maxtime].
    """
    log.info('Writing data in window (lat, lon) = (%s +/- %s, %s +/- %s).' % window)
        
    # Filter to the forecast hours we actually want.
    wanted = list(dataset_list)
    if not wanted:
        # If nothing lands in-window, fall back to using all we have.
        wanted = dataset_list
    num_times = len(wanted)
    log.info('Writing %d forecast-hour files.' % num_times)
    
    starttime = datetime.now() 
    current_time = 0
    
    for (valid_dt, ds) in wanted:   
        current_time += 1
        timestamp = int(pd.Timestamp(valid_dt).timestamp())
        log.info('Writing data for %s (POSIX %s).' % (valid_dt.ctime(), timestamp))
        
        # Grab the five variables as plain numpy arrays: shape (lev, lat, lon).
        try:
            gh = ds['gh'].values
            u  = ds['u'].values
            v  = ds['v'].values
            t  = ds['t'].values
            w  = ds['w'].values
        except Exception as e:
            log.info('failed to get variable arrays: %s' % e)
            import traceback; traceback.print_exc()
            return 1
    
        levs = ds['isobaricInhPa'].values          # pressures, descending
        lats_all = ds['latitude'].values
        lons_all = ds['longitude'].values
        
        # Which latitudes/longitudes fall in the window.
        latitudes = [[i, la] for i, la in enumerate(lats_all)
                     if math.fabs(la - window[0]) <= window[1]]
        center = window[2]
        longitudes = []
        for i, lo in enumerate(lons_all):
            if longitude_distance(lo, center) <= window[3]:
                disp = lo
                while disp - center > 180:
                    disp -= 360
                while disp - center < -180:
                    disp += 360
                longitudes.append([i, disp])
        longitudes.sort(key=lambda x: x[1])  
        
        if not latitudes or not longitudes:
            log.info('window selects no grid points (lat=%s lon=%s)'
                     % (len(latitudes), len(longitudes)))
            return 1
        
        # Build the output filename for this timestamp.
        output_filename = output_format
        output_filename = output_filename.replace('%(time)', str(timestamp))
        output_filename = output_filename.replace('%(lat)', str(window[0]))
        output_filename = output_filename.replace('%(latdelta)', str(window[1]))
        output_filename = output_filename.replace('%(lon)', str(window[2]))
        output_filename = output_filename.replace('%(londelta)', str(window[3]))
        
        log.info('   Writing \'%s\'...' % output_filename)
        output = open(output_filename, 'w')
        
        # Header.
        output.write('# window centre latitude, window latitude radius, window centre longitude, window longitude radius, POSIX timestamp\n')
        header = window + (timestamp,)
        output.write(','.join(map(str, header)) + '\n')
        
        # Axis count.
        output.write('# num_axes\n')
        output.write('3\n')
                
        # Axis 1: pressures.
        output.write('# axis 1: pressures\n')
        output.write(str(len(levs)) + '\n')
        output.write(','.join(map(str, levs)) + '\n')
                     
        # Axis 2: latitudes.
        output.write('# axis 2: latitudes\n')
        output.write(str(len(latitudes)) + '\n')
        output.write(','.join(map(lambda x: str(x[1]), latitudes)) + '\n')
        
        # Axis 3: longitudes.
        output.write('# axis 3: longitudes\n')
        output.write(str(len(longitudes)) + '\n')
        output.write(','.join(map(lambda x: str(x[1]), longitudes)) + '\n')
        
        # Number of data lines.
        output.write('# number of lines of data\n')
        output.write('%s\n' % (len(levs) * len(latitudes) * len(longitudes)))
        
        # Components per line.
        output.write('# data line component count\n')
        output.write('5\n')
        
        output.write('# now the data in axis 3 major order\n')
        output.write('# data is: '  
                     'geopotential height [gpm], u-component wind [m/s], '
                     'v-component wind [m/s], temperature [K], '
                     'vertical velocity (pressure) [Pa/s]\n')
        output.flush()
        
        # Data: pressures outer, then latitudes, then longitudes.
        try:         
            for pidx in range(len(levs)):
                for (latidx, _lat) in latitudes:
                    for (lonidx, _lon) in longitudes:
                        record = (gh[pidx, latidx, lonidx],
                                  u[pidx, latidx, lonidx],
                                  v[pidx, latidx, lonidx],
                                  t[pidx, latidx, lonidx],
                                  w[pidx, latidx, lonidx])
                        output.write(','.join(map(str, record)) + '\n')
        except Exception as e:
            log.info('failed writing data records: %s' % e)
            import traceback; traceback.print_exc()
            output.close()
            return 1
        
        output.close()
        update_progress(gfs_percent=int(10 + (current_time * 90) / num_times),
                        gfs_timeremaining="Please wait...")
        log.debug('write_file of %s completed normally' % output_filename)
        
    return 0


def canonicalise_longitude(lon):
    """
    The GFS model has all longitudes in the range 0.0 -> 359.5. Canonicalise
    a longitude so that it fits in this range and return it.
    """
    lon = math.fmod(lon, 360)
    if lon < 0.0:
        lon += 360.0
    assert((lon >= 0.0) & (lon < 360.0))
    return lon

def longitude_distance(lona, lonb):
    """
    Return the shortest distance in degrees between longitudes lona and lonb.
    """
    distances = ( \
        math.fabs(lona - lonb),  # Straightforward distance
        360 - math.fabs(lona - lonb), # Other way 'round.
    )
    return min(distances)

def datetime_to_posix(time):
    """
    Convert a datetime object to a POSIX timestamp.
    """
    return calendar.timegm(time.timetuple())

def timestamp_to_datetime(timestamp):
    """
    Convert a GFS fractional timestamp into a datetime object representing 
    that time.
    """
    # The GFS timestmp is a floating point number fo days from the epoch,
    # day '0' appears to be January 1st 1 AD.

    (fractional_day, integer_day) = (0., 0)
    if hasattr(timestamp, "__len__"):
        (fractional_day, integer_day) = math.modf(timestamp[0])
    else:  
        (fractional_day, integer_day) = math.modf(timestamp)

    # Unfortunately, the datetime module uses a different epoch.
    ordinal_day = int(integer_day - 1)

    # Convert the integer day to a time and add the fractional day.
    # return datetime.datetime.fromordinal(ordinal_day) + \
    #     datetime.timedelta(days = fractional_day)
    return datetime.fromordinal(ordinal_day) + \
        timedelta(days = fractional_day)

def possible_urls(time, hd):
    """
    Given a datetime object representing a date and time, return a list of
    possible data URLs which could cover that period.

    The list is ordered from latest URL (i.e. most likely to be correct) to
    earliest.

    We assume that a particular data set covers a period of P days and
    hence the earliest data set corresponds to time T - P and the latest
    available corresponds to time T given target time T.
    """

    log.debug('start possible_urls at time = %s' % (time))
    # period = datetime.timedelta(days = 7.5)
    # log.debug('preperiod')
    period = timedelta(days = 15.0)
    # log.debug('period')
    maxtimeaheadofnow = timedelta(days = 1)
    # log.debug('maxtimeaheadofnow')
    # nomads dataset available times are screwed up online
    utcoffset = timedelta(hours = 7.0)
    # utcoffset = datetime.timedelta(hours = 15.0)
    # log.debug('utcoffset')

    earliest = time - period
    # latest = time
    latest = time - utcoffset
    log.debug('latest')
    # cantbelaterthan = datetime.utcnow() + maxtimeaheadofnow
    cantbelaterthan = datetime.now(timezone.utc) + maxtimeaheadofnow
    log.debug('latest = %s' % (latest))
    if latest > cantbelaterthan:
        latest = cantbelaterthan

    # nomads.ncep.noaa.gov now uses https rather than http (began Feb 2019):
    if hd:
        url_format = 'https://{host}/dods/gfs_0p25/gfs%i%02i%02i/gfs_0p25_%02iz'
    #    url_format = 'http://{host}:80/dods/gfs_0p25/gfs%i%02i%02i/gfs_0p25_%02iz'
    #    url_format = 'http://{host}:9090/dods/gfs_0p25/gfs%i%02i%02i/gfs_0p25_%02iz'
    #    url_format = 'http://{host}:9090/dods/gfs_hd/gfs_hd%i%02i%02i/gfs_hd_%02iz'
    else:
        url_format = 'https://{host}/dods/gfs_1p00/gfs%i%02i%02i/gfs_1p00_%02iz'
    #    url_format = 'http://{host}:80/dods/gfs_1p00/gfs%i%02i%02i/gfs_1p00_%02iz'
    #    url_format = 'http://{host}:9090/dods/gfs_1p00/gfs%i%02i%02i/gfs_1p00_%02iz'
    #    url_format = 'http://{host}:9090/dods/gfs/gfs%i%02i%02i/gfs_%02iz'

    # Often we have issues where one IP address (the DNS resolves to 2 or more)
    # will have a dataset and the other one won't yet.
    # This causes "blah is not an available dataset" errors since predict.py
    # thinks it's OK to use a recent one, and then by chance we end up talking
    # to a server on a later request that doesn't have it.
    # print('url_format =', url_format)
    selected_ip = socket.gethostbyname("nomads.ncep.noaa.gov")
    # log.debug('here3')
    log.info("Picked IP: {0}".format(selected_ip))
    # url_format = url_format.format(host=selected_ip)
    url_format = url_format.format(host='nomads.ncep.noaa.gov')
    # print('url_format including host =', url_format)

    # Start from the latest, work to the earliest
    proposed = latest
    possible_urls = []
    while proposed >= earliest:
        for offset in ( 18, 12, 6, 0 ):
            if proposed.day != latest.day or proposed.hour >= offset:
                possible_urls.append(url_format % \
                    (proposed.year, proposed.month, proposed.day, offset))
                print('.....................', offset, url_format % (proposed.year, proposed.month, proposed.day, offset))
        # proposed -= datetime.timedelta(days = 1)
        proposed -= timedelta(days = 1)
        print('.....................', proposed)
    
    print('possible_urls returns: ', possible_urls)

    return possible_urls

# NEW: Amine's code
def dataset_for_time(time, hd):
    """
    Grab GFS forecast hours from NOAA's S3 bucket
    Returns one list of (valid_datetime, xarray.Dataset) per hour.
    Each dataset is (isobaricInhPa, latitude, longitude).
    """ 
    sys.path.insert(0, ROOT_DIR.rstrip('/'))

    log.info(f"Looking for GFS data covering {time}")
    fxx_range = range(0, 27, 3)
            
    for offset in [6, 12, 18, 24]:
        run = time - timedelta(hours=offset)
        run = run.replace(hour=(run.hour // 6) * 6, minute=0, second=0,
                          microsecond=0, tzinfo=None)
        log.info(f"Trying run {run}")
        datasets = []
        try:
            for fxx in fxx_range:
                ds = open_forecast_hour(run, fxx)
                if ds is None:
                    raise RuntimeError(f"f{fxx:03d} not available for run {run}")
                valid = pd.Timestamp(run) + timedelta(hours=fxx)
                datasets.append((valid.to_pydatetime(), ds))

            latest = datasets[-1][0]
            if latest < time.replace(tzinfo=None):
                log.info(f"Run {run} only reaches {latest}; trying older run.")
                continue

            log.info(f"Got {len(datasets)} forecast hours from run {run}.")
            update_progress(gfs_timestamp=run.strftime("%Y%m%d_%Hz"))
            return datasets

        except Exception as e:
            log.info(f"Run {run} failed: {e}")
            import traceback; traceback.print_exc()
            continue

    raise RuntimeError('Could not find a complete GFS dataset')

 

def detach_process(redirect):
    # Fork
    if os.fork() > 0:
        os._exit(0)

    # Detach
    os.setsid()

    null_fd = os.open(os.devnull, os.O_RDONLY)
    out_fd = os.open(redirect, os.O_WRONLY | os.O_APPEND)

    os.dup2(null_fd, sys.stdin.fileno())
    for s in [sys.stdout, sys.stderr]:
        os.dup2(out_fd, s.fileno())

    # Fork
    if os.fork() > 0:
        os._exit(0)

def alarm_workaround(parent):
    # wait for the parent
    parent.join(600)
    # if the parent (main) thread is still alive, then we need to kill it
    if parent.isAlive():
        os._exit(0)

def setup_alarm():
    # Prevent hung download:
    if OS_IS_WINDOWS:
        import threading
        t = threading.Thread(target=alarm_workaround, args=(threading.currentThread(),))
        # setting the thread as a daemon means we don't need to worry about cleaning it up
        t.daemon = True
        t.start()
    else:
        signal.alarm(600)

# If this is being run from the interpreter, run the main function.
if __name__ == '__main__':
    try:
        main()
    except SystemExit as e:
        log.debug("Exit: " + repr(e))
        if e.code != 0 and progress_f:
            update_progress(error="Unknown error exit")
            statsd.increment("unknown_error_exit")
        raise
    except Exception as e:
        statsd.increment("uncaught_exception")
        log.exception("Uncaught exception")
        info = traceback.format_exc()
        if progress_f:
            update_progress(error="Unhandled exception: " + info)
        raise
