/**************************************************************************/
/*!
        @file     BalloonAndPfoilState.hh
        @author1  Justin Albert (jalbert@uvic.ca)
        @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
        @author3  Aravind Shaj  (aravindshaj@gmail.com)
        @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
        @license  GPL

        This is the class containing the present state of the ALTAIR
        parafoil, balloon, solar panels (if installed), rigging, and 
        helium bleed valve.  
    
        An example usage:
            #include "ALTAIR_state.hh"
            #include "BalloonAndPfoilState.hh"

            bool isCutdown = altairState->getBalAndPfoil()->getIsCutdown();

        Justin Albert  jalbert@uvic.ca            first version 7 Jul. 2018

        @section  HISTORY

        v1.0  - First version
*/
/**************************************************************************/


#ifndef __BALLOONANDPFOILSTATE_HH__
#define __BALLOONANDPFOILSTATE_HH__

#define IDEAL_GAS_CONSTANT                     (8314.5)     // in J K^-1 kmol^-1
#define HELIUM_KILOGRAMSPERKILOMOLE               (4.0)

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

#include <stdlib.h>

//              ---------------------
//              -- Class Interface --
//              ---------------------
class   BalloonAndPfoilState {

    public:

        BalloonAndPfoilState();
        virtual ~BalloonAndPfoilState();

        virtual bool                   getIsCutdown()                   { return _isCutdown;                  }

        virtual bool                   getAreSolarPanelsInstalled()     { return _areSolarPanelsInstalled;    }
        virtual int                    getNumOfSolarPanels()            { return _numOfSolarPanels;           }
        virtual float                  getIndivSolarPanelMass()         { return _indivSolarPanelMass;        }  // in kg
        virtual float                  getIndivSolarPanelWidth()        { return _indivSolarPanelWidth;       }  // in meters
        virtual float                  getIndivSolarPanelLength()       { return _indivSolarPanelLength;      }  // in meters
        virtual float                  getSolarPanelSetupUpperRadius()  { return _solarPanelSetupUpperRadius; }  // in meters
        virtual float                  getSolarPanelAvgTemp()           { return _solarPanelAvgTemp;          }  // in Celsius
        virtual float                  getSolarPanelAvgEfficiency()     { return _solarPanelAvgEfficiency;    }

        virtual float                  getParafoilArea()                { return _parafoilArea;               }  // in m^2 (120" x 39" = 3.02 m^2)
        virtual float                  getParafoilAndRiggingMass()      { return _parafoilRiggingMass;        }  // in kg (about 0.5 kg)

        virtual float                  getBalloonHeliumMass()           { return _balloonHeliumMass;          }  // in kg (one T-size tank contains 1.44 kg of He)
        virtual float                  getBalloonLatexMass()            { return _balloonLatexMass;           }  // in kg (a common mass is 1.5 kg)
        virtual float                  getBalloonValveAndNeckRigMass()  { return _balloonValveAndNeckRigMass; }  // in kg (usually about 0.4 kg)
        virtual float                  getBalloonLatexDensity()         { return _balloonLatexDensity;        }  // in kg/m^3 (around 950 kg/m^3)
        virtual float                  getBalloonUnstretchedRadius()    { return _balloonUnstretchedRadius;   }  // in meters (a typical uninflated radius is ~0.9 m)
        virtual float                  getBalloonUnstretchedThickness() { return _balloonUnstretchedThickns;  }  // in meters (typically ~0.2 mm = 0.0002 m)
        virtual float                  getBalloonBurstingThickness()    { return _balloonBurstingThickns;     }  // in meters (typically ~5 um = 0.000005 m)
        virtual float                  getBalloonShearModulus()         { return _balloonShearModulus;        }  // Elastic shear modulus of the balloon latex
                                                                                                                 // in Pa (typically ~300 kPa = 300000 Pa)
        virtual float                  getBalloonHeading()              { return _balloonHeading;             }  // Present balloon heading in degrees East of true North.
                                                                                                                 // The range of _balloonHeading is -infinity to +infinity, 
                                                                                                                 // rather than 0 to 360, since the string connecting the 
                                                                                                                 // balloon to the payload can get twisted.  If the difference
                                                                                                                 // | getBalloonHeading() - getCurrentHeading() | > 360, then
                                                                                                                 // that implies there is at least one full twist in the string.
        static  const size_t           numDataElements                = 20                               ;
        virtual void                   setVariable(                   size_t variableID ,
                                                                      float  dataVariable               );

        virtual void                   setIsCutdown(                  bool   isCutdown                  );

        virtual void                   setAreSolarPanelsInstalled(    bool   areSolarPanelsInstalled    );
        virtual void                   setNumOfSolarPanels(           int    numOfSolarPanels           );
        virtual void                   setIndivSolarPanelMass(        float  indivSolarPanelMass        );
        virtual void                   setIndivSolarPanelWidth(       float  indivSolarPanelWidth       );
        virtual void                   setIndivSolarPanelLength(      float  indivSolarPanelLength      );
        virtual void                   setSolarPanelSetupUpperRadius( float  solarPanelSetupUpperRadius );
        virtual void                   setSolarPanelAvgTemp(          float  solarPanelAvgTemp          );
        virtual void                   setSolarPanelAvgEfficiency(    float  solarPanelAvgEfficiency    );

        virtual void                   setParafoilArea(               float  parafoilArea               );
        virtual void                   setParafoilAndRiggingMass(     float  parafoilRiggingMass        );

        virtual void                   setBalloonHeliumMass(          float  balloonHeliumMass          );
        virtual void                   setBalloonLatexMass(           float  balloonLatexMass           );
        virtual void                   setBalloonValveAndNeckRigMass( float  balloonValveAndNeckRigMass );
        virtual void                   setBalloonLatexDensity(        float  balloonLatexDensity        );
        virtual void                   setBalloonUnstretchedRadius(   float  balloonUnstretchedRadius   );
        virtual void                   setBalloonUnstretchedThickness(float  balloonUnstretchedThickns  );
        virtual void                   setBalloonBurstingThickness(   float  balloonBurstingThickns     );
        virtual void                   setBalloonShearModulus(        float  balloonShearModulus        );
        virtual void                   setBalloonHeading(             float  balloonHeading             );

    private:

#define            BALLOONANDPFOILSTATE_isCutdown                       0
        bool                           _isCutdown;

#define            BALLOONANDPFOILSTATE_areSolarPanelsInstalled         1
        bool                           _areSolarPanelsInstalled;
#define            BALLOONANDPFOILSTATE_numOfSolarPanels                2
        int                            _numOfSolarPanels;
#define            BALLOONANDPFOILSTATE_indivSolarPanelMass             3
        float                          _indivSolarPanelMass;
#define            BALLOONANDPFOILSTATE_indivSolarPanelWidth            4
        float                          _indivSolarPanelWidth;
#define            BALLOONANDPFOILSTATE_indivSolarPanelLength           5
        float                          _indivSolarPanelLength;
#define            BALLOONANDPFOILSTATE_solarPanelSetupUpperRadius      6
        float                          _solarPanelSetupUpperRadius;
#define            BALLOONANDPFOILSTATE_solarPanelAvgTemp               7
        float                          _solarPanelAvgTemp;
#define            BALLOONANDPFOILSTATE_solarPanelAvgEfficiency         8
        float                          _solarPanelAvgEfficiency;

#define            BALLOONANDPFOILSTATE_parafoilArea                    9
        float                          _parafoilArea;
#define            BALLOONANDPFOILSTATE_parafoilRiggingMass            10
        float                          _parafoilRiggingMass;

#define            BALLOONANDPFOILSTATE_balloonHeliumMass              11
        float                          _balloonHeliumMass;
#define            BALLOONANDPFOILSTATE_balloonLatexMass               12
        float                          _balloonLatexMass;
#define            BALLOONANDPFOILSTATE_balloonValveAndNeckRigMass     13
        float                          _balloonValveAndNeckRigMass;
#define            BALLOONANDPFOILSTATE_balloonLatexDensity            14
        float                          _balloonLatexDensity;
#define            BALLOONANDPFOILSTATE_balloonUnstretchedRadius       15
        float                          _balloonUnstretchedRadius;
#define            BALLOONANDPFOILSTATE_balloonUnstretchedThickns      16
        float                          _balloonUnstretchedThickns;
#define            BALLOONANDPFOILSTATE_balloonBurstingThickns         17
        float                          _balloonBurstingThickns;
#define            BALLOONANDPFOILSTATE_balloonShearModulus            18
        float                          _balloonShearModulus;
#define            BALLOONANDPFOILSTATE_balloonHeading                 19
        float                          _balloonHeading;
};

#endif // __BALLOONANDPFOILSTATE_HH__
