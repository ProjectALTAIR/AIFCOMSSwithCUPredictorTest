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
    _balloonRadius(               1.5 ),            // in meters
    _balloonHeading(              0.  )             // in degrees
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
BalloonAndPfoilState::setVariable(                  size_t variableID   ,
                                                    float  dataVariable               )
{
  switch (variableID) {
    case BALLOONANDPFOILSTATE_isCutdown:
      setIsCutdown((bool) dataVariable);
      break;
    case BALLOONANDPFOILSTATE_areSolarPanelsInstalled:
      setAreSolarPanelsInstalled((bool) dataVariable);
      break;
    case BALLOONANDPFOILSTATE_numOfSolarPanels:
      setNumOfSolarPanels((int) dataVariable);
      break;
    case BALLOONANDPFOILSTATE_indivSolarPanelWidth:
      setIndivSolarPanelWidth(dataVariable);
      break;
    case BALLOONANDPFOILSTATE_indivSolarPanelLength:
      setIndivSolarPanelLength(dataVariable);
      break;
    case BALLOONANDPFOILSTATE_solarPanelSetupUpperRadius:
      setSolarPanelSetupUpperRadius(dataVariable);
      break;
    case BALLOONANDPFOILSTATE_solarPanelAvgTemp:
      setSolarPanelAvgTemp(dataVariable);
      break;
    case BALLOONANDPFOILSTATE_solarPanelAvgEfficiency:
      setSolarPanelAvgEfficiency(dataVariable);
      break;
    case BALLOONANDPFOILSTATE_balloonRadius:
      setBalloonRadius(dataVariable);
      break;
    case BALLOONANDPFOILSTATE_balloonHeading:
      setBalloonHeading(dataVariable);
      break;
    default:
      break;                                          // should really be an error here ...
  }
}


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

void
BalloonAndPfoilState::setBalloonHeading(             float balloonHeading             )
{
  _balloonHeading             = balloonHeading             ;
}
