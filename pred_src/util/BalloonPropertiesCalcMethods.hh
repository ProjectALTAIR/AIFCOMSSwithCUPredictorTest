/**************************************************************************/
/*!
    @file     BalloonPropertiesCalcMethods.hh
    @author1  Justin Albert (jalbert@uvic.ca)
    @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
    @author3  Aravind Shaj  (aravindshaj@gmail.com)
    @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
    @license  GPL

    This is the class containing methods to 1) calculate the predicted 
    balloon radius at the exterior pressure and temp given by the current
    ExternalEnvironState (and using, for example, the Mooney-Rivlin model 
    for latex expansion); and 2) calculate the predicted interior pressure 
    inside the balloon envelope, also in the conditions given by the 
    current ExternalEnvironState.

    @section  HISTORY

    v1.0  - First version
*/
/**************************************************************************/

#ifndef __BALLOONPROPERTIESCALCMETHODS_HH__
#define __BALLOONPROPERTIESCALCMETHODS_HH__

#define   LATEX_KAPPA_MOONEYRIVLIN          (0.1)          // The parameter kappa == (1 - alpha)/alpha for latex in
                                                           // the Mooney-Rivlin latex elasticity model.
//              ---------------------
//              -- Class Interface --
//              ---------------------

class BalloonPropertiesCalcMethods {

    public:

        static float getBalloonRadius();		   // Get the predicted balloon radius, in meters (in the 
                                                           // conditions given by the current ExternalEnvironState).

        static float getAddnlRadiusStretchFromPayload();   // Get an effective additional stretch (in units of meters) 
                                                           // of the "radius," due to the fact that the balloon isn't
                                                           // in fact a sphere, but rather is stretched into an oblong 
                                                           // shape by the mass of the payload (i.e. the gondola + 
                                                           // parafoil + rigging) hanging down from its neck.

        static float getInteriorPressure();		   // Get the predicted balloon interior pressure, in Pa (in 
                                                           // the conditions given by the current ExternalEnvironState).

        static float getTotalMassOfAllBalloonComponents(); // Self-explanatory, in kg.

        static float getMassOfDisplacedAir()             ; // Get the present mass of the air displaced by the balloon, in kg.

    private:

        static float getZerothOrderBalloonRadius(); 
        static float getNeoHookianRadiusCorrection();
        static float getMooneyRivlinRadiusCorrection();
        static float getGentRadiusCorrection();

        static float getNeoHookianPressureCorrection();
        static float getMooneyRivlinPressureCorrection();
        static float getGentPressureCorrection();

        BalloonPropertiesCalcMethods() {} 		  // constructor should never be used
};



#endif // __BALLOONPROPERTIESCALCMETHODS_HH__

