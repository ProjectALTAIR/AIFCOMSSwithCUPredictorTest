/**************************************************************************/
/*!
    @file     DragCalcMethods.hh
    @author1  Justin Albert (jalbert@uvic.ca)
    @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
    @author3  Aravind Shaj  (aravindshaj@gmail.com)
    @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
    @license  GPL

    This is the class containing drag calculation methods to determine the
    magnitude (in Newtons) of the drag force acting on the ALTAIR
    payload at a current instant, in terms of variables such as: 1) ALTAIR's
    current elevation above mean sea level, 2) balloon volume, pressure, and 
    internal temp, and 3) current temperature and density.

    @section  HISTORY

    v1.0  - First version
*/
/**************************************************************************/

#ifndef __DRAGCALCMETHODS_HH__
#define __DRAGCALCMETHODS_HH__

//              ---------------------
//              -- Class Interface --
//              ---------------------

class DragCalcMethods {

    public:

        static float getBalloonDrag(     float balloonSpeed    );  // get the drag due to the balloon

        static float getTerminalSpeed(   float propThrust      );  // get the terminal speed (in m/s) for a given propeller thrust (in
	                                                           // Newtons), assuming effectively all the drag is due to the balloon
        static float getThrustFromSpeed( float terminalSpeed   );  // the inverse of the function above: get propeller thrust (in
                                                                   // Newtons) from a given terminal speed (in m/s)
        static float getThrustFromPower( float propulsivePower );  // get propeller thrust (in Newtons) from a given propulsive
                                                                   // power (in Watts, assuming no wasted power -- i.e., multiply by efficiency)

        static float getParafoilDrag(    float parafoilSpeed   );  // get the drag due to the parafoil

        static float getGondolaDrag(     float gondolaSpeed    );  // currently only returns a dummy value for drag on the ALTAIR gondola

        static float getApproxEffSphericalDragCoef(            );  // return the approximate effective drag coefficient 
                                                                   // k_d = sqrt(pi * c_d / (2 * g)) for a sphere (where c_d = 0.47).

        static float getApproxEffRectangDragCoef(              );  // return the approximate effective drag coefficient 
                                                                   // k_d = sqrt(c_d / (2 * g)) for a rectangle (where c_d = 1.28).

    private:

        DragCalcMethods() {} 			                   // constructor should never be used
};



#endif // __DRAGCALCMETHODS_HH__

