/**************************************************************************/
/*!
    @file     PropulsionUtils.cc
    @author1  Justin Albert (jalbert@uvic.ca)
    @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
    @author3  Aravind Shaj  (aravindshaj@gmail.com)
    @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
    @license  GPL

    Justin Albert  jalbert@uvic.ca             started 26 Jan. 2019

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
#include "PropulsionUtils.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "../state/ALTAIR_state.hh"
#include "../state/ExternalEnvironState.hh"
#include "../state/GondolaAndPropState.hh"
#include "ThrustCalcMethods.hh"
#include "DragCalcMethods.hh"

//              ----------------------------------------
//              -- Public Member Function Definitions --
//              ----------------------------------------
//
void  PropulsionUtils::getPropVelocityVector(float *prop_u, float *prop_v, float *prop_z)
{
        GondolaAndPropState*  gondAndProp   = altairState->getGondAndProp()              ;

        float                 thrust        = ThrustCalcMethods::getInterpMethodThrust( );
        float                 terminalSpeed = DragCalcMethods::getTerminalSpeed( thrust );
        float                 upTilt        = (180. / M_PI) *
                                              gondAndProp->getGondolaUpwardsTilt(       );
        float                 heading       = (180. / M_PI) *
                                              gondAndProp->getGondolaHeading(           );

        *prop_v                             = cos(upTilt) * cos(heading) * terminalSpeed ;
        *prop_u                             = cos(upTilt) * sin(heading) * terminalSpeed ;
        *prop_z                             = sin(upTilt) *                terminalSpeed ;

}

float PropulsionUtils::getExpendedPower()
{
        float                 thrust        = ThrustCalcMethods::getInterpMethodThrust( );
        float                 terminalSpeed = DragCalcMethods::getTerminalSpeed( thrust );
        return              ( thrust * terminalSpeed / ALTAIRPropulsiveEfficiency       );
}

int   PropulsionUtils::setSpeedTo( float speed )
{
        GondolaAndPropState*  gondAndProp    = altairState->getGondAndProp()             ;

        float                 requiredThrust = DragCalcMethods::getThrustFromSpeed(speed);
        float                 requiredPower  = requiredThrust * speed / ALTAIRPropulsiveEfficiency ;
        int                   requiredRPM    = ThrustCalcMethods::getRPMFromThrustIntMethod(requiredThrust);
 
        if (       gondAndProp->getBatteryStoredEnergy() > ALTAIRCanTurnOnAboveBattFraction   * ALTAIRTotalBatteryStoredEnergy) {
          if (requiredPower <= ALTAIRFullSpeedAheadPower) {
            for (int i = 0; i < 4; ++i)
                   gondAndProp->setRPMMotor( i+1,  requiredRPM                )          ;
            return                             0                                         ;
          } else {
                              requiredThrust = DragCalcMethods::getThrustFromPower(ALTAIRFullSpeedAheadPower * ALTAIRPropulsiveEfficiency);
                              requiredRPM    = ThrustCalcMethods::getRPMFromThrustIntMethod(requiredThrust);
            for (int i = 0; i < 4; ++i)
                   gondAndProp->setRPMMotor( i+1,  requiredRPM                )          ;
            return                             1                                         ;
          }
        } else if (gondAndProp->getBatteryStoredEnergy() < ALTAIRMustTurnOffBelowBattFraction * ALTAIRTotalBatteryStoredEnergy) {
          for (int i = 0; i < 4; ++i) 
                   gondAndProp->setRPMMotor( i+1,  0                          )          ;
          return                              -1                                         ;
        }
        return                                 2                                         ;
}

int   PropulsionUtils::goFullSpeedAhead(       )
{
        GondolaAndPropState*  gondAndProp    = altairState->getGondAndProp()             ;

        float                 requiredThrust = DragCalcMethods::getThrustFromPower(ALTAIRFullSpeedAheadPower * ALTAIRPropulsiveEfficiency);
        float                 requiredRPM    = ThrustCalcMethods::getRPMFromThrustIntMethod(requiredThrust);

        if (       gondAndProp->getBatteryStoredEnergy() > ALTAIRCanTurnOnAboveBattFraction   * ALTAIRTotalBatteryStoredEnergy) {
          for (int i = 0; i < 4; ++i) 
                   gondAndProp->setRPMMotor( i+1, requiredRPM                 )          ;
          return                               0                                         ;
        } else if (gondAndProp->getBatteryStoredEnergy() < ALTAIRMustTurnOffBelowBattFraction * ALTAIRTotalBatteryStoredEnergy) {
          for (int i = 0; i < 4; ++i) 
                   gondAndProp->setRPMMotor( i+1,  0                          )          ;
          return                              -1                                         ;
        }
        return                                 2                                         ;
}
