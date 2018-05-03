# AIFCOMSS with CUSF Prediction
## Installation

First, make sure that your Mac, Linux, or Windows computer has Python 
([https://www.python.org](https://www.python.org)) - but NOT version 3.x of Python - version 2.7 is best (note that Python v3 is really a quite different language from Python v2, and not backwards-compatible with v2), installed 
(and if not, then please install Python v2.7, using the instructions on the Python website.  Install pip, if asked, during the Python installation.) <br>

Then additionally, for the specific case of Windows machines (but this is not necessary on Mac or Linux), 
please also install <i>either</i> Cygwin ([https://www.cygwin.com](https://www.cygwin.com)) <i>or</i> Visual Studio ([https://www.visualstudio.com](https://www.visualstudio.com)).  Note that if you choose to install Visual Studio (rather than Cygwin) on
your Windows machine, then after you install Visual Studio, all commands below must be run in powershell, rather
than in the standard Windows command - you can access powershell from the standard Windows command line by
typing: `$ powershell`, and note that it is also necessary to run the command terminal with Administrator privileges.<br>

Following that, on <i>any</i> platform (Mac, Linux, or Windows), this package requires the installation of an 
AMP (Apache/MySQL/PHP) stack. Instructions can be found online for:

 * Ubuntu Linux: [https://www.unixmen.com/how-to-install-lamp-stack-on-ubuntu-16-04/](https://www.unixmen.com/how-to-install-lamp-stack-on-ubuntu-16-04/)
 * Mac OS: [http://www.ampps.com/](http://www.ampps.com/)
 * Windows: [http://www.ampps.com/](http://www.ampps.com/)
	 * NOTE: install AMPPS into a location that does not have spaces, ie. NOT `Program Files` -- I recommend that it be installed directly under C:\ , i.e. as C:\Ampps\ .

Ignore any optional steps - the goal is just to get PHP pages to display.

Note that on Windows, it is probable that port 80 will be occupied preventing PHP to run. To fix the problem,      
run the command terminal as administrator (by right-clicking on cmd.exe, and selecting Run as Administrator), and enter the following line :

        $ net stop http

This will terminate unimportant Windows processes using port 80.  (Mind that Skype may also use port 80 
unless you change your Skype settings.)

Now, navigate to the directory from which your HTML and PHP files are being served (in Linux for example, 
this would be `/var/www` or `/var/www/html`, for AMPPS on Windows, this would be `C:\path\to\Ampps\www\`, i.e. the installation path of AMPPS) and install Node.js ([https://nodejs.org/en/](https://nodejs.org/en/)). Install version 1.29 (not the most recent version, but version 1.29) of
Cesium.js ([http://cesiumjs.org/downloads.html](http://cesiumjs.org/downloads.html)) to an **entirely** new subdirectory
here (i.e. create a new subdirectory before installing Cesium.js v1.29 into that subdirectory) - you'll need it later. For Ubuntu, and Linux in general, Node-v6.11.2 is required. It is very easy to
install through the following commands :

        $ sudo apt-get install curl
        $ curl -sL https://deb.nodesource.com/setup_6.x | sudo bash -
        $ sudo apt-get install nodejs

Back in your `/www/` (i.e., `/var/www/` or `/var/www/html/` or `C:\Ampps\www\`, etc etc)
directory, download the package repository at
[https://github.com/ProjectALTAIR/AIFCOMSSwithCUPredictorTest](https://github.com/ProjectALTAIR/AIFCOMSSwithCUPredictorTest)
so that all the project files are located at `/www/AIFCOMSSwithCUPredictorTest/`  (and **note that the directory should be
called** `AIFCOMSSwithCUPredictorTest`, ***not*** `AIFCOMSSwithCUPredictorTest-master`). Copy all of the files
***except server.js and package.json*** from your Cesium directory into
`/www/AIFCOMSSwithCUPredictorTest/predict`, leaving the original `server.js` and `package.json` files
from this GitHub AIFCOMSSwithCUPredictorTest repository intact.

There are a few paths that need to be changed at this point:

 * In `/www/AIFCOMSSwithCUPredictorTest/predict/includes/php_variables.php`, change `ROOT_DIR` to the location of the project folder (e.g., `/www/AIFCOMSSwithCUPredictorTest/` or `C:\\Ampps\\www\\AIFCOMSSwithCUPredictorTest\\`). **Make sure the path ends with a `/` or `\\` (the latter on Windows)**.  Also, change `PYTHON_PATH` to the location of your Python executable (eg., `/usr/bin/python` on Linux or `C:\\Python27\\python.exe` on Windows). **On Windows, make sure you have *double* backslashes in the filenames, to prevent the backslashes from being interpreted as escape characters.**
 * In `/www/AIFCOMSSwithCUPredictorTest/predict/py_variables.py`, change `ROOT_DIR` to the same value as you set it in the above (`php_variables.php`) file.  
 * And in `/www/AIFCOMSSwithCUPredictorTest/predict/js_variables.js`, change exports.ROOT_DIR to be the same value, but with the addition of predict/ at the end, i.e. `/www/AIFCOMSSwithCUPredictorTest/predict/` or `C:\\Ampps\\www\\AIFCOMSSwithCUPredictorTest\\predict\\`.

and one that might possibly need to be (depending on your setup):

 * In `/www/AIFCOMSSwithCUPredictorTest/predict/AIFCOMSS.html`, change the path of `AIFpred.php` to the correct location, <i>if</i> the correct location happens to be different than `http://localhost/AIFCOMSSwithCUPredictorTest/predict/AIFpred.php` in your setup.

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
 * `/var/www/AIFCOMSSwithCUPredictorTest/cron/purge-predictions-cronjob.sh`

Furthermore, the `/var/www/AIFCOMSSwithCUPredictorTest/predict/preds` and `/var/www/AIFCOMSSwithCUPredictorTest/gfs` 
directories must be given full (rwx) access by the PHP interpreter and both `predict.py` files. The simplest way to do so is 
using `sudo chmod a+rwx [directory]`, although safety-conscious users may want to be more selective with their permissions.

Navigate back to `/var/www/AIFCOMSSwithCUPredictorTest/` and run `sudo -H pip install -r requirements_linux.txt` to install some of the 
Python requirements. Change directories to `/var/www/AIFCOMSSwithCUPredictor/predict/` and run `npm update` to install the 
remaining requirements.

In this same `predict` directory, run `node server.js`.  Open a web browser, and navigate to `http://localhost:8080/AIFCOMSS.html`.

You should then have AIFCOMSS running in your web browser.

#### Windows (tested on Windows 7)

Windows has no built in C compiler or interpreter.

Thus, Cygwin (or some other analogous environment -- if you, for example, prefer VirtualBox to Cygwin, then see the VirtualBox 
instructions starting about 40 lines below) is required to compile some of the source files - download it at 
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

to install the remaining requirements.

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

#### Some debugging hints if you need them (Linux)

Run your server and use the developer console of your browser to find any display or protocol errors. Identify the code 
language at the source of the error and use its log file to solve the problem.

Log files for the different code languages used can be found here :

 * JavaScript : Developer console of the server
 * PHP : /var/log/apache2/error.log
 * Python : /var/www/AIFCOMSSwithCUPredictorTest/predict/pred/[the folder corresponding to your run]


