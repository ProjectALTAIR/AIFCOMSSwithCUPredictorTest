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

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//              ---------------------
//              -- Class Interface --
//              ---------------------
class   ExternalEnvironState {

    public:

        ExternalEnvironState();

        virtual ~ExternalEnvironState();

        virtual long int               getCurrentTime();                 // Standard Unix timestamp (i.e., seconds since midnight Jan. 1, 1970)
        virtual float                  getOutsideTemp();                 // In Celsius
        virtual float                  getOutsideAirPressure();          // In Pascals
        virtual float                  getOutsideAirDensity();           // Related to pressure and temp by the equation of state for air 
        virtual float                  getForwardSpeedRelToWind();
        virtual float                  getElevationASL();                // Elevation (in meters) above mean sea level

        virtual void                   setCurrentTime(           long int currentTime            );
        virtual void                   setOutsideTemp(           float    outsideTemp            );
        virtual void                   setOutsideAirPressure(    float    outsideAirPressure     );
        virtual void                   setForwardSpeedRelToWind( float    forwardSpeedRelToWind  );
        virtual void                   setElevationASL(          float    elevationASL           );


    private:

        long int                       _currentTime;
        float                          _outsideTemp;
        float                          _outsideAirPressure;
        float                          _forwardSpeedRelToWind;
        float                          _elevationASL;

};



#endif // __EXTERNALENVIRONSTATE_HH__
