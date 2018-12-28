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
#include "DragCalcMethods.hh"

//              ----------------------------------------
//              -- Public Member Function Definitions --
//              ----------------------------------------
//
//
float AscentAndBurstCalcMethods::getEquilibAscentRate()
{
        ExternalEnvironState* extEnv 	  = altairState->getExtEnv()                          ;
        BalloonAndPfoilState* balAndPfoil = altairState->getBalAndPfoil()                     ;
        GondolaAndPropState*  gondAndProp = altairState->getGondAndProp()                     ;

        if (extEnv->getElevationASL() >= getBurstAltitude())  balAndPfoil->setIsCutdown(true) ;
        if (!balAndPfoil->getIsCutdown())                     return 5.                       ;    // is ascending
        return -(DragCalcMethods::getParafoilDrag())/sqrt(extEnv->getOutsideAirDensity())     ;    // is descending
}


//
float AscentAndBurstCalcMethods::getBurstAltitude()
{
	ExternalEnvironState* extEnv 	  = altairState->getExtEnv()                          ;
	BalloonAndPfoilState* balAndPfoil = altairState->getBalAndPfoil()                     ;

	return 30000.                                                                         ;
}


