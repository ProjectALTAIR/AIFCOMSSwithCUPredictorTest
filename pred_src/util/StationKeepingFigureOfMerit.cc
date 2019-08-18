/**************************************************************************/
/*!
    @file     StationKeepingFigureOfMerit.cc
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
#include "StationKeepingFigureOfMerit.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "../state/ALTAIR_state.hh"
#include "../state/ExternalEnvironState.hh"


#define   STATIONKEEPING_SIGMA_IN_DEGREES               (0.5)

//              ----------------------------------------
//              -- Public Member Function Definitions --
//              ----------------------------------------
//
float  StationKeepingFigureOfMerit::presentFigureOfMerit(   )
{
    // a Gaussian figure of merit centred on the station-keeping lat and lon, and independent of altitude or battery state, or any other variables
    ExternalEnvironState* extEnv               =       altairState->getExtEnv()                                          ;
    float                 latDiff              =       extEnv->getStationKeepingLat() - extEnv->getCurrentLat()          ;
    float                 lonDiff              =       extEnv->getStationKeepingLon() - extEnv->getCurrentLon()          ;
    float                 sinTheta             =       sinf(M_PI * (90.f - extEnv->getCurrentLat()) / 180.f)             ;
    float                 udiff                =       lonDiff*sinTheta                                                  ;
    float                 vdiff                =       latDiff                                                           ;
    float                 sigSquare            =       STATIONKEEPING_SIGMA_IN_DEGREES * STATIONKEEPING_SIGMA_IN_DEGREES ;
    float                 chiSquare            =       (udiff*udiff + vdiff*vdiff)/(sigSquare)                           ;
    return                                             exp(-chiSquare/2.f)                                               ;
}

float  StationKeepingFigureOfMerit::presentdFoMdlat(        )
{
    ExternalEnvironState* extEnv               =       altairState->getExtEnv()                                          ;
    float                 latDiff              =       extEnv->getStationKeepingLat() - extEnv->getCurrentLat()          ;
    float                 lonDiff              =       extEnv->getStationKeepingLon() - extEnv->getCurrentLon()          ;
    float                 sinTheta             =       sinf(M_PI * (90.f - extEnv->getCurrentLat()) / 180.f)             ;
    float                 cosTheta             =       cosf(M_PI * (90.f - extEnv->getCurrentLat()) / 180.f)             ;
    float                 udiff                =       lonDiff*sinTheta                                                  ;
    float                 vdiff                =       latDiff                                                           ;
    float                 sigSquare            =       STATIONKEEPING_SIGMA_IN_DEGREES * STATIONKEEPING_SIGMA_IN_DEGREES ;
    float                 chiSquare            =       (udiff*udiff + vdiff*vdiff)/(sigSquare)                           ;
    float                 ududlat              =       -M_PI * udiff * lonDiff * cosTheta / 180.f                        ;
    float                 vdvdlat              =               vdiff                                                     ;
    return                                             -((ududlat + vdvdlat) / sigSquare) * exp(-chiSquare/2.f)          ;
}

float  StationKeepingFigureOfMerit::presentdFoMdlon(        )
{
    ExternalEnvironState* extEnv               =       altairState->getExtEnv()                                          ;
    float                 latDiff              =       extEnv->getStationKeepingLat() - extEnv->getCurrentLat()          ;
    float                 lonDiff              =       extEnv->getStationKeepingLon() - extEnv->getCurrentLon()          ;
    float                 sinTheta             =       sinf(M_PI * (90.f - extEnv->getCurrentLat()) / 180.f)             ;
    float                 udiff                =       lonDiff*sinTheta                                                  ;
    float                 vdiff                =       latDiff                                                           ;
    float                 sigSquare            =       STATIONKEEPING_SIGMA_IN_DEGREES * STATIONKEEPING_SIGMA_IN_DEGREES ;
    float                 chiSquare            =       (udiff*udiff + vdiff*vdiff)/(sigSquare)                           ;
    float                 ududlon              =               udiff *           sinTheta                                ;
    return                                             -( ududlon            / sigSquare) * exp(-chiSquare/2.f)          ;
}

float  StationKeepingFigureOfMerit::presentdFoMdalt(        )
{
    // a placeholder, here just for the time being
    return 0.;
}

