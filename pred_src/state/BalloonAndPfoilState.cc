/**************************************************************************/
/*!
        @file     BalloonAndPfoilState.cc
        @author1  Justin Albert (jalbert@uvic.ca)
        @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
        @author3  Aravind Shaj  (aravindshaj@gmail.com)
        @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
        @license  GPL

        This is the class containing the present state of the ALTAIR
        parafoil, balloon, solar panels (if installed), rigging, and 
        helium bleed valve.
    
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
  : _isCutdown(               false   ),
    _areSolarPanelsInstalled( false   ),
    _numOfSolarPanels(            0   ),            
    _indivSolarPanelWidth(        1.0 ),            // in meters
    _indivSolarPanelLength(       1.0 ),            // in meters
    _solarPanelSetupUpperRadius(  1.0 ),            // in meters
    _solarPanelAvgTemp(           0.  ),            // in Celsius
    _solarPanelAvgEfficiency(     0.1 ),    
    _balloonRadius(               1.5 )             // in meters
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
void
BalloonAndPfoilState::setIsCutdown(                  bool  isCutdown                  )
{
  _isCutdown                  = isCutdown                  ;
}


void
BalloonAndPfoilState::setAreSolarPanelsInstalled(    bool  areSolarPanelsInstalled    )
{
  _areSolarPanelsInstalled    = areSolarPanelsInstalled    ;
}

void
BalloonAndPfoilState::setNumOfSolarPanels(           int   numOfSolarPanels           )
{
  _numOfSolarPanels           = numOfSolarPanels           ;
}

void
BalloonAndPfoilState::setIndivSolarPanelWidth(       float indivSolarPanelWidth       )
{
  _indivSolarPanelWidth       = indivSolarPanelWidth       ;
}

void
BalloonAndPfoilState::setIndivSolarPanelLength(      float indivSolarPanelLength      )
{
  _indivSolarPanelLength      = indivSolarPanelLength      ;
}

void
BalloonAndPfoilState::setSolarPanelSetupUpperRadius( float solarPanelSetupUpperRadius )
{
  _solarPanelSetupUpperRadius = solarPanelSetupUpperRadius ;
}

void
BalloonAndPfoilState::setSolarPanelAvgTemp(          float solarPanelAvgTemp          )
{
  _solarPanelAvgTemp          = solarPanelAvgTemp          ;
}

void
BalloonAndPfoilState::setSolarPanelAvgEfficiency(    float solarPanelAvgEfficiency    )
{
  _solarPanelAvgEfficiency    = solarPanelAvgEfficiency    ;
}



void
BalloonAndPfoilState::setBalloonRadius(              float balloonRadius              )
{
  _balloonRadius              = balloonRadius              ;
}
