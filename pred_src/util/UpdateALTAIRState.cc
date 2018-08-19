/**************************************************************************/
/*!
    @file     UpdateALTAIRState.cc
    @author1  Justin Albert (jalbert@uvic.ca)
    @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
    @author3  Aravind Shaj  (aravindshaj@gmail.com)
    @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
    @license  GPL

    This is the class containing 

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

#include "UpdateALTAIRState.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

#include "../state/ALTAIR_state.hh"
#include "../state/ExternalEnvironState.hh"
#include "../state/BalloonAndPfoilState.hh"
#include "../state/GondolaAndPropState.hh"
#include "../state/OptSourcePayloadState.hh"


//              ----------------------------------------
//              -- Public Member Function Definitions --
//              ----------------------------------------
//
//
//
void UpdateALTAIRState::doUpdate()
{
	ExternalEnvironState*  extEnv 	   = altairState->getExtEnv()       ;
	BalloonAndPfoilState*  balAndPfoil = altairState->getBalAndPfoil()  ;
        GondolaAndPropState*   gondAndProp = altairState->getGondAndProp()  ;
        OptSourcePayloadState* optSource   = altairState->getOptSource()    ;

	return;
}


