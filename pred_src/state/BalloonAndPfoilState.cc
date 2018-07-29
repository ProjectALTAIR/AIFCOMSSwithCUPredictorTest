/**************************************************************************/
/*!
        @file     BalloonAndPfoilState.cc
        @author1  Justin Albert (jalbert@uvic.ca)
        @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
        @author3  Aravind Shaj  (aravindshaj@gmail.com)
        @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
        @license  GPL

        This is the class containing the present state of the ALTAIR
        parafoil, balloon, rigging, and helium bleed valve.
    
        Justin Albert  jalbert@uvic.ca            first version 7 Jul. 2018

        @section  HISTORY

        v1.0  - First version
*/
/**************************************************************************/

//-----------------------
// This Class's Header --
//-----------------------
#include "BalloonAndPfoilState.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------


//              ----------------------------------------
//              -- Public Function Member Definitions --
//              ----------------------------------------

//---------------
// Constructor --
//---------------
BalloonAndPfoilState::BalloonAndPfoilState()
  : _isCutdown(false)
{
}

//--------------
// Destructor --
//--------------
BalloonAndPfoilState::~BalloonAndPfoilState()
{
}


//-----------
// Methods --
//-----------
bool
BalloonAndPfoilState::getIsCutdown()
{
  return _isCutdown;
}

void
BalloonAndPfoilState::setIsCutdown( bool isCutdown )
{
  _isCutdown = isCutdown;
}
