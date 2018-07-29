/**************************************************************************/
/*!
        @file     OptSourcePayloadState.cc
        @author1  Justin Albert (jalbert@uvic.ca)
        @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
        @author3  Aravind Shaj  (aravindshaj@gmail.com)
        @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
        @license  GPL

        This is the class containing the present state of the ALTAIR
        optical source payload, and of its onboard photodiode measurement.
    
        Justin Albert  jalbert@uvic.ca            first version 7 Jul. 2018

        @section  HISTORY

        v1.0  - First version
*/
/**************************************************************************/

//-----------------------
// This Class's Header --
//-----------------------
#include "OptSourcePayloadState.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------


//              ----------------------------------------
//              -- Public Function Member Definitions --
//              ----------------------------------------

//---------------
// Constructor --
//---------------
OptSourcePayloadState::OptSourcePayloadState()
 : _redLEDsAreOn(    false ),
   _yellowLEDsAreOn( false ),
   _blueLEDsAreOn(   false ),
   _greenLEDsAreOn(  false )
{
}

//--------------
// Destructor --
//--------------
OptSourcePayloadState::~OptSourcePayloadState()
{
}


//-----------
// Methods --
//-----------
bool
OptSourcePayloadState::getRedLEDsAreOn()
{
  return _redLEDsAreOn;
}

bool
OptSourcePayloadState::getYellowLEDsAreOn()
{
  return _yellowLEDsAreOn;
}

bool
OptSourcePayloadState::getBlueLEDsAreOn()
{
  return _blueLEDsAreOn;
}

bool
OptSourcePayloadState::getGreenLEDsAreOn()
{
  return _greenLEDsAreOn;
}


void
OptSourcePayloadState::setRedLEDsAreOn(    bool redLEDsAreOn    )
{
  _redLEDsAreOn    = redLEDsAreOn;
}

void
OptSourcePayloadState::setYellowLEDsAreOn( bool yellowLEDsAreOn )
{
  _yellowLEDsAreOn = yellowLEDsAreOn;
}

void
OptSourcePayloadState::setBlueLEDsAreOn(   bool blueLEDsAreOn   )
{
  _blueLEDsAreOn   = blueLEDsAreOn;
}

void
OptSourcePayloadState::setGreenLEDsAreOn(  bool greenLEDsAreOn  )
{
  _greenLEDsAreOn  = greenLEDsAreOn;
}

