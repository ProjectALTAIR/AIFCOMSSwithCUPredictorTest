# AIFCOMSS with [CUSF](https://github.com/cuspaceflight) Prediction
These instructions are intended to be read from https://github.com/ProjectALTAIR/AIFCOMSSwithCUPredictorTest , on the bottom half of that page.  So please go to [that page](https://github.com/ProjectALTAIR/AIFCOMSSwithCUPredictorTest#readme) instead, if you are reading them from another location.<br>

Windows screenshot (on Chrome v. 67 browser) &nbsp; &nbsp; &nbsp; Mac screenshot (on Safari v. 11 browser)                

<img src="https://github.com/ProjectALTAIR/AIFCOMSSwithCUPredictorTest/raw/master/screenshots/AIFCOMSSScreenshotWindows_9jul18.jpg" data-canonical-src="https://github.com/ProjectALTAIR/AIFCOMSSwithCUPredictorTest/raw/master/screenshots/AIFCOMSSScreenshotWindows_9jul18.jpg" width=336 height=240 /> &nbsp; &nbsp; <img src="https://github.com/ProjectALTAIR/AIFCOMSSwithCUPredictorTest/raw/master/screenshots/AIFCOMSSScreenshotMac_24jun18.jpg" data-canonical-src="https://github.com/ProjectALTAIR/AIFCOMSSwithCUPredictorTest/raw/master/screenshots/AIFCOMSSScreenshotMac_24jun18.jpg" width=336 height=240 /><br><br>

<details>
	<summary>## Installation instructions and debugging hints for <b><i>Linux</i></b> machines</summary>
        <br>
	Linux instructions and debugging hints will be moved/copied here.
</details>
<details>
	<summary>Installation instructions and debugging hints for <b><i>MacOS</i></b> machines</summary>
	Mac instructions and debugging hints will be moved/copied here.
</details>
<details>
	<summary>Installation instructions and debugging hints for <b><i>Windows</i></b> machines</summary>
	Windows instructions and debugging hints will be moved/copied here.
</details>

## Installation

After first [completely uninstalling any other version of Anaconda](https://docs.anaconda.com/free/anaconda/install/uninstall/) or of [Python](https://www.wikihow.com/Uninstall-Python) that you might possibly already have on your 
Mac, Linux, or Windows computer, please install the Python 3.11 version of [Anaconda](https://www.anaconda.com/products/individual), using
the installer that can be found near the bottom of [this page](https://www.anaconda.com/products/individual).  (If you are on a Mac, use 
the command-line version of the Anaconda installer.  If you are on Windows, make sure to do both this Anaconda installation, and EVERY step in these instructions, when logged into an account with Administrator privileges on your machine -- don't do anything anywhere in these instructions as a non-Adminstrator user when on Windows.  However, if on MacOS or Linux, please do the opposite: do absolutely **nothing** via sudo or otherwise with superuser priviliges unless you absolutely have to, i.e. if there is absolutely no other way to install or perform the given action on your machine without sudo.  On *Windows*: please install Anaconda into a location that does not have spaces, i.e. NOT under `Program Files` -- I recommend that it be installed directly under C:\ )  [Installation instructions for the Anaconda installer can be found here.](https://docs.anaconda.com/anaconda/install/)
If asked during the installation, make sure to add Anaconda3 to your PATH environment variable (even though it might say that that option is
not recommended), as well as registering Anaconda3 as your default Python 3.11 (if that is asked as well).  Once you have that installed, you
should then have Python 3.11 installed on your computer -- it is part of the Anaconda installation.  After you successfully install the
Python 3.11 version of Anaconda on your machine, you should reboot your computer (so that Anaconda's settings to make it the default location
for your Python application, etc., can take effect).<br>

Then, also on <i>any</i> platform (Mac, Linux, or Windows), and also following the reboot mentioned above, 
this package <i>also</i> requires the installation of an 
AMP (Apache/MySQL/PHP) stack. Instructions can be found online for:

 * Ubuntu Linux: [https://www.unixmen.com/how-to-install-lamp-stack-on-ubuntu-16-04/](https://www.unixmen.com/how-to-install-lamp-stack-on-ubuntu-16-04/)
 * Mac OS: Instructions for setting up an AMP stack in High Sierra, i.e. Mac OS 10.13, can be found (for example) [here](https://websitebeaver.com/set-up-localhost-on-macos-high-sierra-apache-mysql-and-php-7-with-sslhttps).  Updated guides for Mojave, i.e. Mac OS 10.14.x, can be found (for example) [here](https://coolestguidesontheplanet.com/install-apache-mysql-php-on-macos-mojave-10-14/); for Catalina, i.e. Mac OS 10.15.x [here](https://tech-cookbook.com/2019/10/07/setting-up-your-local-server-on-macos-catalina-2019-mamp/) and [here](https://medium.com/@firstsquares/macos-catalina-set-up-localhost-e4a0b5d5be33); and for Monterey, i.e. Mac OS 12.x.y, or higher [here (for PHP & Apache](https://getgrav.org/blog/macos-monterey-apache-multiple-php-versions) and [here (for MySQL)](https://tech-cookbook.com/2021/10/25/how-to-setup-mamp-macos-apache-mysql-php-on-macos-12-monterey-2021/) (but you must *definitely* note the following for Mac OS Monterey!!!).  Note that all versions of Mac OS X ⚠️**prior to Monterey, i.e. prior to MacOS 12.x.y** ⚠️, already come with both Apache and PHP installed (you just need to turn them on, and if you have a pre-High Sierra OS version, i.e. pre- Mac OS 10.13, then you'll need to upgrade your PHP from v.5 to v.7.1, but you won't need to install either Apache or PHP ab initio).  ⚠️However, starting with MacOS Monterey (12.x.y), MacOS does **not** come with PHP (although it does come with Apache), and you will need to install PHP if you have (or have upgraded to) MacOS >=12.x.y ⚠️ -- to install PHP on Monterey or higher, see [here](https://getgrav.org/blog/macos-monterey-apache-multiple-php-versions) however there are, unfortunately, ⚠️multiple necessary [caveats](https://github.com/ProjectALTAIR/AIFCOMSSwithCUPredictorTest/blob/master/README_CaveatsInstallingPHPonMacOSMonterey.md) to that page ⚠️ that you will need to know if installing PHP on MacOS >= 12.x.y Monterey. As mentioned in the above instructions, though, you will, however, ***always*** need to install MySQL.  You can simply google `mac apache` for instructions on setting the AMP stack up in OS X versions that are earlier than 10.13 (if you're running with such an ancient Mac OS version).  And (as mentioned below) ignore any optional steps - the goal is just to get PHP pages to display.  But, per all of these Mac Apache directions ⚠️**prior to Monterey**⚠️, **do certainly be sure to modify your /etc/apache2/httpd.conf file to make sure that DocumentRoot is pointing to your "/Users/YOUR_USERNAME/Sites/" directory and not the default "/Library/WebServer/Documents" directory, though** (and that may need to be renewed each time you update the OS -- for example when updating from High Sierra to Mojave, Apache gets re-installed and the httpd.conf file goes back to its default).  (The Monterey analogue of the warning in the previous sentence is provided in the [Monterey PHP & Apache instructions](https://getgrav.org/blog/macos-monterey-apache-multiple-php-versions) and their [caveats](https://github.com/ProjectALTAIR/AIFCOMSSwithCUPredictorTest/blob/master/README_CaveatsInstallingPHPonMacOSMonterey.md) mentioned previously.)
 * Windows: [http://www.ampps.com/](http://www.ampps.com/)
	 * NOTE: install AMPPS into a location that does not have spaces, ie. NOT `Program Files` -- I recommend that it be installed directly under C:\ , i.e. as C:\Ampps\ .

Ignore any optional steps - the goal is just to get PHP pages to display.

Note that on Windows, it is probable that port 80 will be occupied, preventing PHP from running. To fix the problem, run the command terminal as administrator (by right-clicking on cmd.exe, and selecting Run as Administrator), and enter the following line :

        $ net stop http

This will terminate unimportant Windows processes using port 80.  (Be aware that Skype may also use port 80, unless you change your Skype settings.)

Now, install Node.js ([https://nodejs.org/en/](https://nodejs.org/en/)).  Use the default Node.js installation, of the version of Node.js that is "Recommended for most users" on that nodejs.org site, but making sure to add Node.js executables to your PATH if asked during the installation.  (One might need to add the Node.js executables to one's PATH manually at some point afterwards, since installation might not ask if this is wanted, and also might not do it automatically -- but if so, you will find out later that having these executables in your PATH will be needed when the system fails to find them, but that's OK, you can just add them to your PATH at that point.)

Then, navigate to the directory from which your HTML and PHP files are being served (in Linux for example, 
this would be `/var/www/` or `/var/www/html/`; for AMPPS on Windows, this would be `C:\path\to\Ampps\www\`, i.e. the installation path of AMPPS; and for Mac OS, this would likely be `/Users/<your user name>/Sites/`) and install version 1.114 (not necessarily the most recent version, but version 1.114) of CesiumJS ([https://github.com/CesiumGS/cesium/releases/download/1.114/Cesium-1.114.zip](https://github.com/CesiumGS/cesium/releases/download/1.114/Cesium-1.114.zip), which is likely _not_ the most recent version in https://cesium.com/downloads/) to an **entirely** new subdirectory here (i.e. create a new subdirectory before installing CesiumJS v1.114 into that subdirectory) - you'll need it later. 

<!--- For Ubuntu, and Linux in general, Node-v6.11.2 is required. It is very easy to install through the following commands :
 
        $ sudo apt-get install curl
        $ curl -sL https://deb.nodesource.com/setup_6.x | sudo bash -
        $ sudo apt-get install nodejs -->

Back in your `/<www>/` (i.e., `/var/www/` or `/var/www/html/` or `C:\Ampps\www\` or `/Users/<your user name>/Sites/`, etc etc) directory, download the package repository at
[https://github.com/ProjectALTAIR/AIFCOMSSwithCUPredictorTest](https://github.com/ProjectALTAIR/AIFCOMSSwithCUPredictorTest)
so that all the project files are located at `/<www>/AIFCOMSSwithCUPredictorTest/`  (and **note that your directory should be called** `AIFCOMSSwithCUPredictorTest`, ***not*** `AIFCOMSSwithCUPredictorTest-master` -- if it is the latter, just rename it to be the former). Copy all of the files and folders 
***except server.js and package.json*** from your Cesium directory into
`/<www>/AIFCOMSSwithCUPredictorTest/predict`, leaving the original `server.js` and `package.json` files
from this GitHub AIFCOMSSwithCUPredictorTest repository intact.  (A few other files will show up as duplicates;
users should replace those other existing files in `/<www>/AIFCOMSSwithCUPredictorTest/predict` with the new files from your Cesium directory, rather than just ignoring those files.)  Be sure to put those Cesium files and folders **directly** into your `/<www>/AIFCOMSSwithCUPredictorTest/predict/` directory, e.g. after doing that you should have the subdirectories `/predict/Build/`, `/predict/Apps/`, `/predict/ThirdParty/`, and files including `/predict/LICENCE.md`, etc.; rather than putting those Cesium files and folders all into a subdirectory of their own.

There are a few paths that need to be changed at this point:

 * In `/<www>/AIFCOMSSwithCUPredictorTest/predict/includes/php_variables.php`, change `ROOT_DIR` to the location of the project folder (e.g., `/var/www/html/AIFCOMSSwithCUPredictorTest/` or `C:\\Ampps\\www\\AIFCOMSSwithCUPredictorTest\\` etc). **Make sure the path ends with a `/` or `\\` (the latter on Windows)**.  Also, change `PYTHON_PATH` to the location of your anaconda3 Python executable (e.g., `/Users/<your username>/anaconda3/bin/python` on a Mac). **On Windows, make sure you have *double* backslashes in the filenames, to prevent the backslashes from being interpreted as escape characters.  Note that *some* Windows versions (for example *some* installations of Windows 10) can autocorrect file path names, so in that case it is slightly less important to ensure that you always have double backslashes, however in all cases it is best to not rely on autocorrection, and so you should always put double backslashes within the filenames inside these files on Windows PCs.  You can change the paths inside these files on Windows by opening the files in WordPad, for example.**
 * In `/<www>/AIFCOMSSwithCUPredictorTest/predict/py_variables.py`, change `ROOT_DIR` to the same value as you set it in the above (`php_variables.php`) file.  
 * And in `/<www>/AIFCOMSSwithCUPredictorTest/predict/js_variables.js`, change `ROOT_DIR` to be the same value, but with the addition of predict/ at the end, i.e. `/var/www/html/AIFCOMSSwithCUPredictorTest/predict/` or `C:\\Ampps\\www\\AIFCOMSSwithCUPredictorTest\\predict\\` etc.  **On Windows, if you use single backslashes instead of double backslashes inside your `C:\Ampps\www\AIFCOMSSwithCUPredictorTest\predict\js_variables.js` file, and you end up with files named, e.g., AmppswwwAIFCOMSSwithCUPredictorTestpredictaltairdata.txt and AmppswwwAIFCOMSSwithCUPredictorTestpredictaltairpos.txt inside your AIFCOMSSwithCUPredictorTest\predict directory after you complete all these instructions and run AIFCOMSS, then that's a sure sign that you need double backslashes within your definition of ROOT_DIR inside your `C:\Ampps\www\AIFCOMSSwithCUPredictorTest\predict\js_variables.js` file instead**.

and one that might possibly need to be (depending on your setup):

 * In `/<www>/AIFCOMSSwithCUPredictorTest/predict/AIFCOMSS.html`, change the path of `AIFpred.php` to the correct location, <i>if</i> the correct location happens to be different than `http://localhost/AIFCOMSSwithCUPredictorTest/predict/AIFpred.php` in your setup.

The next steps depend heavily on the operating system.<br><br>



### <ins>Linux</ins> (tested on Mint 21.2 and Mint 18.2)
	
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
 * `/var/www/AIFCOMSSwithCUPredictorTest/pred_src/pred`
 * `/var/www/AIFCOMSSwithCUPredictorTest/pred_src/pred_ALTAIR`
 * `/var/www/AIFCOMSSwithCUPredictorTest/pred_src/pred_StationKeep`
 * `/var/www/AIFCOMSSwithCUPredictorTest/cron/clear-pydap-cache-cronjob.sh`
 * `/var/www/AIFCOMSSwithCUPredictorTest/cron/prune-predictions-cronjob.sh`

Furthermore, the `/var/www/AIFCOMSSwithCUPredictorTest/predict/preds` and `/var/www/AIFCOMSSwithCUPredictorTest/gfs` 
directories must be given full (rwx) access by the PHP interpreter and both `predict.py` files. The simplest way to do so is 
using `sudo chmod a+rwx [directory]`, although safety-conscious users may want to be more selective with their permissions.

***If*** you have SELinux (Security-Enhanced Linux) enabled, you will need to do this [couple of extra steps](https://github.com/ProjectALTAIR/AIFCOMSSwithCUPredictorTest/blob/master/README_CaveatsInstallingOnSELinux.md).

Navigate back to `/var/www/AIFCOMSSwithCUPredictorTest/` and run `pip install -r requirements_python3.11.txt` to install the 
Python requirements. Change directories to `/var/www/AIFCOMSSwithCUPredictorTest/predict/` and run 

    npm install

then

    npm update

to install the remaining requirements.

Then, within this same `predict` directory, create the symbolic link files `altairpos.txt` and `altairdata.txt` via the commands

    ln -s /tmp/altairpos.txt .
    ln -s /tmp/altairdata.txt .

Then, in this same `predict` directory, run `node server.js`.  Open a web browser, and navigate to `http://localhost:8080/AIFCOMSS.html`.

You should then have AIFCOMSS running in your web browser.<br><br>



### <ins>Mac OS X</ins> (tested on v. 14.3 "Sonoma", on v. 12.6 "Catalina", and on earlier MacOS versions)

Check that you have CMake installed on your Mac by typing `which cmake` in a terminal window from your
`/Users/<your user name>/Sites/AIFCOMSSwithCUPredictorTest/` directory.  If you don't have it, then download the .dmg
Mac installer for it from https://cmake.org/download/ , and then install it, following all instructions.  After that,
`which cmake` should find your installed CMake executable in `/usr/local/bin/cmake`.  (But if `which cmake` from your
`/Users/<your user name>/Sites/AIFCOMSSwithCUPredictorTest/` directory **still** fails to find cmake after that, then try
the command `sudo "/Applications/CMake.app/Contents/bin/cmake-gui" --install` and then hopefully after that, `which cmake`
will successfully find the cmake command.)

Then, navigate to `/Users/<your user name>/Sites/AIFCOMSSwithCUPredictorTest/pred_src` and run

    $ cmake .
    $ make

to compile the code found there.  (If you happen to encounter errors such as "Could not find pkgConfig" and
"Could not find package glib-2.0" after running `make` as above, then try downloading and installing [Homebrew](https://coolestguidesontheplanet.com/installing-homebrew-on-macos-sierra-package-manager-for-unix-apps/), 
and then running `brew install glib` \[and `brew install pkgconfig` if you need that too\], before trying `cmake .` and `make` again.)

Once compiled, the following files must be made executable (`sudo chmod +x [file]`):

 * `/Users/<your user name>/Sites/AIFCOMSSwithCUPredictorTest/predict.py`
 * `/Users/<your user name>/Sites/AIFCOMSSwithCUPredictorTest/pred_src/pred`
 * `/Users/<your user name>/Sites/AIFCOMSSwithCUPredictorTest/pred_src/pred_ALTAIR`
 * `/Users/<your user name>/Sites/AIFCOMSSwithCUPredictorTest/pred_src/pred_StationKeep`
 * `/Users/<your user name>/Sites/AIFCOMSSwithCUPredictorTest/cron/clear-pydap-cache-cronjob.sh`
 * `/Users/<your user name>/Sites/AIFCOMSSwithCUPredictorTest/cron/prune-predictions-cronjob.sh`

Furthermore, the `/Users/<your user name>/Sites/AIFCOMSSwithCUPredictorTest/predict/preds` and `/Users/<your user name>/Sites/AIFCOMSSwithCUPredictorTest/gfs` 
directories must be given full (rwx) access by the PHP interpreter and the `predict.py` file. The simplest way to do so is 
using `sudo chmod a+rwx [directory]`, although safety-conscious users may want to be more selective with their permissions.

Navigate back to `/Users/<your user name>/Sites/AIFCOMSSwithCUPredictorTest/` and run `pip install -r requirements_python3.11.txt` to install some of the Python requirements. Change directories to `/Users/<your user name>/Sites/AIFCOMSSwithCUPredictorTest/predict/` and run 

    npm install

then

    npm update
 
to install the remaining requirements.

Then, within this same `predict` directory, create the symbolic link files `altairpos.txt` and `altairdata.txt` via the commands

    ln -s /tmp/altairpos.txt .
    ln -s /tmp/altairdata.txt .

Then, in this same `predict` directory, run `node server.js`.  Open a web browser, and navigate to `http://localhost:8080/AIFCOMSS.html`.

You should then have AIFCOMSS running in your web browser.<br><br>



### <ins>Windows</ins> (tested on Windows 10 and Windows 7)

Windows has no built in C compiler or interpreter.

Thus, Cygwin (or some other analogous environment: if you, for example, prefer VirtualBox to Cygwin, then see the VirtualBox 
instructions starting about 40 lines below.  Visual Studio \[[https://www.visualstudio.com](https://www.visualstudio.com)]
would be a third option, if you so choose.  Note that if you choose to install Visual Studio \[rather than Cygwin or VirtualBox\] on
your Windows machine, then after you install Visual Studio, all commands below must be run in powershell, rather
than in the standard Windows command - you can access powershell from the standard Windows command line by
typing: `$ powershell`, and note that it is also necessary to run the command terminal with Administrator privileges) is required to compile some of the source files - download Cygwin at 
[https://www.cygwin.com](https://www.cygwin.com). Run the setup, choosing the default installation options -- but in addition to the default packages also install:

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

to build the source files, and then close the Cygwin terminal.  (Don't worry about the warnings you get when you do cmake . or make, but if you constantly get an error involving glib-2.0, FindPkgConfig.cmake, and/or pkg_check_modules when doing cmake . that prevents you from then doing make and building the pred executables, then please see the debugging hints at the bottom of this page.)  Copy the following files from the `bin` folder in Cygwin's 
installation directory to the `pred_src` directory containing `pred.exe`:

 -  `cygglib-2.0-0.dll`
 -  `cygiconv-2.dll`
 -  `cygintl-8.dll`
 -  `cygpcre-1.dll`
 -  `cygwin1.dll`
 -  `cygstdc++-6.dll`
 -  `cyggcc_s-seh-1.dll`

When you installed Cygwin above, it probably installed python on your system, and then also (unfortunately) made that into the default python on your system (by putting the Cygwin python executables at the head of your Windows PATH environment variable.)  You will need to remove the Cygwin-installed python from your PATH, so that the Python 3.11 executables (including pip, which you will be using right below) from your **Anaconda installation** will again be your default Python executables.  To do this on Windows 10, follow the instructions shown [here](https://www.architectryan.com/2018/03/17/add-to-the-path-on-windows-10/), but instead of adding the python executable directories to your PATH, you will be **deleting** the Cygwin-installed python executable directories from your system PATH.  (Don't worry, this doesn't delete the actual Cygwin-installed python executable files themselves -- it just makes them non-default.) Fortunately, the Cygwin-installed python executable directories will be obvious within the system PATH.  Delete these directories from your system PATH, and then choose OK.

Then, open a new Cygwin terminal window, navigate to the `AIFCOMSSwithCUPredictorTest` directory, make absolutely sure that you will be getting the Anaconda-installed Python 3.11 version of pip by typing

    which pip

and then (if the result of the above looks good -- i.e. there's an anaconda in the directory name), run

    pip install -r requirements_python3.11.txt

to install the Python requirements.  (Note that you might need to use the full path to the location of your pip.exe executable.
That above pip installation also assumes that you have [git](https://git-scm.com/download/win) already installed on your computer;
if you don't, then you might need to also install git, and then after that also make sure that the git executable is in your PATH.)

Then, navigate to the `AIFCOMSSwithCUPredictorTest/predict` directory and run

    npm install

then

    npm update

to install the remaining requirements.  (Note that if `npm install` does not work for you within a command terminal, please instead try it within either a cygwin terminal, or powershell, from the `AIFCOMSSwithCUPredictorTest/predict` directory as above.)  

Then, while still in that same `C:\Ampps\www\AIFCOMSSwithCUPredictorTest\predict\` directory, note that the two files `altairpos.txt` and `altairdata.txt` in that directory should be "symbolic links" to the files `C:\tmp\altairpos.txt`  and `C:\tmp\altairdata.txt` -- rather than their just containing the text /tmp/altairpos.txt and /tmp/altairdata.txt.  (This AIFCOMSSwithCUPredictorTest package repository in Github should have automatically created those symbolic links when you downloaded the AIFCOMSSwithCUPredictorTest package, however symbolic link creation from Github package downloads appears to be unavoidably broken on Windows.)  If those two files just contain text, rather than being symbolic links, then you'll need to erase those two files `altairpos.txt` and `altairdata.txt`, and then create those two symbolic links manually -- you can follow the instructions at https://www.howtogeek.com/16226/complete-guide-to-symbolic-links-symlinks-on-windows-or-linux/ if you have not created symbolic links in Windows before.  (BTW, those two links should be _soft_ symbolic links, also known as softlinks.  Unfortunately, doing `ln -s` within a Cygwin window will **not** create properly-functioning corresponding softlinks on Windows, even though it will at first look like it has created reasonable-looking corresponding link files -- they **won't** actually work properly, one **has** to instead use `mklink` as described in those instructions.)  You could first create the two files `C:\tmp\altairpos.txt`  and `C:\tmp\altairdata.txt`, to which the symbolic links in `C:\Ampps\www\AIFCOMSSwithCUPredictorTest\predict\altairpos.txt` and `C:\Ampps\www\AIFCOMSSwithCUPredictorTest\predict\altairdata.txt` should point, by (within a Cygwin terminal) trying the two commands:

touch /cygdrive/c/tmp/altairpos.txt

touch /cygdrive/c/tmp/altairdata.txt

which should create empty files in `C:\tmp\altairpos.txt` and `C:\tmp\altairdata.txt` -- and then, after that, creating the symbolic links (using `mklink` within an Administrator command prompt) to those files mentioned above.  (If you happen to do `mklink` of these two files first, then you probably will not need to do the above `touch` commands -- but don't worry, you might just get a harmless error message if so.)

Then, make sure that your `C:\Ampps\www\AIFCOMSSwithCUPredictorTest\predict\` directory, your `C:\Ampps\www\AIFCOMSSwithCUPredictorTest\predict\preds\` directory, your `C:\Ampps\www\AIFCOMSSwithCUPredictorTest\gfs\`, and your `C:\tmp\` directory (and everything in each of them) all have full control and read, write, and execute access by the system, and also by system administrators and yourself, and that your `C:\Ampps\www\AIFCOMSSwithCUPredictorTest\pred_src\` directory (and everything in it) has (at very least) execute access by the system and by yourself.  You could consult this guide to changing Windows file and folder permissions if you are new to changing permissions on Windows: https://www.online-tech-tips.com/computer-tips/set-file-folder-permissions-windows/.  (Note that changing permissions **must** be done via the Windows file permission system \[which uses Windows access control lists -- ACLs\] -- it **cannot** be done using chmod commands within Cygwin.  Attempting to change Windows file or folder permissions using chmod commands within a Cygwin terminal will **completely mess up** the actual Windows ACL permissions of those files and folders -- so DON'T EVEN TRY THAT, and if you do, you will most definitely end up REGRETTING it!)

Then, launch Ampps, and in your web browser navigate to `localhost/ampps`. Click on Apache Configuration, turn `mod_headers` on, and 
restart Apache.

In a new terminal, navigate to the `AIFCOMSSwithCUPredictorTest/predict` directory, and run

    node server.js

Open your web browser, and navigate to `http://localhost:8080/AIFCOMSS.html`. You should then have AIFCOMSS running in your browser.  

***IF*** the bottom frame (the p5.js window) always just says "Loading ..." (that problem can often occur, especially on some Windows systems), then modify line 6 of `predict\ALTAIR_entities_p5frame.js` to change the value of the
audibleAlarms variable on that line from `true` to `false`, and then re-run (and let me know if you have any problems).

Also, ***IF*** you get the error "This process does not have permission to listen on port 8080" when you run node server.js, then in that case please try the suggestion on this page: https://medium.com/@myolisi/how-to-fix-error-eacces-permission-denied-8080-in-nodejs-66d6cd5b2ed5, i.e. find the other process that is blocking your port 8080 using "netstat -aon | findstr 8080" and then kill it using "Stop-Process -ID <process_ID> -Force" (and let me know if you have any problems with that).

And ***IF*** you are not getting a blue flight path prediction curve on the map in the upper left browser frame, make sure that Ampps is running (and if it is, try restarting it), and then check your `AIFCOMSSwithCUPredictorTest/predict/preds/` folder for new predictor output subdirectories.  Within the new predictor output subdirectory (which will have a long gibberish-like hexadecimal name), look in the `py_log` file for the main output of the process.  Then look at the "Some debugging hints ..." below (especially the one at the very bottom) for common problems. 


<i>If you prefer VirtualBox to Cygwin</i>:

Install VirtualBox [https://www.virtualbox.org/wiki/Downloads](https://www.virtualbox.org/wiki/Downloads) and create a virtual 
machine. I would recommend 2GB of RAM and 25 GiO of memory allocated to your virtual machine. Once your virtual machine is 
created, download Ubuntu [https://www.ubuntu.com/download/desktop](https://www.ubuntu.com/download/desktop) then run it as a 
virtual optical disk on your virtual machine, install it, and you're done! General instructions can be found here : 
[https://linus.nci.nih.gov/bdge/installUbuntu.html](https://linus.nci.nih.gov/bdge/installUbuntu.html)

Then follow the upper instructions concerning Linux installation (though be sure to note that the port 80 problem described above remains, as your 
connection is provided by a Windows machine).<br><br>



### <ins>Some debugging hints if you need them</ins> (all OS's)

Run your server, and use the developer Javascript console of your browser to find any display or protocol errors. Identify the code language at the source of the error, and the use its log file -- see below for location -- to solve the problem.

The log files for the different code languages used can be found in the locations below :

 * Javascript : The developer Javascript console of the server (you can find this in your web browser).
 * PHP : /var/log/apache2/error.log (on Linux)
 * Python : /var/www/AIFCOMSSwithCUPredictorTest/predict/preds/[the folder corresponding to your run] (on Linux)

<b>If you are not getting any flight path prediction whatsoever</b> (which can sometimes happen on any of MacOS, Linux, or Windows -- for example, the first time you run AIFCOMSS, or after a reboot, etc): On MacOS or Linux, try doing a

    sudo apachectl restart

and then re-running node server.js and AIFCOMSS.  If that doesn't help, then check that going to http://localhost in a web browser (without the :8080, and http rather than https !) is, in fact, taking you to the **parent** directory of your AIFCOMSSwithCUPredictorTest/ directory -- since, especially on MacOS, and especially right after OS upgrades or disk transfers to another computer, your /etc/apache2/httpd.conf file can get completely messed up and reset (see above, near the top of this README file), which puts your DocumentRoot setting in the wrong place (possibly amongst other problems with /etc/apache2/httpd.conf which can occur !) -- if that has happened, then replace your new /etc/apache2/httpd.conf with your previous, good /etc/apache2/httpd.conf file.  

On Windows, try either a) restarting Apache within AMPPS (there is a toggle switch in the AMPSS window for doing that), or b) restarting AMPSS entirely; and then (after having done either a or b), re-running node server.js and AIFCOMSS.  On any one of the three operating systems -- if you haven't run AIFCOMSS in a while (or are running the flight path prediction for the very first time), and are not getting a flight path prediction the first time you run it after your long hiatus
since your last AIFCOMSS run (or, on your first AIFCOMSS flight path prediction run), then try just reloading the browser -- typically you just need that one browser reload to get the flight path prediction going (after it takes its ~20 seconds to determine the flight path prediction, of course).

On Windows, if you constantly get an error like the below when doing cmake . :

    -- Checking for module 'glib-2.0'
    --
    CMake Error at /usr/share/cmake-3.14.5/Modules/FindPkgConfig.cmake:457 (message):
      A required package was not found
    Call Stack (most recent call first):
      /usr/share/cmake-3.14.5/Modules/FindPkgConfig.cmake:642 (_pkg_check_modules_internal)
      CMakeLists.txt:8 (pkg_check_modules)
      
even if you properly installed libglib2.0 and libglib2.0-devel , etc (per the above Windows instructions) when you installed Cygwin, then you should comment out line 8 of the pred_src/CMakeLists.txt file by putting a # symbol in front of line 8 like so:

    # pkg_check_modules(GLIB REQUIRED glib-2.0)
    
and then re-do cmake . and then make.  The above error should then go away, and then your pred executables should get built properly.

On any OS (*especially* just after one upgrades OS versions on one's computer) if, when doing the `make` command within the pred_src directory, you constantly get an error saying that the file glib.h can't be found (or get a similar problem with `make` being unable to find other system include files on your computer), then this problem may be due to CMake having cached the old location of your system include files and libraries, and thus you need to get rid of that old CMake-cached info, and ensure that CMake updates these locations.  Run the command:

    source ./cmakeclean.script
    
and then run `make` again, following the above.  Doing the above command will remake your CMake-cached system file location info, and hopefully fix your problem!

On any OS, if you run into problems with your npm packages that you installed via `npm install` (perhaps multiple times in the past), and attempted to update via `npm update`, then

    npm rebuild
    
will sometimes help a lot.

On Windows, if you are still getting mysterious permissions problems with pred_StationKeep.exe (and/or pred.exe or pred_ALTAIR.exe), even after checking the Windows ACLs of the executable and its pred_src directory, trying to run as Administrator, etc, then try copying ALL of the \*.dll files from the cygwin installation directory into the pred_src directory -- i.e., in a Cygwin terminal, navigating to your pred_src directory, then doing

    make clean
    rm *.dll
    cmake .
    make
    cp /cygdrive/c/cygwin64/bin/*.dll .

and then try running pred_StationKeep.exe (or one of the other pred executables) in a regular command prompt terminal again at that point -- and making sure to do **ALL** of the above when logged in with an Administrator account.  (I have absolutely no idea why the above seems to work when nothing else does, however it certainly did for me at very least once, and then repeatedly worked for me afterward for at very least the rest of that day.  However, it **ONLY** works when one is logged in as an Administrator, so that both the Cygwin terminal AND the command prompt have Administrator privileges.  Otherwise, it doesn't fix the persistent mysterious permissions problems at all.  And failing to do any of the above, even when logged in as an Administrator, doesn't fix anything either.  This appears to me to just be an unidentified flying Windows permissions problem \[UFWPP\] ...)

**UPDATE**: The above Windows 10 permissions problem with pred_StationKeep.exe appears to not in fact be a permissions problem at all, but rather a problem with Windows 10 Exploit Protection settings!!!  In the Start menu, go to Settings, then to Windows Security, then to App & browser control, then to Exploit protection settings, then to the Program settings within Exploit protection settings -- and then add each of pred_StationKeep.exe, pred_ALTAIR.exe, and pred.exe to provide each of those executables with its own custom program settings.  Then, manually remove all default exploit protection settings that have been turned on for those executables (i.e., turn them off), and then save those settings.  That then solves the mysterious "Access is denied." problems with these executables (that occur even when their ACLs allow full control of these executables)!!!
    
