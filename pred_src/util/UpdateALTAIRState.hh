/**************************************************************************/
/*!
    @file     UpdateALTAIRState.hh
    @author1  Justin Albert (jalbert@uvic.ca)
    @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
    @author3  Aravind Shaj  (aravindshaj@gmail.com)
    @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
    @license  GPL

    This is the class containing the static method doUpdate() to perform an
    update of the global altairState variable together with all of its member 
    classes (containing the states of the environment, balloon, gondola, 
    payload, etc) from the most recently telemetered (or simulated) info.

    @section  HISTORY

    v1.0  - First version
*/
/**************************************************************************/

#ifndef __UPDATEALTAIRSTATE_HH__
#define __UPDATEALTAIRSTATE_HH__

//              ---------------------
//              -- Class Interface --
//              ---------------------

class UpdateALTAIRState {

    public:

        static void doUpdate();		// Update the current ALTAIR state, from the telemetered (or simulated) info obtained via 
                                        // the p5.js frame, and having been output to the /tmp/altairdata.txt file via server.js .
                                        // (Note that the altairdata.txt file used to be called altairpos.txt, when that file only 
                                        // contained lat, lon, and ele information.)
    private:

        UpdateALTAIRState() {} 		// constructor should never be used
};



#endif // __UPDATEALTAIRSTATE_HH__

