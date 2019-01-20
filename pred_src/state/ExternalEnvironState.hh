/**************************************************************************/
/*!
        @file     ExternalEnvironState.hh
        @author1  Justin Albert (jalbert@uvic.ca)
        @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
        @author3  Aravind Shaj  (aravindshaj@gmail.com)
        @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
        @license  GPL

        This is the class containing the present state of all of ALTAIR's 
        relevant external environmental conditions.  
    
        An example usage:
            #include "ALTAIR_state.hh"
            #include "ExternalEnvironState.hh"

            float outsideTemp = altairState->getExtEnv()->getOutsideTemp();

        Justin Albert  jalbert@uvic.ca            first version 7 Jul. 2018

        @section  HISTORY

        v1.0  - First version
*/
/**************************************************************************/


#ifndef __EXTERNALENVIRONSTATE_HH__
#define __EXTERNALENVIRONSTATE_HH__

//---------------
// Definitions --
//---------------
#define         kelvinMinusCelsius        273.15                         // in K (or degC)
#define         idealGasConstant         8314.46                         // in J K^-1 kmol^-1
#define         airMolarMass               28.97                         // in kg kmol^-1
#define         heliumMolarMass             4.00                         // in kg kmol^-1
#define         gravConstantLittleg         9.81                         // in m s^-2

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

#include <stdlib.h>

//              ---------------------
//              -- Class Interface --
//              ---------------------
class   ExternalEnvironState {

    public:

        ExternalEnvironState();

        virtual ~ExternalEnvironState();

        virtual long  int              getCurrentTime(           ); // Standard Unix timestamp (i.e., seconds since midnight Jan. 1, 1970)
        virtual float                  getCurrentLat(            ); // Present ALTAIR latitude in degrees North of the Equator (negative for South)
        virtual float                  getCurrentLon(            ); // Present ALTAIR longitude in degrees East of the prime meridian (negative for West)
        virtual float                  getCurrentHeading(        ); // Present ALTAIR heading in degrees East of true North (range is [0.,360.) )
        virtual float                  getOutsideTemp(           ); // In Celsius
        virtual float                  getOutsideAirPressure(    ); // In Pascals
        virtual float                  getOutsideAirDensity(     ); // Related to pressure and temp by the equation of state for air 
        virtual float                  getForwardSpeedRelToWind( );
        virtual float                  getElevationASL(          ); // Present ALTAIR elevation (in meters) above mean sea level
        virtual float                  getAscentRate(            ); // In m/s
        virtual float                  getTerrainHeight(         ); // Height of ground (or water) terrain, in meters above mean sea level, at present ALTAIR lat and lon
        
        static  const size_t           numDataElements             = 10                           ;
        virtual void                   setVariable(              size_t   variableID ,
                                                                 float    dataVariable           );

        virtual void                   setCurrentTime(           long int currentTime            );
        virtual void                   setCurrentLat(            float    currentLat             );
        virtual void                   setCurrentLon(            float    currentLon             );
        virtual void                   setCurrentHeading(        float    currentHeading         );
        virtual void                   setOutsideTemp(           float    outsideTemp            );
        virtual void                   setOutsideAirPressure(    float    outsideAirPressure     );
        virtual void                   setForwardSpeedRelToWind( float    forwardSpeedRelToWind  );
        virtual void                   setElevationASL(          float    elevationASL           );
        virtual void                   setAscentRate(            float    ascentRate             );
        virtual void                   setTerrainHeight(         float    terrainHeight          );

    private:

#define            EXTERNALENVIRONSTATE_currentLat               0
        float                          _currentLat;
#define            EXTERNALENVIRONSTATE_currentLon               1
        float                          _currentLon;
#define            EXTERNALENVIRONSTATE_elevationASL             2
        float                          _elevationASL;
#define            EXTERNALENVIRONSTATE_terrainHeight            3
        float                          _terrainHeight;
#define            EXTERNALENVIRONSTATE_ascentRate               4
        float                          _ascentRate;
#define            EXTERNALENVIRONSTATE_currentHeading           5
        float                          _currentHeading;
#define            EXTERNALENVIRONSTATE_currentTime              6
        long int                       _currentTime;
#define            EXTERNALENVIRONSTATE_outsideTemp              7
        float                          _outsideTemp;
#define            EXTERNALENVIRONSTATE_outsideAirPressure       8
        float                          _outsideAirPressure;
#define            EXTERNALENVIRONSTATE_forwardSpeedRelToWind    9
        float                          _forwardSpeedRelToWind;

};



#endif // __EXTERNALENVIRONSTATE_HH__
