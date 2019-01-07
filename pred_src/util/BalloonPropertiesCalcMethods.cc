/**************************************************************************/
/*!
    @file     BalloonPropertiesCalcMethods.cc
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//-----------------------
// This Class's Header --
//-----------------------

#include "BalloonPropertiesCalcMethods.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

#include "../state/ALTAIR_state.hh"
#include "../state/ExternalEnvironState.hh"
#include "../state/BalloonAndPfoilState.hh"
#include "../state/GondolaAndPropState.hh"

//              ----------------------------------------
//              -- Public Member Function Definitions --
//              ----------------------------------------
//
//
float BalloonPropertiesCalcMethods::getBalloonRadius()
{
        float zerothOrderBalRad = getZerothOrderBalloonRadius()                               ;
//        float radiusCorrection  = getNeoHookianRadiusCorrection()                             ;
        float radiusCorrection  = getMooneyRivlinRadiusCorrection()                           ;
//        float radiusCorrection  = getGentRadiusCorrection()                                   ;
        return                    (zerothOrderBalRad - radiusCorrection)                      ; 
}

//
float BalloonPropertiesCalcMethods::getAddnlRadiusStretchFromPayload()
{
        ExternalEnvironState* extEnv 	  = altairState->getExtEnv()                          ;
        BalloonAndPfoilState* balAndPfoil = altairState->getBalAndPfoil()                     ;
        GondolaAndPropState*  gondAndProp = altairState->getGondAndProp()                     ;

        float massHangingDown = gondAndProp->getGondolaTotalMass() +
                                balAndPfoil->getParafoilAndRiggingMass()                      ;

        return (2. * gravConstantLittleg * massHangingDown) /                                    // The 2. is a somewhat arbitrary
               (M_PI * (getInteriorPressure() - extEnv->getOutsideAirPressure()) *               // constant that in reality will
                getBalloonRadius())                                                           ;  // depend on factors such as the
}                                                                                                // diameter of the balloon neck, etc.

//
float BalloonPropertiesCalcMethods::getInteriorPressure()
{
	ExternalEnvironState* extEnv 	  = altairState->getExtEnv()                          ;

//        float pressureCorrection = getNeoHookianPressureCorrection()                          ;
        float pressureCorrection = getMooneyRivlinPressureCorrection()                        ;
//        float pressureCorrection = getGentPressureCorrection()                                ;

	return                     ((extEnv->getOutsideAirPressure()) + pressureCorrection)   ;
}

float BalloonPropertiesCalcMethods::getTotalMassOfAllBalloonComponents()
{
        BalloonAndPfoilState* balAndPfoil = altairState->getBalAndPfoil()                     ;
        GondolaAndPropState*  gondAndProp = altairState->getGondAndProp()                     ;

        return  (balAndPfoil->getBalloonHeliumMass() +
                 balAndPfoil->getBalloonLatexMass() +
                 balAndPfoil->getParafoilAndRiggingMass() +
                 gondAndProp->getGondolaTotalMass()         )                                 ;
}

float BalloonPropertiesCalcMethods::getMassOfDisplacedAir()
{
        ExternalEnvironState* extEnv      = altairState->getExtEnv()                          ;
        float                 balRad      = getBalloonRadius()                                ;

        return (4. * M_PI / 3.) * balRad * balRad * balRad * extEnv->getOutsideAirDensity()   ;
}

//              ----------------------------------------
//              -- Private Member Function Definitions --
//              ----------------------------------------
//
//
float BalloonPropertiesCalcMethods::getZerothOrderBalloonRadius()
{
        ExternalEnvironState* extEnv 	  = altairState->getExtEnv()                          ;
        BalloonAndPfoilState* balAndPfoil = altairState->getBalAndPfoil()                     ;

        float zerothOrderBalVol = balAndPfoil->getBalloonHeliumMass() * 
                                  idealGasConstant * 
                                  (extEnv->getOutsideTemp() + kelvinMinusCelsius) / 
                                  (heliumMolarMass * extEnv->getOutsideAirPressure())         ;
        return                    pow(zerothOrderBalVol/(4.*M_PI/3.), (1./3.))                ;
}

float BalloonPropertiesCalcMethods::getNeoHookianRadiusCorrection() 
{
        ExternalEnvironState* extEnv 	  = altairState->getExtEnv()                          ;
        BalloonAndPfoilState* balAndPfoil = altairState->getBalAndPfoil()                     ;

        float alpha = extEnv->getOutsideAirPressure()                                         ;
        float beta  = balAndPfoil->getBalloonHeliumMass() * idealGasConstant *
                      (extEnv->getOutsideTemp() + kelvinMinusCelsius) / heliumMolarMass       ;
        float phi   = pow((alpha/beta), (1./3.))                                              ;
        float gamma = 2. * balAndPfoil->getBalloonShearModulus() * 
                      balAndPfoil->getBalloonUnstretchedThickness()                           ;
        float delta = gamma * pow((phi * balAndPfoil->getBalloonUnstretchedRadius()), 6.)     ;

        return ((gamma - delta) / ((3. * alpha) - (gamma * phi) + (7. * delta * phi)))        ;
}

float BalloonPropertiesCalcMethods::getMooneyRivlinRadiusCorrection() 
{
        ExternalEnvironState* extEnv 	  = altairState->getExtEnv()                          ;
        BalloonAndPfoilState* balAndPfoil = altairState->getBalAndPfoil()                     ;
        float                 balUnsRad   = balAndPfoil->getBalloonUnstretchedRadius()        ;

        float alpha  = extEnv->getOutsideAirPressure()                                        ;
        float beta   = balAndPfoil->getBalloonHeliumMass() * idealGasConstant *
                       (extEnv->getOutsideTemp() + kelvinMinusCelsius) / heliumMolarMass      ;
        float phi    = pow((alpha/beta), (1./3.))                                             ;
        float gamma  = 2. * balAndPfoil->getBalloonShearModulus() * 
                       balAndPfoil->getBalloonUnstretchedThickness()                          ;
        float delta  = gamma * pow((phi * balUnsRad), 6.)                                     ;
        float mrCorr = LATEX_KAPPA_MOONEYRIVLIN / (phi * balUnsRad * balUnsRad)               ;
        float eta    = gamma * mrCorr                                                         ;
        float rho    = delta * mrCorr                                                         ;

        return ((gamma - delta + (eta/phi) - (rho/phi)) / 
                ((3. * alpha) - (gamma * phi) + (7. * delta * phi) + eta + (5. * rho)))       ;
}

float BalloonPropertiesCalcMethods::getGentRadiusCorrection() 
{
        return 0.                                                                             ;   // will implement later
}



float BalloonPropertiesCalcMethods::getNeoHookianPressureCorrection() 
{
        return 3. * altairState->getExtEnv()->getOutsideAirPressure() * 
               (getNeoHookianRadiusCorrection() / getZerothOrderBalloonRadius())              ;
}

float BalloonPropertiesCalcMethods::getMooneyRivlinPressureCorrection() 
{
        return 3. * altairState->getExtEnv()->getOutsideAirPressure() * 
               (getMooneyRivlinRadiusCorrection() / getZerothOrderBalloonRadius())            ;
}

float BalloonPropertiesCalcMethods::getGentPressureCorrection() 
{
        return 3. * altairState->getExtEnv()->getOutsideAirPressure() * 
               (getGentRadiusCorrection() / getZerothOrderBalloonRadius())                    ;
}

