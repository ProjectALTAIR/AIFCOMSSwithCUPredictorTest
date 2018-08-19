/**************************************************************************/
/*!
    @file     ThrustCalcMethods.cc
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
    of the thrust vector (i.e., the forward speed fSpeed); etc.

    Note that the specific momentum calculation methods ("interpolation",
    "momentum transfer", etc) are static member functions of this class.  The 
    interface to obtain the different possible thrust calculations is via 
    these static member functions within this class.

    An example usage:
        #include "ThrustCalcMethods.hh"

        double thrust = ThrustCalcMethods::getInterpMethodThrust();


    Patrick Ryan   patrick.ryan5787@gmail.com  predecessor written 3 Jul. 2018
    Justin Albert  jalbert@uvic.ca             completely revamped 5 Jul. 2018

    @section  HISTORY

    v1.0  - First version
*/
/**************************************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//-----------------------
// This Class's Header --
//-----------------------
#include "ThrustCalcMethods.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "../state/ALTAIR_state.hh"
#include "../state/ExternalEnvironState.hh"
#include "../state/GondolaAndPropState.hh"

//              ----------------------------------------
//              -- Public Member Function Definitions --
//              ----------------------------------------
//
// Get the thrust (in Newtons) using the interpolation method:
//
// Estimated Thrust = (Thrust coefficient C_T) * (Air density rho) * (Revolution rate n)^2 * (Propeller diameter d)^4 .
// Sum the thrust from all 4 propellers, and return the total estimated thrust (in Newtons).
//
float ThrustCalcMethods::getInterpMethodThrust()
{
        ExternalEnvironState* extEnv        =  altairState->getExtEnv()                 ;
        GondolaAndPropState*  gondAndProp   =  altairState->getGondAndProp()            ;
        float                 thrustProp[4], propRPS[4]                                 ;  // calculated for each of the 4 propellers

        float                 d             = gondAndProp->getPropellerDiameter()       ;
        float                 rho           = extEnv->getOutsideAirDensity()            ;
        for (int i = 0; i < 4; ++i) {
                              propRPS[i]    = (gondAndProp->getRPMMotor(i+1))/60.       ;  // rotations per *second*
                              thrustProp[i] = getThrustCoefficientC_T(getAdvanceRatioJ(i+1)) * rho * (propRPS[i] * propRPS[i]) * (d * d * d * d);
        }
        return (thrustProp[0] + thrustProp[1] + thrustProp[2] + thrustProp[3]);
}



//
// Get the thrust using the momentum transfer method:
//
// Source: https://www.electricrcaircraftguy.com/2014/04/propeller-static-dynamic-thrust-equation-background.html
//
// propXSectionAirmassPerMetre =  airDensity * pi * propRadiusInMeters^2   (units are kg/m)
//
// maxTheoSpeed                =  rpm * mPerInch * pitch / 60.             (units are m/s)
//
// empCorrFactor               =  diameter / (3.29546 * pitch)             (unitless)
//
// thrust                      =  propXSectionAirmassPerMetre * 
//                                maxTheoSpeed                *
//                               (maxTheoSpeed - fSpeed)      *
//                                empCorrFactor^1.5                        (units are kg.m/(s^2) -- i.e. Newtons)
//
// Sum the thrust from all 4 propellers, and return the total estimated thrust (in Newtons).
//
float ThrustCalcMethods::getMomTransMethodThrust() 
{
        float                 thrustProp[4]                                             ;
        ExternalEnvironState* extEnv             =  altairState->getExtEnv()            ;
        GondolaAndPropState*  gondAndProp        =  altairState->getGondAndProp()       ;

        float                 airDensity         =  extEnv->getOutsideAirDensity()      ;
        float                 fSpeed             =  extEnv->getForwardSpeedRelToWind()  ;
        float                 propDiam           =  gondAndProp->getPropellerDiameter() ;
        float                 pitch              =  gondAndProp->getPropellerPitch()    ;

        float                 propRadiusInMeters =  propDiam     /  2.                  ;   // propDiam is already in meters
        float                 propXSectionAirmassPerMetre       
                                                 =  airDensity   *  M_PI    * 
                                                    propRadiusInMeters      * 
                                                    propRadiusInMeters                  ;
           // Note: Here 3.29546 is an emperically determined value.  The method by which
           //       it is obtained is outlined in the source given above this method.
        float empCorrFactor                      =  propDiam     / (3.29546 * pitch)    ;  

        for (int i = 0; i < 4; ++i) {
                float         rpm                =  gondAndProp->getRPMMotor(i+1)       ;
           // Note: Here we divide by 60 simply as a conversion from RPM to RPS.  Pitch is already in meters, so maxTheoSpeed is in m/s.
                float         maxTheoSpeed       = (rpm * pitch) /  60.                 ; 

                              thrustProp[i]      =  propXSectionAirmassPerMetre *
                                                    maxTheoSpeed                *
                                                   (maxTheoSpeed - fSpeed)      *
                                                    pow(empCorrFactor,1.5)              ;
        }

        return  ( thrustProp[0] + thrustProp[1] + thrustProp[2] + thrustProp[3] )       ;
}



//              -------------------------------------------
//              -- Protected Member Function Definitions --
//              -------------------------------------------
//
//
// Advance ratio J (unitless) = (Forward speed fSpeed) / ((Propeller angular frequency angFreq) * (Propeller diameter d))
//
float ThrustCalcMethods::getAdvanceRatioJ(int propNum)
{
        float fSpeed  = altairState->getExtEnv()->getForwardSpeedRelToWind();
        float d       = altairState->getGondAndProp()->getPropellerDiameter();
        float angFreq = altairState->getGondAndProp()->getRPMMotor(propNum) * M_PI / 30.;         // turn RPM into radians/s
        return (fSpeed / (angFreq*d));                        
}


//
// Thrust coefficient C_T (unitless) ~= 0.0881 - (0.0881*J / 0.7)
//  (see http://dcrc.org/pdf/Model%20Propellers%20Article.pdf and Aravind's "DeterminingPropellerThrust" Word document)
//
float ThrustCalcMethods::getThrustCoefficientC_T(float J)
{
        const float linearCTInterp = 0.0881;                                                // linear interpolation for approximation of C_T

        if (J >= 0 && J <= 0.7)                                                             // sanity check
        {
                return linearCTInterp - ((linearCTInterp / 0.7)*J);                         // linear interpolation using above
        }
        return 0.;
}


