/**************************************************************************/
/*!
    @file     AscentAndBurstCalcMethods.cc
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//-----------------------
// This Class's Header --
//-----------------------

#include "AscentAndBurstCalcMethods.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

#include "../state/ALTAIR_state.hh"
#include "../state/ExternalEnvironState.hh"
#include "../state/BalloonAndPfoilState.hh"
#include "../state/GondolaAndPropState.hh"
#include "BalloonPropertiesCalcMethods.hh"
#include "DragCalcMethods.hh"

//              ----------------------------------------
//              -- Public Member Function Definitions --
//              ----------------------------------------
//
//
float AscentAndBurstCalcMethods::getPresentAscentTerminalVelocity()
{
        ExternalEnvironState* extEnv      = altairState->getExtEnv()                          ;
        float                 buoyMass    = 
                    BalloonPropertiesCalcMethods::getMassOfDisplacedAir() -
                    BalloonPropertiesCalcMethods::getTotalMassOfAllBalloonComponents()        ;
        float                 sqrtMass    = buoyMass > 0. ? sqrt(buoyMass) : -sqrt(-buoyMass) ;

        return sqrtMass  /            
               (DragCalcMethods::getApproxEffSphericalDragCoef() *
                BalloonPropertiesCalcMethods::getBalloonRadius() * 
                sqrt(extEnv->getOutsideAirDensity())              )                           ;
//        return 5.                                                                             ;
}

float AscentAndBurstCalcMethods::getPresentDescentTerminalVelocity()
{
        ExternalEnvironState* extEnv      = altairState->getExtEnv()                          ;
        BalloonAndPfoilState* balAndPfoil = altairState->getBalAndPfoil()                     ;
        GondolaAndPropState*  gondAndProp = altairState->getGondAndProp()                     ;

        float                 sqrtMass    = sqrt(balAndPfoil->getParafoilAndRiggingMass() +
                                                 gondAndProp->getGondolaTotalMass()         ) ;
        return -sqrtMass  /            
                (DragCalcMethods::getApproxEffRectangDragCoef() *
                 sqrt(balAndPfoil->getParafoilArea() * 
                      extEnv->getOutsideAirDensity()   )          )                           ;
}

float AscentAndBurstCalcMethods::getVerticalAcceleration()
{
        ExternalEnvironState* extEnv 	  = altairState->getExtEnv()                          ;
        BalloonAndPfoilState* balAndPfoil = altairState->getBalAndPfoil()                     ;
        GondolaAndPropState*  gondAndProp = altairState->getGondAndProp()                     ;

        if (balAndPfoil->getIsCutdown()) {                                                         // is descending
            return (DragCalcMethods::getParafoilDrag(extEnv->getAscentRate())) - 
                   (gondAndProp->getGondolaTotalMass() + balAndPfoil->getParafoilAndRiggingMass()) * gravConstantLittleg;
        }
        return gravConstantLittleg;
//               - (DragCalcMethods::getBalloonDrag(extEnv->getAscentRate()) / 5000.) ;
/*
                (balAndPfoil->getBalloonHeliumMass() + 
                 balAndPfoil->getBalloonLatexMass() +
                 balAndPfoil->getParafoilAndRiggingMass() +
                 gondAndProp->getGondolaTotalMass()         )              )                  ;
*/
//        return 0.5;

/*
        return gravConstantLittleg * (balAndPfoil->getBalloonHeliumMass() *
                                      (airMolarMass/heliumMolarMass - 1.) -
                                      balAndPfoil->getBalloonLatexMass() -
                                      balAndPfoil->getParafoilAndRiggingMass() -
                                      gondAndProp->getGondolaTotalMass());
*/
/*
        return gravConstantLittleg * (balAndPfoil->getBalloonHeliumMass() * 
                                      ((airMolarMass    * extEnv->getOutsideAirPressure()) /
                                       (heliumMolarMass * BalloonPropertiesCalcMethods::getInteriorPressure()) - 1.) -
                                      balAndPfoil->getBalloonLatexMass() -
                                      balAndPfoil->getParafoilAndRiggingMass() -
                                      gondAndProp->getGondolaTotalMass())
               - DragCalcMethods::getBalloonDrag(extEnv->getAscentRate())                    ;
*/
//        return 0.                                                                             ;
/*
        if (extEnv->getElevationASL() >= getBurstAltitude())  balAndPfoil->setIsCutdown(true) ;
        if (!balAndPfoil->getIsCutdown()) {                                                        // is ascending
            return 5.                                                                         ;    // this (return 5 m/s) is the default (stupid) thing to do  
        }
        return -(DragCalcMethods::getParafoilDrag(5.))/sqrt(extEnv->getOutsideAirDensity())   ;    // is descending
*/
}


//
float AscentAndBurstCalcMethods::getBurstAltitude()
{
	ExternalEnvironState* extEnv 	  = altairState->getExtEnv()                          ;
	BalloonAndPfoilState* balAndPfoil = altairState->getBalAndPfoil()                     ;

	return 30000.                                                                         ;
}


