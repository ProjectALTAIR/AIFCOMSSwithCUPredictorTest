/**************************************************************************/
/*!
    @file     DragCalcMethods.cc
    @author1  Justin Albert (jalbert@uvic.ca)
    @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
    @author3  Aravind Shaj  (aravindshaj@gmail.com)
    @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
    @license  GPL

    This is the class containing drag calculation methods to determine the
    magnitude (in Newtons) of the drag force acting on the ALTAIR
    payload at a current instant, in terms of variables such as: 1) ALTAIR's
    current elevation above mean sea level, 2) balloon volume, pressure, and
    internal temp, and 3) current temperature and density.

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

#include "DragCalcMethods.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

#include "../state/ALTAIR_state.hh"
#include "../state/ExternalEnvironState.hh"
#include "../state/BalloonAndPfoilState.hh"
#include "../state/GondolaAndPropState.hh"
#include "BalloonPropertiesCalcMethods.hh"

//              ----------------------------------------
//              -- Public Member Function Definitions --
//              ----------------------------------------
//
// Gets the drag in Newtons affecting the balloon, at the current altitude 
// and at a given speed relative to the wind in some direction, calculated 
// based on the current size of the balloon.  The balloon's current size is 
// determined using the ideal gas law PV=nRT.
//
float DragCalcMethods::getBalloonDrag( float balloonSpeed )
{
	ExternalEnvironState* extEnv 	  = altairState->getExtEnv()					;

//	float payloadSpeed		  = extEnv->getForwardSpeedRelToWind()				;
	float tempInK			  = extEnv->getOutsideTemp() + kelvinMinusCelsius 		;
	float airDensity		  = extEnv->getOutsideAirDensity()				;
	float elevationASL		  = extEnv->getElevationASL()					;
	float pressure 			  = extEnv->getOutsideAirPressure()				;
	float balloonRadius  		  = BalloonPropertiesCalcMethods::getBalloonRadius()            ;
	
        // The formula below approximates dynamic viscosity in SI units (poiseuilles, i.e. kg / (m s) or Pa s).
        // Note that the dynamic viscosity of gases (somewhat surprisingly!) is largely independent of 
        // pressure, and thus is only given as a function of temperature.
	float airViscosity 		  = ( 1.458e-6 * sqrt(tempInK) ) / ( 1. + 110.4/tempInK )	;  
	float densityOverViscosity 	  = airDensity / airViscosity					;
	float crossSecArea   		  = M_PI * balloonRadius * balloonRadius     			;
	float reynoldsNumber 		  = densityOverViscosity * balloonRadius * 2. * balloonSpeed	;

	// This equation comes from curve fitting and is sourced from: http://pages.mtu.edu/~fmorriso/DataCorrelationForSphereDrag2016.pdf
	float dragCoef       		  = 24/reynoldsNumber + 2.6*(reynoldsNumber/5)/(1+pow((reynoldsNumber/5),1.52)) + 0.411*pow(reynoldsNumber/2.63e5,-7.94)
							/(1+pow(reynoldsNumber/2.63e5,-8)) + 0.25*(reynoldsNumber/1e6)/(1+(reynoldsNumber/1e6));

	return ( dragCoef * crossSecArea * airDensity * balloonSpeed * balloonSpeed / 2. )		;
}


// Returns drag in Newtons due to the ALTAIR parafoil at a given speed relative to the wind in some direction. 
// Note that this function returns a dummy value that is likely way off, depending on the current wind speed,
// density, and other external variables at ALTAIR's current position. 
//
float DragCalcMethods::getParafoilDrag( float parafoilSpeed )
{
	ExternalEnvironState* extEnv 	  = altairState->getExtEnv()                        ;
	BalloonAndPfoilState* balAndPfoil = altairState->getBalAndPfoil()                   ;

	float airDensity       = extEnv->getOutsideAirDensity()                             ;
	float crossSecArea     = balAndPfoil->getParafoilArea()                             ;
        float dragCoef         = 5.5225                                                     ;
	
        return dragCoef;
//	return ( dragCoef * crossSecArea * airDensity * parafoilSpeed * parafoilSpeed / 2. ) ;
}


// Returns drag in Newtons affecting the ALTAIR gondola at a given speed relative to the wind in some direction. 
// Note that this function returns a dummy value that is likely way off, depending on the current wind speed,
// density, and other external variables at ALTAIR's current position. 
//
float DragCalcMethods::getGondolaDrag( float gondolaSpeed )
{
	ExternalEnvironState* extEnv 	  = altairState->getExtEnv()			    ;
	GondolaAndPropState*  gondAndProp = altairState->getGondAndProp()		    ;

//	float payloadSpeed     = extEnv->getForwardSpeedRelToWind()      		    ;
	float airDensity       = extEnv->getOutsideAirDensity()          		    ;
	float crossSecArea     = gondAndProp->getGondolaXSecArea() 			    ;
	float dragCoef         = 2.05			 	                            ;
	
	return ( dragCoef * crossSecArea * airDensity * gondolaSpeed * gondolaSpeed / 2. )  ;
}

// The below two methods are good approximations, respectively, for spheres and for rectangles perpendicular to
// airfow, both at high Reynolds numbers, which will effectively always be the case in every calculation we do.
float DragCalcMethods::getApproxEffSphericalDragCoef()
{
        float c_d             = 0.47                                                        ;
        return sqrt(M_PI * c_d / (2. * gravConstantLittleg))                                ;
}

float DragCalcMethods::getApproxEffRectangDragCoef()
{
        float c_d             = 1.28                                                        ;
        return sqrt(       c_d / (2. * gravConstantLittleg))                                ;
}
