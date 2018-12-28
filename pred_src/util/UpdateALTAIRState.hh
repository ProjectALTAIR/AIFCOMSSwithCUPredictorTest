/**************************************************************************/
/*!
    @file     UpdateALTAIRState.hh
    @author1  Justin Albert (jalbert@uvic.ca)
    @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
    @author3  Aravind Shaj  (aravindshaj@gmail.com)
    @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
    @license  GPL

    This is the class containing the static methods initializeState() to 
    initialize the global altairState and doUpdate() to perform an
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

        static void initializeState(                                );   // Initialize the current ALTAIR state from telemetered info 
                                                                         // obtained via the p5.js frame, and having been output to 
                                                                         // the /tmp/altairdata.txt file via server.js .

        static void doUpdate(             float   newLat ,               // Update the current ALTAIR state from simulated info 
                                          float   newLon ,               // obtained via the flight path prediction simulation that
                                          float   newAlt            );   // utilizes the current wind and weather data.
    private:

        static void setALTAIRDataElement( size_t  elemID ,
                                          float   altairDataElement );


        UpdateALTAIRState(                                          ) {} // constructor should never be used
};

#endif // __UPDATEALTAIRSTATE_HH__
