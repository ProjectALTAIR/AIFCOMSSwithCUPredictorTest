/**************************************************************************/
/*!
    @file     PropulsionUtils.cc
    @author1  Justin Albert (jalbert@uvic.ca)
    @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
    @author3  Aravind Shaj  (aravindshaj@gmail.com)
    @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
    @license  GPL

    Justin Albert  jalbert@uvic.ca             started 26 Jan. 2019

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
#include "PropulsionUtils.hh"

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
void PropulsionUtils::doUpdate(float *newLat, float *newLon, float *newAlt)
{
        ExternalEnvironState* extEnv        =  altairState->getExtEnv()                 ;
        GondolaAndPropState*  gondAndProp   =  altairState->getGondAndProp()            ;


}



