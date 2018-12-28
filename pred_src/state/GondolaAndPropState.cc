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
GondolaAndPropState::GondolaAndPropState() :
  _propAxleRotAngle(0.)
{
  for (int i = 0; i < 4; ++i) {
    _rpmMotor[i]     = 0;
    _currentMotor[i] = 0;
  }
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
GondolaAndPropState::getPropRotDir( int motorNum )
{
  switch (motorNum) {
    case 1:
      return    1 ;
    case 2:
      return   -1 ;
    case 3:
      return    1 ;
    case 4:
      return   -1 ;
    default:
      return -999 ;    // we should turn this into an actual error...
  }
}

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

float
GondolaAndPropState::getCurrentMotor( int motorNum )
{
  if (motorNum >= 1 && motorNum <= 4) return _currentMotor[motorNum-1];
  else                                return -999.;   // we should turn this into an actual error...
}

float
GondolaAndPropState::getCurrentMotor1()
{
  return _currentMotor[0];
}

float
GondolaAndPropState::getCurrentMotor2()
{
  return _currentMotor[1];
}

float
GondolaAndPropState::getCurrentMotor3()
{
  return _currentMotor[2];
}

float
GondolaAndPropState::getCurrentMotor4()
{
  return _currentMotor[3];
}


void
GondolaAndPropState::setVariable(         size_t variableID   ,
                                          float  dataVariable    )
{
  switch (variableID) {
    case GONDOLAANDPROPSTATE_propAxleRotAngle:
      setPropAxleRotAngle(dataVariable);
      break;
    case  GONDOLAANDPROPSTATE_rpmMotor         :
    case (GONDOLAANDPROPSTATE_rpmMotor     + 1):
    case (GONDOLAANDPROPSTATE_rpmMotor     + 2):
    case (GONDOLAANDPROPSTATE_rpmMotor     + 3):
      setRPMMotor(     variableID + 1 - GONDOLAANDPROPSTATE_rpmMotor     , (int) dataVariable);
      break;
    case  GONDOLAANDPROPSTATE_currentMotor     :
    case (GONDOLAANDPROPSTATE_currentMotor + 1):
    case (GONDOLAANDPROPSTATE_currentMotor + 2):
    case (GONDOLAANDPROPSTATE_currentMotor + 3):
      setCurrentMotor( variableID + 1 - GONDOLAANDPROPSTATE_currentMotor , (int) dataVariable);
      break;
    default:
      break;                                          // should really be an error here ...
  }
}

void
GondolaAndPropState::setPropAxleRotAngle( float propAxleRotAngle )
{
  _propAxleRotAngle = propAxleRotAngle;
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

void
GondolaAndPropState::setCurrentMotor(  int motorNum, float currentMotor )
{
  if (motorNum >= 1 && motorNum <= 4) _currentMotor[motorNum-1] = currentMotor;
  else                                {}            // we should turn this into an actual error...
}

void
GondolaAndPropState::setCurrentMotor1( float currentMotor1 )
{
  _currentMotor[0] = currentMotor1;
}

void
GondolaAndPropState::setCurrentMotor2( float currentMotor2 )
{
  _currentMotor[1] = currentMotor2;
}

void
GondolaAndPropState::setCurrentMotor3( float currentMotor3 )
{
  _currentMotor[2] = currentMotor3;
}

void
GondolaAndPropState::setCurrentMotor4( float currentMotor4 )
{
  _currentMotor[3] = currentMotor4;
}

