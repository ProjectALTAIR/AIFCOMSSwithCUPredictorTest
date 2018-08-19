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

        virtual void                   setIsCutdown(                  bool  isCutdown                  );

        virtual void                   setAreSolarPanelsInstalled(    bool  areSolarPanelsInstalled    );
        virtual void                   setNumOfSolarPanels(           int   numOfSolarPanels           );
        virtual void                   setIndivSolarPanelWidth(       float indivSolarPanelWidth       );
        virtual void                   setIndivSolarPanelLength(      float indivSolarPanelLength      );
        virtual void                   setSolarPanelSetupUpperRadius( float solarPanelSetupUpperRadius );
        virtual void                   setSolarPanelAvgTemp(          float solarPanelAvgTemp          );
        virtual void                   setSolarPanelAvgEfficiency(    float solarPanelAvgEfficiency    );

        virtual void                   setBalloonRadius(              float balloonRadius              );


    private:

        bool                           _isCutdown;

        bool                           _areSolarPanelsInstalled;
        int                            _numOfSolarPanels;
        float                          _indivSolarPanelWidth;
        float                          _indivSolarPanelLength;
        float                          _solarPanelSetupUpperRadius;
        float                          _solarPanelAvgTemp;
        float                          _solarPanelAvgEfficiency;

        float                          _balloonRadius;
        float                          _balloonHeading;
};

#endif // __BALLOONANDPFOILSTATE_HH__
