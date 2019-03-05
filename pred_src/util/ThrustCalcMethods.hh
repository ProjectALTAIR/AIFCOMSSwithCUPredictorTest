/**************************************************************************/
/*!
    @file     ThrustCalcMethods.hh
    @author1  Justin Albert (jalbert@uvic.ca)
    @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
    @author3  Aravind Shaj  (aravindshaj@gmail.com)
    @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
    @license  GPL

    This is the class containing thrust calculation methods to determine the 
    magnitude (in Newtons) of the propulsive thrust acting on the ALTAIR 
    payload at a current instant, in terms of variables such as: 1) the RPM of
    each of the 4 propellers; and 2) the magnitude of the projection of 
    ALTAIR's present velocity, minus the present wind velocity, onto the axis 
    of the thrust vector; etc.

    Note that the specific momentum calculation methods ("interpolation",
    "momentum transfer", etc) are static member functions of this class.  The
    interface to obtain the different possible thrust calculations is via 
    these static member functions within this class.

    An example usage:
        #include "ThrustCalcMethods.hh"

        double thrust = ThrustCalcMethods::getInterpMethodThrust();


    Patrick Ryan   patrick.ryan5787@gmail.com  predecessor written 3 Jul. 2018
    Justin Albert  jalbert@uvic.ca             completely revamped 4 Jul. 2018

    @section  HISTORY

    v1.0  - First version
*/
/**************************************************************************/


#ifndef __THRUSTCALCMETHODS_HH__
#define __THRUSTCALCMETHODS_HH__

//              ---------------------
//              -- Class Interface --
//              ---------------------
class ThrustCalcMethods {

    public:

	static float getInterpMethodThrust();                                  // get the present thrust in Newtons using the interpolation method
	static int   getRPMFromThrustIntMethod(   float   totalThrust   );     // given a thrust in Newtons, determine propeller RPM to create that thrust

        static float getMomTransMethodThrust();                                // get the present thrust in Newtons using the momentum transfer method

        // helper functions, used internally by the above static methods
        static float getAdvanceRatioJ(            int     propNum       );

    protected:

        static float getThrustCoefficientC_T(     float   advanceRatioJ );


    private:

        ThrustCalcMethods() {}                                                  // constructor should never be used
	
};

#endif // __THRUSTCALCMETHODS_HH__
