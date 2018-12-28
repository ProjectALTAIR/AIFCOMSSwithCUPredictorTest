/**************************************************************************/
/*!
    @file     AscentAndBurstCalcMethods.hh
    @author1  Justin Albert (jalbert@uvic.ca)
    @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
    @author3  Aravind Shaj  (aravindshaj@gmail.com)
    @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
    @license  GPL

    This is the class containing methods to 1) calculate the equilibrium 
    ascent rate (with respect to stationary winds, i.e. this ascent rate 
    should be added to any updrafting winds to get the resulting predicted 
    equilibrium ascent rate) of the balloon and gondola; and to 2) calculate
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

        static float getEquilibAscentRate();		// get the equilibrium ascent rate (with respect to stationary winds), in m/s
	
        static float getBurstAltitude();		// get the estimated burst altitude of the balloon, in meters

    private:

        AscentAndBurstCalcMethods() {} 			// constructor should never be used
};



#endif // __ASCENTANDBURSTCALCMETHODS_HH__

