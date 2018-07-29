/**************************************************************************/
/*!
        @file     GondolaAndPropState.cc
        @author1  Justin Albert (jalbert@uvic.ca)
        @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
        @author3  Aravind Shaj  (aravindshaj@gmail.com)
        @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
        @license  GPL

        This is the class containing the present state of the ALTAIR
        gondola (and of all internals, except for all state variables 
        related to the light source payload) and of the ALTAIR propulsion 
        system.
    
        Justin Albert  jalbert@uvic.ca            first version 7 Jul. 2018

        @section  HISTORY

        v1.0  - First version
*/
/**************************************************************************/

//-----------------------
// This Class's Header --
//-----------------------
#include "GondolaAndPropState.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------


//              ----------------------------------------
//              -- Public Function Member Definitions --
//              ----------------------------------------

//---------------
// Constructor --
//---------------
GondolaAndPropState::GondolaAndPropState()
{
}

//--------------
// Destructor --
//--------------
GondolaAndPropState::~GondolaAndPropState()
{
}


//-----------
// Methods --
//-----------
int
GondolaAndPropState::getRPMMotor( int motorNum )
{
  if (motorNum >= 1 && motorNum <= 4) return _rpmMotor[motorNum-1];
  else                                return -999;    // we should turn this into an actual error...
}

int
GondolaAndPropState::getRPMMotor1()
{
  return _rpmMotor[0];
}

int
GondolaAndPropState::getRPMMotor2()
{
  return _rpmMotor[1];
}

int
GondolaAndPropState::getRPMMotor3()
{
  return _rpmMotor[2];
}

int
GondolaAndPropState::getRPMMotor4()
{
  return _rpmMotor[3];
}


void
GondolaAndPropState::setRPMMotor(  int motorNum, int rpmMotor )
{
  if (motorNum >= 1 && motorNum <= 4) _rpmMotor[motorNum-1] = rpmMotor;
  else                                {}            // we should turn this into an actual error...
}

void
GondolaAndPropState::setRPMMotor1( int rpmMotor1 )
{
  _rpmMotor[0] = rpmMotor1;
}

void
GondolaAndPropState::setRPMMotor2( int rpmMotor2 )
{
  _rpmMotor[1] = rpmMotor2;
}

void
GondolaAndPropState::setRPMMotor3( int rpmMotor3 )
{
  _rpmMotor[2] = rpmMotor3;
}

void
GondolaAndPropState::setRPMMotor4( int rpmMotor4 )
{
  _rpmMotor[3] = rpmMotor4;
}

