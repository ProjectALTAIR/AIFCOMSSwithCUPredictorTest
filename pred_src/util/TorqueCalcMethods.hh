/**************************************************************************/
/*!
    @file     TorqueCalcMethods.hh
    @author1  Justin Albert (jalbert@uvic.ca)
    @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
    @author3  Aravind Shaj  (aravindshaj@gmail.com)
    @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
    @license  GPL

    This is the class containing torque calculation methods to determine the 
    amount (in Newton-meters) of torque acting on the ALTAIR gondola at a 
    current instant, in terms of variables such as: 1) the RPM of each of the 
    4 propellers; 2) the mass of the gondola, and 3) the mass of the balloon
    and its coupling to the gondola.

    Note that the calculation methods for the torque on the gondola along 
    each of the 3 axes of rotation (azimuth, elevation, and sideways-roll)
    are static member functions of this class.  The interface to obtain 
    the instantaneous torque along each of these axes is via these static 
    member functions within this class.

    An example usage:
        #include "TorqueCalcMethods.hh"

        float torqueAzim = TorqueCalcMethods::getTorqueAzim();  // azimuthal torque
        float torqueElev = TorqueCalcMethods::getTorqueElev();  // elevational torque
        float torqueRoll = TorqueCalcMethods::getTorqueRoll();  // sideways-roll torque


    Justin Albert  jalbert@uvic.ca             initial version 14 Aug. 2018

    @section  HISTORY

    v1.0  - First version
*/
/**************************************************************************/


#ifndef __TORQUECALCMETHODS_HH__
#define __TORQUECALCMETHODS_HH__

//              ---------------------
//              -- Class Interface --
//              ---------------------
class TorqueCalcMethods {

    public:

	static float getTorqueAzim();      // Get the present azimuthal torque in Newton-meters.  Clockwise, as viewed from
                                           // below, is positive; counter-clockwise viewed from below is negative.

	static float getTorqueElev();      // Get the present elevational torque in Newton-meters.  Nose-up torque is positive,
                                           // nose-down torque is negative.

	static float getTorqueRoll();      // Get the present sideways-roll torque in Newton-meters. Right-side down torque is
                                           // positive; left-side down torque is negative.



        static float getMutualTorque();    // Get the present azimuthal torque on the gondola from the balloon (as a function
                                           // of the degree difference in heading between the balloon and the gondola).

        static float getMotorTorque(int propNum); 
                                           // Get the absolute magnitude of the present torque generated by motor 1, 2, 3, or 4.


    protected:                             // Helper functions, used internally by the above static methods.

        static float getTorqueCoefficientC_Q(     float   advanceRatioJ );


    private:

        TorqueCalcMethods() {}             // Constructor should never be used.
	
};

#endif // __TORQUECALCMETHODS_HH__
