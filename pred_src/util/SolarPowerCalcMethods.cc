/**************************************************************************/
/*!
    @file     SolarPowerCalcMethods.cc
    @author1  Justin Albert (jalbert@uvic.ca)
    @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
    @author3  Aravind Shaj  (aravindshaj@gmail.com)
    @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
    @license  GPL

    This is the class containing the solar power calculation methods to
    determine the amount of solar power (in Watts) both generated by the
    solar panels and collected by the onboard batteries (i.e., accounting
    for, and thus subtracting, all known losses) at any given time.

    @section  HISTORY

    v1.0  - First version
*/
/**************************************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//-----------------------
// This Class's Header --
//-----------------------

#include "SolarPowerCalcMethods.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

#include "../state/ALTAIR_state.hh"
#include "../state/ExternalEnvironState.hh"
#include "../state/BalloonAndPfoilState.hh"
#include "../state/GondolaAndPropState.hh"
#include "BalloonPropertiesCalcMethods.hh"


#define    solarIntensityExternalToAtmosphere   1353.                                                                                       // in W/m^2


//              ----------------------------------------
//              -- Public Member Function Definitions --
//              ----------------------------------------
//
// This function returns the instantaneous solar power available from the solar panel array at a given time, location, and elevationASL
// of the ALTAIR balloon and payload.
//
// The equations for the air mass coefficient are determined by Dr. Albert so as to be useful for high elevations.
//
float SolarPowerCalcMethods::getInstantaneousSolarPower()
{
        const float         earthRadius                 = 6371.                                                                         ;   // in km
        const float         effTotalHeightOfAtm         =    8.577                                                                      ;   // in km

        std::vector<float>  solarAngles                 =     getZenithAndAzAnglesOfSunFromBalloon()                                    ;
        float               projectedAreaOfSolarPanels  =     getSolarPanelProjectedArea( solarAngles )                                 ;
        float               efficiencyOfSolarPanels     =     altairState->getBalAndPfoil()->getSolarPanelAvgEfficiency()               ;

        // Now we only need to know the intensity (W/m^2) of the solar radiation hitting ALTAIR.
        // For this, we will need to determine the airmass correction affecting the intensity of the solar radiation.

        float               zenithAngle                 =     solarAngles[0]                                                            ;

        float               elevationASL                =     altairState->getExtEnv()->getElevationASL() / 1000.                       ;   // in km
        float               effAirmassLength            =    0.                                                                         ;   // in km
        if (elevationASL < 30.) {
                            effAirmassLength            =     effTotalHeightOfAtm * pow( 1. - ( 0.03331 * elevationASL ), 3.5 )         ;
        }

        float               a                           =  (      earthRadius * effAirmassLength * cos(zenithAngle)  / 
                                                              ( effTotalHeightOfAtm * effTotalHeightOfAtm                       )    )  ;
        float               b                           =  ( 2. * earthRadius * effAirmassLength * effAirmassLength  /
                                                              ( effTotalHeightOfAtm * effTotalHeightOfAtm * effTotalHeightOfAtm )    )  ;
        float               yToXAtmosphereRatio         =     effAirmassLength / effTotalHeightOfAtm                                    ;
        float               airMassCoefficient          =     sqrt( ( a * a ) + b + ( yToXAtmosphereRatio * yToXAtmosphereRatio ) ) - a ;
        float               solarIrradiance             =     pow( ( 1. + airMassCoefficient ), 0.095                                ) *
                                                              solarIntensityExternalToAtmosphere                                       *
                                                              pow(   0.7                      , pow( airMassCoefficient, 0.678  )    )  ;

        return                                             (  solarIrradiance * projectedAreaOfSolarPanels * efficiencyOfSolarPanels )  ;
}


//
float SolarPowerCalcMethods::getSolarEnergyForNextMinute()
{
	return 2.5 		;
}

//              -------------------------------------------
//              -- Protected Member Function Definitions --
//              -------------------------------------------
//
//
float SolarPowerCalcMethods::getSolarPanelProjectedArea( std::vector<float> solarAngles )
{
        ExternalEnvironState* extEnv               =       altairState->getExtEnv()                                         ;
        BalloonAndPfoilState* balAndPfoilState     =       altairState->getBalAndPfoil()                                    ;

        // First, check if any solar panels are installed.  If no, then return 0.
        bool                  panelsInstalled      =       balAndPfoilState->getAreSolarPanelsInstalled()                   ;
        if                 ( !panelsInstalled         ) {  return 0.                                                        ; }

        // Next, check that the sun is above the horizon, i.e.: is the solar altitude angle greater than 0?
        float                 solarAltitudeAngle   =       90. - solarAngles[0]                                             ;
        float                 solarAzimuthAngle    =             solarAngles[1]                                             ;
        if                 (  solarAltitudeAngle < 0. ) {  return 0.                                                        ; }

        // Since solar panels are in fact installed, and the sun is in fact above the horizon, we can move forward 
        // with the calculation.
        int                   numSolarPanels       =       balAndPfoilState->getNumOfSolarPanels()                          ;
        float                 solarPanelWidth      =       balAndPfoilState->getIndivSolarPanelWidth()                      ;
        float                 solarPanelLength     =       balAndPfoilState->getIndivSolarPanelLength()                     ;
        float                 indivSolarPanelArea  =       solarPanelLength*solarPanelWidth                                 ;

        float                 balloonHeading       =       balAndPfoilState->getBalloonHeading()                            ;
        float                 angleBetweenPanels   =       360. / numSolarPanels                                            ;
        float                 setupUpperRadius     =       balAndPfoilState->getSolarPanelSetupUpperRadius()                ;
        float                 balloonRadius        =       BalloonPropertiesCalcMethods::getBalloonRadius()                 ;

        float                 verticalDistFromBalEquatorToPanelTop
                                                   =       sqrt((balloonRadius*balloonRadius)-(setupUpperRadius*setupUpperRadius))                              ;
        float                 angleOfPanelAreaVectorAboveHorizon
                                                   =       90. - (atan(verticalDistFromBalEquatorToPanelTop/(balloonRadius-setupUpperRadius))/radiansPerDegree) ;

        // Now, loop through each panel and determine the angle between the panel normal vector and the balloon-to-sun vector, 
        // and then the projected area of the panel.
        float                 totalProjectedArea   =       0.                                                               ;
        for ( int i = 0; i < numSolarPanels; i++ ) {

                // First, determine the vector orthogonal to a given solar panel.  (We will normalize that vector to 1.)
                float         panelNormVectorXMag  =       sin((balloonHeading+(i*angleBetweenPanels)) * radiansPerDegree ) ;
                float         panelNormVectorYMag  =       cos((balloonHeading+(i*angleBetweenPanels)) * radiansPerDegree ) ;
                float         panelNormVectorZMag  =       tan( angleOfPanelAreaVectorAboveHorizon     * radiansPerDegree ) ;

                float         panelNormVectorMag   =       sqrt( ( panelNormVectorXMag * panelNormVectorXMag ) + 
                                                                 ( panelNormVectorYMag * panelNormVectorYMag ) +
                                                                 ( panelNormVectorZMag * panelNormVectorZMag ) )            ;
                              panelNormVectorXMag /=       panelNormVectorMag                                               ;
                              panelNormVectorYMag /=       panelNormVectorMag                                               ;
                              panelNormVectorZMag /=       panelNormVectorMag                                               ;

                // Now, determine the balloon-to-sun vector.
                float         balToSunXMag         =       tan( solarAltitudeAngle * radiansPerDegree )                     ;
                float         balToSunYMag         =      -sin( solarAzimuthAngle  * radiansPerDegree )                     ;
                float         balToSunZMag         =  abs( cos( solarAzimuthAngle  * radiansPerDegree ) )                   ;

                float         currentLat           =       extEnv->getCurrentLat()                                          ;
                if          ( currentLat > 0. )  {         balToSunYMag       = -balToSunYMag                               ; }

                float         balToSunMag          =       sqrt( ( balToSunXMag * balToSunXMag ) + 
                                                                 ( balToSunYMag * balToSunYMag ) + 
                                                                 ( balToSunZMag * balToSunZMag ) )                          ;
                              balToSunXMag        /=       balToSunMag                                                      ;
                              balToSunYMag        /=       balToSunMag                                                      ;
                              balToSunZMag        /=       balToSunMag                                                      ;

                // Now we need the angle between the balToSun vector and the panelNormVector.
                // Since these two vectors are normalized to 1, cos(theta) = dotProductBetweenTheseVectors.
                float         dotProdBalToSunWithAreaVector           
                                                   =    (  balToSunXMag         * panelNormVectorXMag ) +
                                                        (  balToSunYMag         * panelNormVectorYMag ) +
                                                        (  balToSunZMag         * panelNormVectorZMag )                     ;
                float         projectedArea        =       0.                                                               ;
                if ( acos(dotProdBalToSunWithAreaVector) / radiansPerDegree < 90. ) {
                              projectedArea        =       indivSolarPanelArea * dotProdBalToSunWithAreaVector              ;
                }
                              totalProjectedArea  +=       projectedArea                                                    ;
        }
        return                totalProjectedArea                                                                            ;
}


// This method returns a 2-element vector containing (in element [0]) the zenith angle (i.e. the angle away from the zenith, in degrees),
// and (in element [1]) the azimuth angle (i.e. degrees East of true North), of the centre of the solar disc, as viewed from the balloon.
//
// This method uses the documentation on solar calculations from: http://holbert.faculty.asu.edu/eee463/SolarCalcs.pdf
// in order to determine the solarAltitudeAngle and the solarAzimuthAngle at a given time from a given lat/lon of the ALTAIR balloon.
//
// A note that this method relies on the current unix timestamp, and that starting in the year 2038 (at 3:14:08 UTC on 19 January) the 
// unixTime will overflow its 32 bits size and roll over to negatives, at which time this method will no longer be correct.
//
std::vector<float> SolarPowerCalcMethods::getZenithAndAzAnglesOfSunFromBalloon()
{
        const int               secondsInOneYear          = 31556926                                                      ;
        const int               secondsInOneDay           =    86400                                                      ;
        const int               minutesInOneDay           =     1440                                                      ;
        const float             minutesPerDegree          =        4.                                                     ;

        ExternalEnvironState*   extEnv                    = altairState->getExtEnv()                                      ;

        long int                unixTime                  = extEnv->getCurrentTime()                                      ;
        float                   currentLat                = extEnv->getCurrentLat()                                       ;
        float                   currentLon                = extEnv->getCurrentLon()                                       ;

        // First, we need the declination angle of the sun, which is given by: Gamma = 23.45*sin[((N+284)/365)*360]
        // where N is the day number of the year (with January 1 as N = 1).
        int                     secondsSinceThisJanFirst  = unixTime % secondsInOneYear                                   ;
        int                     dayNumber                 = secondsSinceThisJanFirst / secondsInOneDay                    ;
        float                   solarDeclinationAngle     = 23.45 * sin((( (float)dayNumber + 284. ) / 365.) * 2. * M_PI) ;

        // solarDeclinationAngle is the negative of the above equation, if in the southern hemisphere
        if (currentLat < 0)   { solarDeclinationAngle     = -solarDeclinationAngle                                        ; }

        // Next, we determine the hourAngle
        int                     minutesLaterThanUTC       =  currentLon * minutesPerDegree                                ;
        int                     minuteOfDay               = (((unixTime % secondsInOneDay) / 60) + minutesLaterThanUTC) %
                                                             minutesInOneDay                                              ;
        // mod by minutesInOneDay because it is possible with time change to have a later day locally than at UTC
        if (minuteOfDay < 0)  { minuteOfDay               =  minutesInOneDay + minuteOfDay                                ; }
        // In this case the local time is far enough behind UTC to be near midnight the prior day.

        float                   dEquationOfTime           = (360. / 365.) * radiansPerDegree * ((float)dayNumber - 81.)   ;
        float                   equationOfTimeCorrection  = (  9.87       * sin( 2 * dEquationOfTime ) ) - 
                                                            (  7.53       * cos(     dEquationOfTime ) ) -
                                                            (  1.5        * sin(     dEquationOfTime ) )                  ;
        float                   lonOfStandardTimeMeridian =   15.         * round(   currentLon / 15.  )                  ;
        float                   apparentSolarTime         =  minuteOfDay  +
                                                             ( minutesPerDegree * ( lonOfStandardTimeMeridian - currentLon ) ) +
                                                             equationOfTimeCorrection                                     ;
        // The subtraction of 720 minutes is because the hour angle is defined to be negative in the am and positive in the pm in this case.
        float                   hourAngle                 = ( apparentSolarTime - 720. ) / minutesPerDegree               ;

        // With the declination angle and hour angle, we now have enough information to determine the solar zenith angle and azimuth angle.
        float                   solarZenithAngle          =  acos((sin(abs(currentLat)*radiansPerDegree)*sin(solarDeclinationAngle*radiansPerDegree))+
                                                                  (cos(abs(currentLat)*radiansPerDegree)*cos(solarDeclinationAngle*radiansPerDegree)*cos(hourAngle*radiansPerDegree)))/radiansPerDegree;

        float                   solarAzimuthAngle         =  abs(acos(((cos(solarZenithAngle*radiansPerDegree)*sin(abs(currentLat)*radiansPerDegree))-sin(solarDeclinationAngle*radiansPerDegree))/
                                                                       (sin(solarZenithAngle*radiansPerDegree)*cos(abs(currentLat)*radiansPerDegree)))/radiansPerDegree);

    // Solar azimuth angle is defined as having the same sign as the hour angle (negative in am and positive in pm)
        if (hourAngle < 0)    { solarAzimuthAngle         = -solarAzimuthAngle                                            ; }

        std::vector<float>      solarAngles(2)                                                                            ;
                                solarAngles[0]            =  solarZenithAngle                                             ;
                                solarAngles[1]            =  solarAzimuthAngle                                            ;
        return                  solarAngles                                                                               ;
}

