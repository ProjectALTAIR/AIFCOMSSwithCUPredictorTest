/**************************************************************************/
/*!
    @file     AscentAndBurstCalcMethods.hh
    @author1  Justin Albert (jalbert@uvic.ca)
    @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
    @author3  Aravind Shaj  (aravindshaj@gmail.com)
    @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
    @license  GPL

    This is the class containing methods to 1) calculate the vertical 
    acceleration rate (with respect to stationary winds, i.e. the resulting 
    ascent rate should be added to any updrafting winds to get the true
    predicted ascent rate) of the balloon and gondola; and to 2) calculate
    the estimated burst altitude of the balloon.

    @section  HISTORY

    v1.0  - First version
*/
/**************************************************************************/

#ifndef __ASCENTANDBURSTCALCMETHODS_HH__
#define __ASCENTANDBURSTCALCMETHODS_HH__

//              ---------------------
//              -- Class Interface --
//              ---------------------

class AscentAndBurstCalcMethods {

    public:

        static float getPresentAscentTerminalVelocity();
        static float getPresentDescentTerminalVelocity();

        static float getVerticalAcceleration();		// Calculate the present vertical acceleration, in m/s^2. 
	                                                // Negative values will be returned when acceleration is downward. 
        static bool  areWeAtBurstAltitude();		// Has the balloon stretched out so much that it will burst?

    private:

        AscentAndBurstCalcMethods() {} 			// Constructor should never be used.
};



#endif // __ASCENTANDBURSTCALCMETHODS_HH__

