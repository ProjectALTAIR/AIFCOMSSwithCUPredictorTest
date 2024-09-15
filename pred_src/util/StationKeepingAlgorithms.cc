/**************************************************************************/
/*!
    @file     StationKeepingAlgorithms.cc
    @license  GPL

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
#include "StationKeepingAlgorithms.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "PropulsionUtils.hh"
#include "StationKeepingFigureOfMerit.hh"

//              ----------------------------------------
//              -- Public Member Function Definitions --
//              ----------------------------------------
//
void  StationKeepingAlgorithms::doStationKeep(wind_file_cache_t** cacheAddr)
{
    // a placeholder, here just for the time being
    PropulsionUtils::goFullSpeedAhead();
}

