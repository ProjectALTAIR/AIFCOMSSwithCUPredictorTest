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
        virtual float                  getIndivSolarPanelWidth()        { return _indivSolarPanelWidth;       }  // in meters
        virtual float                  getIndivSolarPanelLength()       { return _indivSolarPanelLength;      }  // in meters
        virtual float                  getSolarPanelSetupUpperRadius()  { return _solarPanelSetupUpperRadius; }  // in meters
        virtual float                  getSolarPanelAvgTemp()           { return _solarPanelAvgTemp;          }  // in Celsius
        virtual float                  getSolarPanelAvgEfficiency()     { return _solarPanelAvgEfficiency;    }

        virtual float                  getBalloonRadius()               { return _balloonRadius;              }  // in meters
        virtual float                  getBalloonHeading()              { return _balloonHeading;             }  // Present balloon heading in degrees East of true North.
                                                                                                                 // The range of _balloonHeading is -infinity to +infinity, 
                                                                                                                 // rather than 0 to 360, since the string connecting the 
                                                                                                                 // balloon to the payload can get twisted.  If the difference
                                                                                                                 // | getBalloonHeading() - getCurrentHeading() | > 360, then
                                                                                                                 // that implies there is at least one full twist in the string.
        static  const size_t           numDataElements                = 10                               ;
        virtual void                   setVariable(                   size_t variableID ,
                                                                      float  dataVariable               );

        virtual void                   setIsCutdown(                  bool   isCutdown                  );

        virtual void                   setAreSolarPanelsInstalled(    bool   areSolarPanelsInstalled    );
        virtual void                   setNumOfSolarPanels(           int    numOfSolarPanels           );
        virtual void                   setIndivSolarPanelWidth(       float  indivSolarPanelWidth       );
        virtual void                   setIndivSolarPanelLength(      float  indivSolarPanelLength      );
        virtual void                   setSolarPanelSetupUpperRadius( float  solarPanelSetupUpperRadius );
        virtual void                   setSolarPanelAvgTemp(          float  solarPanelAvgTemp          );
        virtual void                   setSolarPanelAvgEfficiency(    float  solarPanelAvgEfficiency    );

        virtual void                   setBalloonRadius(              float  balloonRadius              );
        virtual void                   setBalloonHeading(             float  balloonHeading             );

    private:

#define            BALLOONANDPFOILSTATE_isCutdown                       0
        bool                           _isCutdown;

#define            BALLOONANDPFOILSTATE_areSolarPanelsInstalled         1
        bool                           _areSolarPanelsInstalled;
#define            BALLOONANDPFOILSTATE_numOfSolarPanels                2
        int                            _numOfSolarPanels;
#define            BALLOONANDPFOILSTATE_indivSolarPanelWidth            3
        float                          _indivSolarPanelWidth;
#define            BALLOONANDPFOILSTATE_indivSolarPanelLength           4
        float                          _indivSolarPanelLength;
#define            BALLOONANDPFOILSTATE_solarPanelSetupUpperRadius      5
        float                          _solarPanelSetupUpperRadius;
#define            BALLOONANDPFOILSTATE_solarPanelAvgTemp               6
        float                          _solarPanelAvgTemp;
#define            BALLOONANDPFOILSTATE_solarPanelAvgEfficiency         7
        float                          _solarPanelAvgEfficiency;

#define            BALLOONANDPFOILSTATE_balloonRadius                   8
        float                          _balloonRadius;
#define            BALLOONANDPFOILSTATE_balloonHeading                  9
        float                          _balloonHeading;
};

#endif // __BALLOONANDPFOILSTATE_HH__
