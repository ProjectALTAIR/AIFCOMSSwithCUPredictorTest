/**************************************************************************/
/*!
    @file     TorqueCalcMethods.cc
    @author1  Justin Albert (jalbert@uvic.ca)
    @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
    @author3  Aravind Shaj  (aravindshaj@gmail.com)
    @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
    @license  GPL

    This is the class containing torque calculation methods to determine the
    amount (in Newton-meters) of torque acting on the ALTAIR gondola at a
    current instant, in terms of variables such as: 1) the RPM of each of the
    4 propellers; 2) the mass of the gondola, and 3) the mass of the balloon
    and its coupling to the gondola.

    Note that the calculation methods for the torque on the gondola along
    each of the 3 axes of rotation (azimuth, elevation, and sideways-roll)
    are static member functions of this class.  The interface to obtain
    the instantaneous torque along each of these axes is via these static
    member functions within this class.

    An example usage:
        #include "TorqueCalcMethods.hh"

        float torqueAzim = TorqueCalcMethods::getTorqueAzim();  // azimuthal torque
        float torqueElev = TorqueCalcMethods::getTorqueElev();  // elevational torque
        float torqueRoll = TorqueCalcMethods::getTorqueRoll();  // sideways-roll torque


    Justin Albert  jalbert@uvic.ca             initial version 14 Aug. 2018

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
#include "TorqueCalcMethods.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "ThrustCalcMethods.hh"
#include "../state/ALTAIR_state.hh"
#include "../state/ExternalEnvironState.hh"
#include "../state/BalloonAndPfoilState.hh"
#include "../state/GondolaAndPropState.hh"

//---------------
// Definitions --
//---------------


//              ----------------------------------------
//              -- Public Member Function Definitions --
//              ----------------------------------------
//
// Get the azimuthal torque (in Newton-meters):
// Add the aximuthal torque from each of the 4 propellers together, then add the mutual torque of the balloon 
// that acts on the gondola.
//
float TorqueCalcMethods::getTorqueAzim()
{
        GondolaAndPropState*  gondAndProp   = altairState->getGondAndProp()                  ;
        float                 azimTorque    = 0.                                             ;

// First, determine the torque from each propeller:
        float                 axleRotAng    = gondAndProp->getPropAxleRotAngle()             ;  // Ranges from (-180., +180.] , 0 degrees corresponds to an upward thrust axis,
                                                                                                // and +90. corresponds to a forward thrust axis.                                                  
        for (int i = 0; i < 4; ++i) {
                              azimTorque   += gondAndProp->getPropRotDir(i+1)         * 
                                                          getMotorTorque(i+1)         *
                                              cos( axleRotAng * radiansPerDegree )           ;
        }

// Then lastly, add the mutual torque between the balloon and gondola:
                              azimTorque   += getMutualTorque()                              ;

        return                azimTorque                                                     ;
}


// Get the elevational torque (in Newton-meters):
// The propellers will not create any elevational torque _directly_.   Elevational torque in world coordinates
// (rather than in gondola coordinates) will only be created by drag of the balloon counteracting the forward 
// (or reverse) thrust of the gondola propulsion.  There should be _no_ elevational torque in gondola coordinates.
// We will return 0. here, and the world-coordinate elevational torque will be calculated within the program loop.
//
float TorqueCalcMethods::getTorqueElev()
{
        return                0.                                                             ;
}


// Get the sideways-roll torque (in Newton-meters):
// Add the sideways-roll torque from each of the 4 propellers together.
//
float TorqueCalcMethods::getTorqueRoll()
{
        GondolaAndPropState*  gondAndProp   = altairState->getGondAndProp()                  ;
        float                 rollTorque    = 0.                                             ;

// First, determine the torque from each propeller:
        float                 axleRotAng    = gondAndProp->getPropAxleRotAngle()             ;  // Ranges from (-180., +180.] , 0 degrees corresponds to an upward thrust axis,
                                                                                                // and +90. corresponds to a forward thrust axis.
        for (int i = 0; i < 4; ++i) {
                              rollTorque   += gondAndProp->getPropRotDir(i+1)         * 
                                                          getMotorTorque(i+1)         *
                                              sin( axleRotAng * radiansPerDegree )           ;
        }
        return                rollTorque                                                     ;
}




float TorqueCalcMethods::getMutualTorque()
{
        ExternalEnvironState* extEnv       = altairState->getExtEnv()                        ;
        BalloonAndPfoilState* balAndPfoil  = altairState->getBalAndPfoil()                   ;

        float                 headingDiff  = balAndPfoil->getBalloonHeading() - 
                                                  extEnv->getCurrentHeading()                ;

// Now, estimate the mutual torque as a function of the heading difference headingDiff.
// In the below *very* rough estimation, we will just assume that if headingDiff is 
// _any_ positive value, the mutual torque will be +1 N-cm = +0.01 N-m, and if  
// headingDiff is _any_ negative value, the mutual torque will be -1 N-cm = -0.01 N-m.

        float                 mutualTorque =  0.                                             ;
        if (headingDiff > 0.) mutualTorque =  0.01                                           ;
        if (headingDiff < 0.) mutualTorque = -0.01                                           ;

        return                mutualTorque                                                   ;
}

// There are several ways to calculate the torque of an aircraft motor.
//
// 1) Perhaps the simplest is: The torque Q (in Newton-meters) = (the motor current I [in amps]) * (the motor constant Kt [in N-m/A])
//    together with: The motor constant Kt (in N-m/A) = 60 / ( 2 * pi * the motor constant kv [in rpm/V] )  =>
//    Q (in Newton-meters) = ( 30 * the motor current I [in amps] ) / ( pi * the motor constant kv [in rpm/V] ) 
//    This is from: https://ruor.uottawa.ca/bitstream/10393/36011/1/CSME%20%2806.29.16%29%20-%20Dominic_Revised.pdf
//    The motor constant kv for the motors we use: Turnigy D2826-6 2200kv outrunner motors
//    (https://hobbyking.com/en_us/d2826-6-2200kv-outrunner-motor.html) is, of course, 2200.
//    Thus: Q  =  ( 30 * the motor current I [in amps] ) / ( pi * 2200 )  =  0.00434 * I .
//
// 2) Another way is to use the formula:
//     The torque Q (in Newton-meters)  =  C_Q * rho * n^2 * d^5 ,
//    (from http://web.mit.edu/16.unified/www/FALL/thermodynamics/notes/node86.html )
//    where C_Q is a dimensionless constant known as the torque coefficient (with value dependent on 
//    the propeller design), rho is the air density (in kg/m^3), n is the rotations per second, and d
//    is the propeller diameter (in m).  To approximate C_Q, we use the function
//    getTorqueCoefficientC_Q (as a function of the propeller advance ratio J, which we obtain from
//    the static method getAdvanceRatioJ in ThrustCalcMethods) below.
//    Note that we use these 14" diameter x 4.7" pitch propellers: 
//     https://hobbyking.com/en_us/multirotor-carbon-fiber-propeller-14x4-7-black-cw-ccw-2pcs.html
//
//
float TorqueCalcMethods::getMotorTorque(int propNum)
{
        ExternalEnvironState* extEnv        = altairState->getExtEnv()                       ;
        GondolaAndPropState*  gondAndProp   = altairState->getGondAndProp()                  ;

//    Using method 1) above:
        float                 method1Torque = 0.0434 * gondAndProp->getCurrentMotor(propNum) ;

//    Using method 2) above:
        float                 torqCoeffC_Q  = 
                        getTorqueCoefficientC_Q(ThrustCalcMethods::getAdvanceRatioJ(propNum));
        float                 rho           =  extEnv->getOutsideAirDensity()                ;
        float                 n             = (gondAndProp->getRPMMotor(propNum))/60.        ;   // rotations per *second*
        float                 d             =  gondAndProp->getPropellerDiameter()           ;

        float                 method2Torque = torqCoeffC_Q * rho * n * n * d * d * d * d * d ;   // = C_Q * rho * n^2 * d^5

//    Return the (unweighted) average of the two methods, until we have a better idea of what to do...
        return              ( method1Torque + method2Torque ) / 2.                           ;
}

//              -------------------------------------------
//              -- Protected Member Function Definitions --
//              -------------------------------------------
//
// Torque coefficient C_Q (unitless) ~= -0.088 J^2  +  0.044 J  +  0.0165
// where J is the (unitless) advance ratio of the propeller
// (an extrapolation from Figure 3-2 in http://dc-rc.org/pdf/Model%20Propellers%20Article.pdf , and
//  also *note* that the propeller power coefficient Cp is equal to the torque coefficient C_Q).
//
float TorqueCalcMethods::getTorqueCoefficientC_Q(float J)
{
        if (J >= 0 && J <= 0.7)                                                             // sanity check
        {
                return ((-0.088 * J * J) + (0.044 * J) + 0.0165);                           // quadratic interpolation, using the above formula
        }
        return 0.;
}

