/**************************************************************************/
/*!
    @file     HeliumLossCalcMethods.hh
    @author1  Justin Albert (jalbert@uvic.ca)
    @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
    @author3  Aravind Shaj  (aravindshaj@gmail.com)
    @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
    @license  GPL

    This is the class containing methods to 1) calculate the estimated 
    equilibrium rate of helium loss through the skin of the balloon; and 
    to 2) calculate the estimated rate of helium loss through the helium
    bleed valve that is located at the bottom of the balloon (when the 
    valve is either open or closed).

    @section  HISTORY

    v1.0  - First version
*/
/**************************************************************************/

#ifndef __HELIUMLOSSCALCMETHODS_HH__
#define __HELIUMLOSSCALCMETHODS_HH__

//              ---------------------
//              -- Class Interface --
//              ---------------------

class HeliumLossCalcMethods {

    public:

        static float getHeLossRateThruBalSkin();	// Get the equilibrium helium loss rate through the skin of the balloon,
                                                        // in kg/s.  (This should always return a _very_ tiny number.)
	
        static float getHeLossRateThruValve();		// Get the equilibrium helium loss rate through the helium bleed valve
                                                        // located at the bottom of the balloon, in kg/s.  (This should return a
                                                        // small number when the valve is open, and an extremely tiny number when
                                                        // the valve is closed.)
    private:

        HeliumLossCalcMethods() {} 			// constructor should never be used
};



#endif // __HELIUMLOSSCALCMETHODS_HH__

