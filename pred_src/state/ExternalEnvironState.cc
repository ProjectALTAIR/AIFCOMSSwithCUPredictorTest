/**************************************************************************/
/*!
        @file     ExternalEnvironState.cc
        @author1  Justin Albert (jalbert@uvic.ca)
        @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
        @author3  Aravind Shaj  (aravindshaj@gmail.com)
        @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
        @license  GPL

        This is the class containing the present state of all of ALTAIR's
        relevant external environmental conditions.
    
        Justin Albert  jalbert@uvic.ca            first version 7 Jul. 2018

        @section  HISTORY

        v1.0  - First version
*/
/**************************************************************************/

//-----------------------
// This Class's Header --
//-----------------------
#include "ExternalEnvironState.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//              ----------------------------------------
//              -- Public Function Member Definitions --
//              ----------------------------------------

//---------------
// Constructor --
//---------------
ExternalEnvironState::ExternalEnvironState()
 : 
   _currentLat(                 0. ),
   _currentLon(                 0. ),
   _elevationASL(               5. ),
   _terrainHeight(              5. ),
   _ascentRate(                 0. ),        // change to 0. later
   _currentHeading(             0. ),
   _currentTime(                0  ),
   _outsideTemp(                0. ),
   _outsideAirPressure(    101325. ),        // standard atmospheric pressure
   _forwardSpeedRelToWind(      0. )
{
}

//--------------
// Destructor --
//--------------
ExternalEnvironState::~ExternalEnvironState()
{
}

//-----------
// Methods --
//-----------
long int
ExternalEnvironState::getCurrentTime()
{
  return _currentTime;
}

float
ExternalEnvironState::getCurrentLat()
{
  return _currentLat;
}

float
ExternalEnvironState::getCurrentLon()
{
  return _currentLon;
}

float
ExternalEnvironState::getCurrentHeading()
{
  return _currentHeading;
}

float
ExternalEnvironState::getOutsideTemp()
{
  return _outsideTemp;
}

float
ExternalEnvironState::getOutsideAirPressure()
{
  return _outsideAirPressure;
}

// Assume, for now, that air is a perfect ideal gas, with a constant molar mass,
// and thus use the ideal gas equation of state in order to get the density:
//
//   density = (pressure * molar mass) / (temperature [in K] * ideal gas constant)
//
float
ExternalEnvironState::getOutsideAirDensity()
{
  float   tempInK = _outsideTemp + kelvinMinusCelsius;
  return (   _outsideAirPressure * airMolarMass ) / ( tempInK * idealGasConstant );
}

float
ExternalEnvironState::getForwardSpeedRelToWind()
{
  return _forwardSpeedRelToWind;
}

float
ExternalEnvironState::getElevationASL()
{
  return _elevationASL;
}

float
ExternalEnvironState::getAscentRate()
{
  return _ascentRate;
}

float
ExternalEnvironState::getTerrainHeight()
{
  return _terrainHeight;
}


void                   
ExternalEnvironState::setVariable(               size_t   variableID ,
                                                 float    dataVariable          )
{
  switch (variableID) {
    case EXTERNALENVIRONSTATE_currentLat:
      setCurrentLat(dataVariable);
      break;
    case EXTERNALENVIRONSTATE_currentLon:
      setCurrentLon(dataVariable);
      break;
    case EXTERNALENVIRONSTATE_elevationASL:
      setElevationASL(dataVariable);
      break;
    case EXTERNALENVIRONSTATE_terrainHeight:
      setTerrainHeight(dataVariable);
      break;
    case EXTERNALENVIRONSTATE_ascentRate:
      setAscentRate(dataVariable);
      break;
    case EXTERNALENVIRONSTATE_currentHeading:
      setCurrentHeading(dataVariable);
      break;
    case EXTERNALENVIRONSTATE_currentTime:
      setCurrentTime(dataVariable);
      break;
    case EXTERNALENVIRONSTATE_outsideTemp:
      setOutsideTemp(dataVariable);
      break;
    case EXTERNALENVIRONSTATE_outsideAirPressure:
      setOutsideAirPressure(dataVariable);
      break;
    case EXTERNALENVIRONSTATE_forwardSpeedRelToWind:
      setForwardSpeedRelToWind(dataVariable);
      break;
    default:                                      
      break;                                          // should really be an error here ...
  }
}

void
ExternalEnvironState::setCurrentTime(            long int currentTime           )
{
  _currentTime           = currentTime;
}

void
ExternalEnvironState::setCurrentLat(             float    currentLat            )
{
  _currentLat            = currentLat;
}

void
ExternalEnvironState::setCurrentLon(             float    currentLon            )
{
  _currentLon            = currentLon;
}

void
ExternalEnvironState::setCurrentHeading(         float    currentHeading        )
{
  _currentHeading        = currentHeading;
}

void
ExternalEnvironState::setOutsideTemp(            float    outsideTemp           )
{
  _outsideTemp           = outsideTemp;
}

void
ExternalEnvironState::setOutsideAirPressure(     float    outsideAirPressure    )
{
  _outsideAirPressure    = outsideAirPressure;
}

void
ExternalEnvironState::setForwardSpeedRelToWind(  float    forwardSpeedRelToWind )
{
  _forwardSpeedRelToWind = forwardSpeedRelToWind;
}

void
ExternalEnvironState::setElevationASL(           float    elevationASL          )
{
  _elevationASL          = elevationASL;
}

void
ExternalEnvironState::setAscentRate(             float    ascentRate            )
{
  _ascentRate            = ascentRate;
}

void
ExternalEnvironState::setTerrainHeight(          float    terrainHeight         )
{
  _terrainHeight         = terrainHeight;
}


