/**************************************************************************/
/*!
        @file     ALTAIR_state.cc
        @author1  Justin Albert (jalbert@uvic.ca)
        @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
        @author3  Aravind Shaj  (aravindshaj@gmail.com)
        @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
        @license  GPL

        This is the class containing links to the present state of the 
        ALTAIR payload, gondola, rigging, parafoil, and balloon; as well  
        as a link to the present state of all of ALTAIR's relevant 
        external environmental conditions.  
    
        Justin Albert  jalbert@uvic.ca            first version 7 Jul. 2018

        @section  HISTORY

        v1.0  - First version
*/
/**************************************************************************/

//-----------------------
// This Class's Header --
//-----------------------
#include "ALTAIR_state.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------


//-------------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions, and Forward Declarations, --
// plus, in *very* rare cases, a Global Variable (such as below) ----------
//-------------------------------------------------------------------------

static ALTAIR_state  initialState;
ALTAIR_state*        altairState   =   &initialState;

//              ----------------------------------------
//              -- Public Function Member Definitions --
//              ----------------------------------------

//---------------
// Constructor --
//---------------
ALTAIR_state::ALTAIR_state(   )
  : _externalEnvironState(  0 ),
    _balloonAndPfoilState(  0 ),
    _gondolaAndPropState(   0 ),
    _optSourcePayloadState( 0 )
{
}

//--------------
// Destructor --
//--------------
ALTAIR_state::~ALTAIR_state(  )
{
}

//-----------
// Methods --
//-----------
ExternalEnvironState*
ALTAIR_state::getExtEnv(      )
{
  return _externalEnvironState;
}

BalloonAndPfoilState*
ALTAIR_state::getBalAndPfoil( )
{
  return _balloonAndPfoilState;
}

GondolaAndPropState*
ALTAIR_state::getGondAndProp( )
{
  return _gondolaAndPropState;
}

OptSourcePayloadState*
ALTAIR_state::getOptSource(   )
{
  return _optSourcePayloadState;
}


void
ALTAIR_state::setExtEnv(        ExternalEnvironState*  statePointer   )
{
         _externalEnvironState  =                      statePointer;
}

void
ALTAIR_state::setBalAndPfoil(   BalloonAndPfoilState*  statePointer   )
{
         _balloonAndPfoilState  =                      statePointer;
}

void
ALTAIR_state::setGondAndProp(   GondolaAndPropState*   statePointer   )
{
         _gondolaAndPropState   =                      statePointer;
}

void
ALTAIR_state::setOptSource(     OptSourcePayloadState* statePointer   )
{
         _optSourcePayloadState =                      statePointer;
}


