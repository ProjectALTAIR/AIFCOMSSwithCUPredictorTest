# AIFCOMSS with CUSF Prediction

Windows screenshot                                   Mac screenshot

![Windows screenshot](https://github.com/ProjectALTAIR/AIFCOMSSwithCUPredictorTest/raw/master/screenshots/AIFCOMSSScreenshotWindows_19jun18.jpg)

## Installation

First, make sure that your Mac, Linux, or Windows computer has Python 
([https://www.python.org](https://www.python.org)) - but NOT version 3.x of Python - version 2.7 is best (note that Python v3 is really a quite different language from Python v2, and not backwards-compatible with v2), installed 
(and if not, then please install Python v2.7, using the instructions on the Python website.  Install pip, if asked, during the Python installation.) <br>

On <i>any</i> platform (Mac, Linux, or Windows), this package <i>also</i> requires the installation of an 
AMP (Apache/MySQL/PHP) stack. Instructions can be found online for:

 * Ubuntu Linux: [https://www.unixmen.com/how-to-install-lamp-stack-on-ubuntu-16-04/](https://www.unixmen.com/how-to-install-lamp-stack-on-ubuntu-16-04/)
 * Mac OS: Instructions for setting up an AMP stack in High Sierra, i.e. Mac OS 10.13, can be found (for example) [here](https://websitebeaver.com/set-up-localhost-on-macos-high-sierra-apache-mysql-and-php-7-with-sslhttps).  Note that all versions of Mac OS X already come with both Apache and PHP installed (you just need to turn them on, and if you have a pre-High Sierra OS version, i.e. pre- Mac OS 10.13, then you'll need to upgrade your PHP from v.5 to v.7.1, but you won't need to install either Apache or PHP ab initio).  As mentioned in the above instructions, though, you will, however, need to install MySQL.  You can simply google `mac apache` for instructions on setting the AMP stack up in earlier OS X versions.  And (as mentioned below) ignore any optional steps - the goal is just to get PHP pages to display.
 * Windows: [http://www.ampps.com/](http://www.ampps.com/)
	 * NOTE: install AMPPS into a location that does not have spaces, ie. NOT `Program Files` -- I recommend that it be installed directly under C:\ , i.e. as C:\Ampps\ .

Ignore any optional steps - the goal is just to get PHP pages to display.

Note that on Windows, it is probable that port 80 will be occupied preventing PHP to run. To fix the problem,      
run the command terminal as administrator (by right-clicking on cmd.exe, and selecting Run as Administrator), and enter the following line :

        $ net stop http

This will terminate unimportant Windows processes using port 80.  (Mind that Skype may also use port 80 
unless you change your Skype settings.)

Now, navigate to the directory from which your HTML and PHP files are being served (in Linux for example, 
this would be `/var/www/` or `/var/www/html/`; for AMPPS on Windows, this would be `C:\path\to\Ampps\www\`, i.e. the installation path of AMPPS; and for Mac OS, this would likely be `/Users/<your user name>/Sites/`) and install Node.js ([https://nodejs.org/en/](https://nodejs.org/en/)). Then install version 1.29 (not the most recent version, but version 1.29) of Cesium.js ([http://cesiumjs.org/downloads.html](http://cesiumjs.org/downloads.html)) to an **entirely** new subdirectory here (i.e. create a new subdirectory before installing Cesium.js v1.29 into that subdirectory) - you'll need it later. For Ubuntu, and Linux in general, Node-v6.11.2 is required. It is very easy to install through the following commands :

        $ sudo apt-get install curl
        $ curl -sL https://deb.nodesource.com/setup_6.x | sudo bash -
        $ sudo apt-get install nodejs

Back in your `/<www>/` (i.e., `/var/www/` or `/var/www/html/` or `C:\Ampps\www\` or `/Users/<your user name>/Sites/`, etc etc) directory, download the package repository at
[https://github.com/ProjectALTAIR/AIFCOMSSwithCUPredictorTest](https://github.com/ProjectALTAIR/AIFCOMSSwithCUPredictorTest)
so that all the project files are located at `/<www>/AIFCOMSSwithCUPredictorTest/`  (and **note that your directory should be called** `AIFCOMSSwithCUPredictorTest`, ***not*** `AIFCOMSSwithCUPredictorTest-master` -- if it is the latter, just rename it to be the former). Copy all of the files
***except server.js and package.json*** from your Cesium directory into
`/<www>/AIFCOMSSwithCUPredictorTest/predict`, leaving the original `server.js` and `package.json` files
from this GitHub AIFCOMSSwithCUPredictorTest repository intact.  (A few other files will show up as duplicates;
users should replace those other existing files in `/<www>/AIFCOMSSwithCUPredictorTest/predict` with the new files from your Cesium directory, rather than just ignoring those files.)

There are a few paths that need to be changed at this point:

 * In `/<www>/AIFCOMSSwithCUPredictorTest/predict/includes/php_variables.php`, change `ROOT_DIR` to the location of the project folder (e.g., `/var/www/html/AIFCOMSSwithCUPredictorTest/` or `C:\\Ampps\\www\\AIFCOMSSwithCUPredictorTest\\` etc). **Make sure the path ends with a `/` or `\\` (the latter on Windows)**.  Also, change `PYTHON_PATH` to the location of your Python executable (eg., `/usr/bin/python` on Linux or `C:\\Python27\\python.exe` on Windows). **On Windows, make sure you have *double* backslashes in the filenames, to prevent the backslashes from being interpreted as escape characters.  Note that *some* Windows versions (notably Windows 10) can autocorrect file path names, so in that case it is slightly less important to ensure that you always have double backslashes, however in all cases it is best to not rely on autocorrection, and so you should always put double backslashes within the filenames inside these files on Windows PCs.  You can change the paths inside these files on Windows by opening the files in WordPad, for example.**
 * In `/<www>/AIFCOMSSwithCUPredictorTest/predict/py_variables.py`, change `ROOT_DIR` to the same value as you set it in the above (`php_variables.php`) file.  
 * And in `/<www>/AIFCOMSSwithCUPredictorTest/predict/js_variables.js`, change exports.ROOT_DIR to be the same value, but with the addition of predict/ at the end, i.e. `/var/www/html/AIFCOMSSwithCUPredictorTest/predict/` or `C:\\Ampps\\www\\AIFCOMSSwithCUPredictorTest\\predict\\` etc.

and one that might possibly need to be (depending on your setup):

 * In `/<www>/AIFCOMSSwithCUPredictorTest/predict/AIFCOMSS.html`, change the path of `AIFpred.php` to the correct location, <i>if</i> the correct location happens to be different than `http://localhost/AIFCOMSSwithCUPredictorTest/predict/AIFpred.php` in your setup.

The next steps depend heavily on the operating system.

#### Linux (tested on Mint 18.2)

The following packages are needed and can be installed from your package manager (eg. `sudo apt-get install [package]`):

 * build-essential
 * cmake
 * pkg-config
 * pkg-config-dev (if available)
 * libglib2.0
 * libglib2.0-dev
 * python-dev
 * python-pip
 * setuptools (install with pip)

Navigate to `/var/www/AIFCOMSSwithCUPredictorTest/pred_src` and run

    $ cmake .
    $ make

to compile the code found there. Once compiled, the following files must be made executable (`sudo chmod +x [file]`):

 * `/var/www/AIFCOMSSwithCUPredictorTest/predict.py`
 * `/var/www/AIFCOMSSwithCUPredictorTest/predict/predict.py`
 * `/var/www/AIFCOMSSwithCUPredictorTest/pred_src/pred`
 * `/var/www/AIFCOMSSwithCUPredictorTest/cron/clear-pydap-cache-cronjob.sh`
 * `/var/www/AIFCOMSSwithCUPredictorTest/cron/prune-predictions-cronjob.sh`

Furthermore, the `/var/www/AIFCOMSSwithCUPredictorTest/predict/preds` and `/var/www/AIFCOMSSwithCUPredictorTest/gfs` 
directories must be given full (rwx) access by the PHP interpreter and both `predict.py` files. The simplest way to do so is 
using `sudo chmod a+rwx [directory]`, although safety-conscious users may want to be more selective with their permissions.

Navigate back to `/var/www/AIFCOMSSwithCUPredictorTest/` and run `sudo -H pip install -r requirements_linux.txt` to install some of the 
Python requirements. Change directories to `/var/www/AIFCOMSSwithCUPredictorTest/predict/` and run `npm update` to install the remaining requirements.

In this same `predict` directory, run `node server.js`.  Open a web browser, and navigate to `http://localhost:8080/AIFCOMSS.html`.

You should then have AIFCOMSS running in your web browser.

#### Mac OS X (tested on v. 10.13.5 "High Sierra")

Check that you have CMake installed on your Mac by typing `which cmake` in a terminal window from your
`/Users/<your user name>/Sites/AIFCOMSSwithCUPredictorTest/` directory.  If you don't have it, then download the .dmg
Mac installer for it from https://cmake.org/download/ , and then install it, following all instructions.  After that,
`which cmake` should find your installed CMake executable in `/usr/local/bin/cmake`.

Then, navigate to `/Users/<your user name>/Sites/AIFCOMSSwithCUPredictorTest/pred_src` and run

    $ cmake .
    $ make

to compile the code found there. Once compiled, the following files must be made executable (`sudo chmod +x [file]`):

 * `/Users/<your user name>/Sites/AIFCOMSSwithCUPredictorTest/predict.py`
 * `/Users/<your user name>/Sites/AIFCOMSSwithCUPredictorTest/predict/predict.py`
 * `/Users/<your user name>/Sites/AIFCOMSSwithCUPredictorTest/pred_src/pred`
 * `/Users/<your user name>/Sites/AIFCOMSSwithCUPredictorTest/cron/clear-pydap-cache-cronjob.sh`
 * `/Users/<your user name>/Sites/AIFCOMSSwithCUPredictorTest/cron/prune-predictions-cronjob.sh`

Furthermore, the `/Users/<your user name>/Sites/AIFCOMSSwithCUPredictorTest/predict/preds` and `/Users/<your user name>/Sites/AIFCOMSSwithCUPredictorTest/gfs` 
directories must be given full (rwx) access by the PHP interpreter and both `predict.py` files. The simplest way to do so is 
using `sudo chmod a+rwx [directory]`, although safety-conscious users may want to be more selective with their permissions.

Navigate back to `/Users/<your user name>/Sites/AIFCOMSSwithCUPredictorTest/` and run `sudo -H pip install -r requirements_macos.txt` to install some of the 
Python requirements. Change directories to `/Users/<your user name>/Sites/AIFCOMSSwithCUPredictorTest/predict/` and run `npm update` to install the remaining requirements.

In this same `predict` directory, run `node server.js`.  Open a web browser, and navigate to `http://localhost:8080/AIFCOMSS.html`.

You should then have AIFCOMSS running in your web browser.

#### Windows (tested on Windows 7)

Windows has no built in C compiler or interpreter.

Thus, Cygwin (or some other analogous environment: if you, for example, prefer VirtualBox to Cygwin, then see the VirtualBox 
instructions starting about 40 lines below.  Visual Studio \[[https://www.visualstudio.com](https://www.visualstudio.com)]
would be a third option, if you so choose.  Note that if you choose to install Visual Studio \[rather than Cygwin or VirtualBox\] on
your Windows machine, then after you install Visual Studio, all commands below must be run in powershell, rather
than in the standard Windows command - you can access powershell from the standard Windows command line by
typing: `$ powershell`, and note that it is also necessary to run the command terminal with Administrator privileges) is required to compile some of the source files - download Cygwin at 
[https://www.cygwin.com](https://www.cygwin.com). Run the setup, and in addition to the default packages also install:

 - devel -> gcc-g++
 - devel -> make
 - devel -> cmake
 - libs -> libglib2.0-devel
 - libs -> libglib2.0_0

It's recommended that you do not add the Cygwin bin folders to your system path, as this will replace some of the usual Windows 
cmd commands.

Launch the Cygwin terminal, and navigate to the `AIFCOMSSwithCUPredictorTest/pred_src` directory - note that the `C:` drive is 
located at `/cygdrive/c` when using this terminal. Run

    cmake .
    make

to build the source files, and then close the Cygwin terminal. Copy the following files from the `bin` folder in Cygwin's 
installation directory to the `pred_src` directory containing `pred.exe`:

 -  `cygglib-2.0-0.dll`
 -  `cygiconv-2.dll`
 -  `cygintl-8.dll`
 -  `cygpcre-1.dll`
 -  `cygwin1.dll`

Open a command terminal, navigate to the `AIFCOMSSwithCUPredictorTest` directory, and run

    pip install -r requirements_win.txt
    pip install numpy

to install some of the Python requirements.  (Note that you might need to use the full path to the location of your pip.exe executable.)

Then navigate to the `AIFCOMSSwithCUPredictorTest/predict` directory and run

    npm install

then

    npm update

to install the remaining requirements.  (Note that if `npm install` does not work for you within a command terminal, please instead try it within either a cygwin terminal, or powershell, from the `AIFCOMSSwithCUPredictorTest/predict` directory as above.) 

Launch Ampps, and in your web browser navigate to `localhost/ampps`. Click on Apache Configuration, turn `mod_headers` on, and 
restart Apache.

In a terminal, navigate to the `AIFCOMSSwithCUPredictorTest/predict` directory, and run

    node server.js

Open your web browser, and navigate to `localhost:8080/AIFCOMSS.html`. You should then have AIFCOMSS running in your browser.  

**If** the bottom frame (the p5.js window) always just says "Loading ..." (that problem can often occur, especially on some Windows systems), then modify line 6 of `predict\ALTAIR_entities_p5frame.js` to change the value of the
audibleAlarms variable on that line from `true` to `false`, and then re-run (and let me know if you have any problems).


<i>If you prefer VirtualBox to Cygwin</i>:

Install VirtualBox [https://www.virtualbox.org/wiki/Downloads](https://www.virtualbox.org/wiki/Downloads) and create a virtual 
machine. I would recommend 2GB of RAM and 25 GiO of memory allocated to your virtual machine. Once your virtual machine is 
created, download Ubuntu [https://www.ubuntu.com/download/desktop](https://www.ubuntu.com/download/desktop) then run it as a 
virtual optical disk on your virtual machine, install it, and you're done! General instructions can be found here : 
[https://linus.nci.nih.gov/bdge/installUbuntu.html](https://linus.nci.nih.gov/bdge/installUbuntu.html)

Then follow the upper instructions concerning Linux installation (though be sure to note that the port 80 problem described above remains, as your 
connection is provided by a Windows machine).

#### Some debugging hints if you need them (all OS's)

Run your server, and use the developer Javascript console of your browser to find any display or protocol errors. Identify the code language at the source of the error, and the use its log file -- see below for location -- to solve the problem.

The log files for the different code languages used can be found in the locations below :

 * Javascript : The developer Javascript console of the server (you can find this in your web browser).
 * PHP : /var/log/apache2/error.log (on Linux)
 * Python : /var/www/AIFCOMSSwithCUPredictorTest/predict/pred/[the folder corresponding to your run] (on Linux)


