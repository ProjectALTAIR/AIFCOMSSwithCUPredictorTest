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
    _parafoilArea(                3.0 ),            // in m^2
    _parafoilRiggingMass(         0.5 ),            // in kg
    _balloonHeliumMass(           2.9 ),            // in kg
    _balloonLatexMass(            1.5 ),            // in kg
    _balloonLatexDensity(         0.9 ),            // in kg/m^3
    _balloonUnstretchedRadius(    0.9 ),            // in meters
    _balloonUnstretchedThickns(   0.0002   ),       // in meters
    _balloonBurstingThickns(      0.000005 ),       // in meters
    _balloonShearModulus(    300000.  ),            // in Pa
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
    case BALLOONANDPFOILSTATE_parafoilArea:
      setParafoilArea(dataVariable);
      break;
    case BALLOONANDPFOILSTATE_parafoilRiggingMass:
      setParafoilAndRiggingMass(dataVariable);
      break;
    case BALLOONANDPFOILSTATE_balloonHeliumMass:
      setBalloonHeliumMass(dataVariable);
      break;
    case BALLOONANDPFOILSTATE_balloonLatexMass:
      setBalloonLatexMass(dataVariable);
      break;
    case BALLOONANDPFOILSTATE_balloonLatexDensity:
      setBalloonLatexDensity(dataVariable);
      break;
    case BALLOONANDPFOILSTATE_balloonUnstretchedRadius:
      setBalloonUnstretchedRadius(dataVariable);
      break;
    case BALLOONANDPFOILSTATE_balloonUnstretchedThickns:
      setBalloonUnstretchedThickness(dataVariable);
      break;
    case BALLOONANDPFOILSTATE_balloonBurstingThickns:
      setBalloonBurstingThickness(dataVariable);
      break;
    case BALLOONANDPFOILSTATE_balloonShearModulus:
      setBalloonShearModulus(dataVariable);
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
BalloonAndPfoilState::setParafoilArea(               float parafoilArea               )
{
  _parafoilArea               = parafoilArea               ;
}

void
BalloonAndPfoilState::setParafoilAndRiggingMass(     float parafoilRiggingMass        )
{
  _parafoilRiggingMass        = parafoilRiggingMass        ;
}



void
BalloonAndPfoilState::setBalloonHeliumMass(          float balloonHeliumMass          )
{
  _balloonHeliumMass          = balloonHeliumMass          ;
}

void
BalloonAndPfoilState::setBalloonLatexMass(           float balloonLatexMass           )
{
  _balloonLatexMass           = balloonLatexMass           ;
}

void
BalloonAndPfoilState::setBalloonLatexDensity(        float balloonLatexDensity        )
{
  _balloonLatexDensity        = balloonLatexDensity        ;
}

void
BalloonAndPfoilState::setBalloonUnstretchedRadius(   float balloonUnstretchedRadius   )
{
  _balloonUnstretchedRadius   = balloonUnstretchedRadius   ;
}

void
BalloonAndPfoilState::setBalloonUnstretchedThickness(float balloonUnstretchedThickns  )
{
  _balloonUnstretchedThickns  = balloonUnstretchedThickns  ;
}

void
BalloonAndPfoilState::setBalloonBurstingThickness(   float balloonBurstingThickns     )
{
  _balloonBurstingThickns     = balloonBurstingThickns     ;
}

void
BalloonAndPfoilState::setBalloonShearModulus(        float balloonShearModulus        )
{
  _balloonShearModulus        = balloonShearModulus        ;
}

void
BalloonAndPfoilState::setBalloonHeading(             float balloonHeading             )
{
  _balloonHeading             = balloonHeading             ;
}
