// --------------------------------------------------------------
// CU Spaceflight Landing Prediction
// Copyright (c) CU Spaceflight 2009, All Right Reserved
//
// Written by Rob Anderson 
// Modified by Fergus Noble
//
// THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
// --------------------------------------------------------------

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

extern "C" {
#include "wind/wind_file_ALTAIR.h"
#include "util/random.h"
}

#include "run_model_StationKeep.hh"
#include "pred_StationKeep.hh"

#include "state/ALTAIR_state.hh"
#include "state/ExternalEnvironState.hh"
#include "state/GondolaAndPropState.hh"
// #include "util/ThrustCalcMethods.hh"
// #include "util/DragCalcMethods.hh"
#include "util/StationKeepingAlgorithms.hh"
#include "util/PropulsionUtils.hh"
#include "util/SolarPowerCalcMethods.hh"
#include "util/UpdateALTAIRState.hh"

extern int verbosity;

#define RADIUS_OF_EARTH 6371009.f

typedef struct model_state_s model_state_t;
struct model_state_s
{
    float               lat;
    float               lng;
    float               alt;
//    altitude_model_t   *alt_model;
    double              loglik;
};

// Get the distance (in metres) of one degree of latitude and one degree of
// longitude. This varys with height (not much grant you).
static void
_get_frame(float lat, float lng, float alt, 
        float *d_dlat, float *d_dlng)
{
    float theta, r;

    theta = 2.f * M_PI * (90.f - lat) / 360.f;
    r = RADIUS_OF_EARTH + alt;

    // See the differentiation section of
    // http://en.wikipedia.org/wiki/Spherical_coordinate_system

    // d/dv = d/dlat = -d/dtheta
    *d_dlat = (2.f * M_PI) * r / 360.f;

    // d/du = d/dlong = d/dphi
    *d_dlng = (2.f * M_PI) * r * sinf(theta) / 360.f;
}

static int 
_advance_one_timestep(wind_file_cache_t** cacheAddr, 
                      unsigned long delta_t,
                      unsigned long timestamp, unsigned long initial_timestamp,
                      unsigned int n_states, model_state_t* states,
                      float rmserror)
{
    unsigned int i;

    for(i=0; i<n_states; ++i)
    {
        float ddlat, ddlng;
        float wind_v, wind_u, wind_var, pres, temp, wind_z_Pas;
        float prop_v, prop_u, prop_z;
        float u_samp, v_samp, u_lik, v_lik;
        float prev_alt, prev_pres, wind_zvel = 0.;
        float battLevel;
        model_state_t* state = &(states[i]);

        prev_alt  = state->alt;

// the below both updates and returns the altitude (it does more than just get it -- it updates it!)
//        if(!altitude_model_get_altitude(state->alt_model, 
        assert(!isnan(state->lat));
        assert(!isnan(state->alt));
        if(!altitude_model_get_altitude( 
                                        timestamp - initial_timestamp, &state->alt))
            return 0; // alt < 0; finished

        if(!get_wind(cacheAddr, state->lat, state->lng, state->alt, timestamp, 
                     &wind_v, &wind_u, &wind_var, &pres, &temp, &wind_z_Pas))
            return -1; // error

        PropulsionUtils::getPropVelocityVector(&prop_u, &prop_v, &prop_z);

        _get_frame(state->lat, state->lng, state->alt, &ddlat, &ddlng);

        // NOTE: it this really the right thing to be doing? - think about what
        // happens near the poles

        wind_var += rmserror * rmserror;

        assert(wind_var >= 0.f);

        battLevel  = altairState->getGondAndProp()->getBatteryStoredEnergy();
        battLevel -= delta_t * PropulsionUtils::getExpendedPower();
        if (battLevel < 0.) battLevel = 0.;

        //fprintf(stderr, "U: %f +/- %f, V: %f +/- %f\n",
        //        wind_u, sqrtf(wind_u_var),
        //        wind_v, sqrtf(wind_v_var));

        u_samp      = random_sample_normal(wind_u, wind_var, &u_lik);
        v_samp      = random_sample_normal(wind_v, wind_var, &v_lik);
        if (battLevel > 0.) {
            u_samp += prop_u;
            v_samp += prop_v;
        }
        //u_samp = wind_u;
        //v_samp = wind_v;

        state->lat += v_samp * delta_t / ddlat;
        state->lng += u_samp * delta_t / ddlng;

        prev_pres = altairState->getExtEnv()->getOutsideAirPressure();
        altairState->getExtEnv()->setOutsideAirPressure(pres);
        altairState->getExtEnv()->setOutsideTemp(temp);
// wind_z_Pas is in Pa/s rather than m/s: want to multiply by m/Pa (= d(alt)/d(pres)) to get m/s
        if (pres != prev_pres) wind_zvel = wind_z_Pas * (state->alt - prev_alt) / (pres - prev_pres);
// and make sure it is not a crazy value
        if (wind_zvel > 5. || wind_zvel < -5.) wind_zvel = 0.;
// add in any vertical speed from propulsion
        if (battLevel > 0.) wind_zvel += prop_z;
        state->alt += (wind_zvel * delta_t);
// add battery energy from solar panels
        if (battLevel > 0.) battLevel += delta_t * SolarPowerCalcMethods::getInstantaneousSolarPower();

        UpdateALTAIRState::doUpdate(state->lat, state->lng, state->alt, battLevel);

        state->loglik += (double)(u_lik + v_lik);
    }

    return 1; // OK, and continue
}

static int _state_compare_rev(const void* a, const void *b)
{
    model_state_t* sa = (model_state_t*)a;
    model_state_t* sb = (model_state_t*)b;

    // this returns a value s.t. the states will be sorted so that
    // the maximum likelihood state is at position 0.
    return sb->loglik - sa->loglik;
}

int run_model(wind_file_cache_t** cacheAddr, 
//            altitude_model_t* alt_model,
              float initial_lat, float initial_lng, float initial_alt,
              long int initial_timestamp, float rmswinderror) 
{
    model_state_t* states;
    const unsigned int n_states = 1;
    unsigned int i;

    states = (model_state_t*) malloc( sizeof(model_state_t) * n_states );

    for(i=0; i<n_states; ++i) 
    {
        model_state_t* state = &(states[i]);

        state->alt = initial_alt;
        state->lat = initial_lat;
        state->lng = initial_lng;
//        state->alt_model = alt_model;
        state->loglik = 0.f;
    }

    long int timestamp = initial_timestamp;
    
    int log_counter = 0; // only write position to output files every LOG_DECIMATE timesteps
    int r, return_code = 1;

    while(1)
    {
        if(log_counter%1000 == 0) { fprintf(stderr, "INFO: Am here in the run_model loop.\n"); fflush(stderr); }
        r = _advance_one_timestep(cacheAddr, TIMESTEP, timestamp, initial_timestamp, 
                                  n_states, states, rmswinderror);
        if (r == -1) // error getting wind. Save prediction, but emit error messages
            return_code = 0;

        if (r != 1) // 1 = continue
            break;

        // Sort the array of models in order of log likelihood. 
        qsort(states, n_states, sizeof(model_state_t), _state_compare_rev);

        // write the maximum likelihood state out.
        if (log_counter == LOG_DECIMATE) {
            write_position(states[0].lat, states[0].lng, states[0].alt, timestamp);
            log_counter = 0;
        }

        log_counter++;
        timestamp += TIMESTEP;

//        PropulsionUtils::goFullSpeedAhead();
        StationKeepingAlgorithms::doStationKeep( cacheAddr );
    }

    for(i=0; i<n_states; ++i) 
    {
        model_state_t* state = &(states[i]);
        write_position(state->lat, state->lng, state->alt, timestamp);
    }

    fprintf(stderr, "INFO: Final maximum log lik: %f (=%f)\n", 
            states[0].loglik, exp(states[0].loglik));

    free(states);

    return return_code;
}

int get_wind(wind_file_cache_t** cacheAddr, float lat, float lng, float alt, long int timestamp,
        float* wind_v, float* wind_u, float *wind_var, float *pres, float *temp, float *wind_z) {
    char* receivedLine;
    size_t bufLen = 10;
    ssize_t charsReceived;
    int brandNewCache = 0;
    int s1, s2, i, n, m = 0, alreadySaidIt = 0;
    float lambda, wu_l, wv_l, wu_h, wv_h, pres_l, pres_h, temp_l, temp_h, wz_l, wz_h;
    float wuvar_l, wvvar_l, wuvar_h, wvvar_h;
    wind_file_cache_entry_t* found_entries[] = { NULL, NULL };
    wind_file_t* found_files[] = { NULL, NULL };
    unsigned int earlier_ts, later_ts;

    receivedLine = (char *)malloc(bufLen * sizeof(char));

    while(1) {
        // if(brandNewCache && m%1000 == 0) { fprintf(stderr, "INFO: I appear to have arrived here.\n"); fflush(stderr); }
        // look for a wind file which matches this latitude and longitude...
        wind_file_cache_find_entry(*cacheAddr, lat, lng, timestamp, 
                &(found_entries[0]), &(found_entries[1]));
        // if(brandNewCache && m%1000 == 0) { fprintf(stderr, "INFO: But have I arrived here?\n"); fflush(stderr); }
        if(!found_entries[0] || !found_entries[1]) {
           // Give the Python code a signal message in the log file to try to get the Python code to download new additional wind files.
           if(!alreadySaidIt) {
               fprintf(stderr, "WARNING: Attempting to download additional wind data centred at %f, %f and starting at %ld.  This will take at _least_ a minute or so...\n", lat, lng, timestamp);
               fflush(stderr);
               alreadySaidIt = 1;
           }
           if(brandNewCache && m%1000 == 0) { 
               fprintf(stderr, "INFO: Arrived at this point.  Why is the cache not properly expanded to include the new entries??\n");
               fprintf(stderr, "INFO: Lat = %f, Lon = %f, Timestamp = %ld\n", lat, lng, timestamp);
               fflush(stderr); 
           }
           // Look for a signal in stdin (specifically, the character 'D') that the Python code has completed the download of the wind files!  (Otherwise, one could unintentionally
           // cache partially-downloaded files.)  Then, re-populate the wind data file cache, and go around the loop to find a good entry from the updated cache.
//           fscanf(stdin, "%1[^\n]", stdInput); // Scan a maximum of 1 character (+1 for the null-terminator '\0'), or until a \n, and store it in stdInput
//           if(stdInput == 'D') {
//           if(stdInput[0]) {
//           if(fgets(stdInput, 1024, stdin)!=NULL) {
//           if(scanf("%d", &n) != 0) {
           charsReceived = getline(&receivedLine, &bufLen, stdin);
           if(charsReceived > 0) {
           //    fprintf(stderr, "INFO: The pred C code got: %d  from the Python code, now refreshing the pred's wind file cache to include the new wind data centred at %f, %f and starting at %ld!\n", n, lat, lng, timestamp);
               fprintf(stderr, "INFO: The pred C code got: %s  from the Python code, now refreshing the pred's wind file cache to include the new wind data centred at %f, %f and starting at %ld!\n", receivedLine, lat, lng, timestamp);
           //    fprintf(stderr, "INFO: The pred C code got the big D from the Python code parent process writing that input into the pred's stdin, now refreshing the pred's wind file cache!\n");
               fflush(stderr);
               wind_file_cache_free(*cacheAddr);
               *cacheAddr = wind_file_cache_new(data_dir);
               brandNewCache = 1;
               continue;
           //    stdInput[0] = 0;
           }
        } else {
           // if(brandNewCache) fprintf(stderr, "Good news!: Found new good wind data!\n");                 // found it!
           if(!wind_file_cache_entry_contains_point(found_entries[0], lat, lng) || 
              !wind_file_cache_entry_contains_point(found_entries[1], lat, lng))
           {
              fprintf(stderr, "ERROR: Could not locate appropriate wind data tile for location "
                      "lat=%f, lon=%f.\n", lat, lng);
              return 0;
           }
          // Look in the cache for the files we need.
          for(i=0; i<2; ++i)
          {
              found_files[i] = wind_file_cache_entry_file(found_entries[i]);
          }
          earlier_ts = wind_file_cache_entry_timestamp(found_entries[0]);
          later_ts = wind_file_cache_entry_timestamp(found_entries[1]);

          if(earlier_ts > timestamp || later_ts < timestamp)
          {
              fprintf(stderr, "Error: found_entries have bad times.\n");
              return 0;
          }
          if(earlier_ts != later_ts)
              lambda = ((float)timestamp - (float)earlier_ts) /
                       ((float)later_ts - (float)earlier_ts);
          else
              lambda = 0.5f;

          s1 = wind_file_get_wind(found_files[0], lat, lng, alt, &wu_l, &wv_l, &wuvar_l, &wvvar_l,
                                                                 &pres_l, &temp_l, &wz_l);
          s2 = wind_file_get_wind(found_files[1], lat, lng, alt, &wu_h, &wv_h, &wuvar_h, &wvvar_h,
                                                                 &pres_h, &temp_h, &wz_h);
          if(s1 == 0 || s2 == 0) {
               if(alreadySaidIt != 2) {
                   fprintf(stderr, "WARNING: Attempting to download additional wind data centred at %f, %f and starting at %ld.  New lat-lon.  This will take at _least_ a minute or so...\n", lat, lng, timestamp);
                   fflush(stderr);
                   alreadySaidIt = 2;
               }
               charsReceived = getline(&receivedLine, &bufLen, stdin);
               if(charsReceived > 0) {
                   fprintf(stderr, "INFO: The pred C code got: %s  from the Python code, now refreshing the pred's wind file cache to include the new wind data centred at %f, %f and starting at %ld! New lat-lon.\n", receivedLine, lat, lng, timestamp);
                   fflush(stderr);
                   wind_file_cache_free(*cacheAddr);
                   *cacheAddr = wind_file_cache_new(data_dir);
                   brandNewCache = 2;
                   continue;
               }
           } else { 
               // if(brandNewCache == 2) fprintf(stderr, "Good news!: Found new good wind data at new lat-lon!\n");                 // found it!
               break;
           }
        }
        ++m;
    }
    // fprintf(stderr, "...and escaped the while loop\n");  // don't uncomment this unless _absolutely_ necessary -- it spews this message to py_log endlessly...

    // if(!found_entries[0] || !found_entries[1]) {
    //     fprintf(stderr, "WARNING: Do not have wind data for this (lat, lon, alt, time) = (%f, %f, %f, %ld)!\n", lat, lng, alt, timestamp);
    //     fprintf(stderr, "WARNING: Attempting to download additional wind data centred at %f, %f and starting at %ld.  This will take at _least_ a minute or so...\n", lat, lng, timestamp);
    //     return 0;
    // }



//    s = wind_file_get_wind(found_files[0], lat, lng, alt, &wu_l, &wv_l, &wuvar_l, &wvvar_l,
//                                                          &pres_l, &temp_l, &wz_l);
//    if (s == 0) return 0; // hard error
//    s = wind_file_get_wind(found_files[1], lat, lng, alt, &wu_h, &wv_h, &wuvar_h, &wvvar_h,
//                                                          &pres_h, &temp_h, &wz_h);
//    if (s == 0) return 0;


    free(receivedLine);

    assert(!isnan(wvvar_h));
    assert(!isnan(wuvar_h));
    assert(!isnan(wuvar_l));
    assert(!isnan(wvvar_l));

    *wind_u = lambda * wu_h   + (1.f-lambda) * wu_l  ;
    *wind_v = lambda * wv_h   + (1.f-lambda) * wv_l  ;
    *pres   = lambda * pres_h + (1.f-lambda) * pres_l;
    *temp   = lambda * temp_h + (1.f-lambda) * temp_l - kelvinMinusCelsius;
    *wind_z = lambda * wz_h   + (1.f-lambda) * wz_l  ;

    // flatten the u and v variances into a single mean variance for the
    // magnitude.
    *wind_var = 0.5f * (wuvar_h + wuvar_l + wvvar_h + wvvar_l);

    return 1;
}

void write_position(float lat, float lng, float alt, int timestamp) {
    float                 prop_v, prop_u, prop_z                                 ;

//    float                 thrust        = ThrustCalcMethods::getInterpMethodThrust( );
//    float                 terminalSpeed = DragCalcMethods::getTerminalSpeed( thrust );
//    float                 expPower      = thrust * terminalSpeed / ALTAIRPropulsiveEfficiency;

    GondolaAndPropState*  gondAndProp             = altairState->getGondAndProp();

    // the predictor uses 0<=lng<360; most other things expect -180<lng<=180
    if (lng > 180)
        lng -= 360;

    if (kml_file) {
        fprintf(kml_file, "%g,%g,%g\n", lng, lat, alt);
        if (ferror(kml_file)) {
          fprintf(stderr, "ERROR: error writing to KML file\n");
          exit(1);
        }
    }

//    fprintf(output, "%d,%g,%g,%g\n", timestamp, lat, lng, alt);
//    fprintf(output, "%d,%g,%g,%g,%g\n", timestamp, lat, lng, alt, altairState->getGondAndProp()->getBatteryStoredEnergy());
    PropulsionUtils::getPropVelocityVector(&prop_u, &prop_v, &prop_z);
    fprintf(output, "%11d,%4.4f,%4.4f,%7.1f,%6.0f,%6.3f,%6.3f,%6.3f,%4d,%4d,%4d,%4d\n", timestamp, lat, lng, alt, gondAndProp->getBatteryStoredEnergy(),
            prop_u, prop_v, prop_z, gondAndProp->getRPMMotor1(), gondAndProp->getRPMMotor2(), gondAndProp->getRPMMotor3(), gondAndProp->getRPMMotor4());
    fflush(output);
//    expPower = PropulsionUtils::getExpendedPower();
//    fprintf(output, "%11d,%4.4f,%4.4f,%7.1f,%6.0f,%6.3f,%6.3f,%6.3f,%4d,%4d,%4d,%4d,%6.3f,%6.3f,%6.3f\n", timestamp, lat, lng, alt, gondAndProp->getBatteryStoredEnergy(),
//            prop_u, prop_v, prop_z, gondAndProp->getRPMMotor1(), gondAndProp->getRPMMotor2(), gondAndProp->getRPMMotor3(), gondAndProp->getRPMMotor4(),
//            thrust, terminalSpeed, expPower);
    if (ferror(output)) {
      fprintf(stderr, "ERROR: error writing to CSV file\n");
      exit(1);
    }
}

// vim:sw=4:ts=4:et:cindent
