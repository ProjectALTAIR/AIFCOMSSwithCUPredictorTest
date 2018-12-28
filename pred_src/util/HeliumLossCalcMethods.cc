/**************************************************************************/
/*!
    @file     HeliumLossCalcMethods.cc
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//-----------------------
// This Class's Header --
//-----------------------

#include "HeliumLossCalcMethods.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

#include "../state/ALTAIR_state.hh"
#include "../state/ExternalEnvironState.hh"
#include "../state/BalloonAndPfoilState.hh"

//              ----------------------------------------
//              -- Public Member Function Definitions --
//              ----------------------------------------
//
//
float HeliumLossCalcMethods::getHeLossRateThruBalSkin()
{
        ExternalEnvironState* extEnv 	  = altairState->getExtEnv()					;
        BalloonAndPfoilState* balAndPfoil = altairState->getBalAndPfoil()				;

        return 1.                                                                                       ;
}


//
float HeliumLossCalcMethods::getHeLossRateThruValve()
{
	ExternalEnvironState* extEnv 	  = altairState->getExtEnv()					;
	BalloonAndPfoilState* balAndPfoil = altairState->getBalAndPfoil()				;

	return 2.                                                                                       ;
}


