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
 : _outsideTemp(                0. ),
   _outsideAirPressure(    101325. ),        // standard atmospheric pressure
   _forwardSpeedRelToWind(      0. ),
   _elevationASL(               5. )
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



void
ExternalEnvironState::setOutsideTemp(            float outsideTemp           )
{
  _outsideTemp           = outsideTemp;
}

void
ExternalEnvironState::setOutsideAirPressure(     float outsideAirPressure    )
{
  _outsideAirPressure    = outsideAirPressure;
}

void
ExternalEnvironState::setForwardSpeedRelToWind(  float forwardSpeedRelToWind )
{
  _forwardSpeedRelToWind = forwardSpeedRelToWind;
}

void
ExternalEnvironState::setElevationASL(           float elevationASL          )
{
  _elevationASL          = elevationASL;
}


