/**************************************************************************/
/*!
        @file     GondolaAndPropState.hh
        @author1  Justin Albert (jalbert@uvic.ca)
        @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
        @author3  Aravind Shaj  (aravindshaj@gmail.com)
        @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
        @license  GPL

        This is the class containing the present state of the ALTAIR 
        gondola (and of all internals, except for all state variables 
        related to the light source payload) and of the ALTAIR propulsion 
        system.  
    
        An example usage:
            #include "ALTAIR_state.hh"
            #include "GondolaAndPropState.hh"

            float rpmOfMotor4 = altairState->getGondAndProp()->getRPMMotor4();

        Justin Albert  jalbert@uvic.ca            first version 7 Jul. 2018

        @section  HISTORY

        v1.0  - First version
*/
/**************************************************************************/


#ifndef __GONDOLAANDPROPSTATE_HH__
#define __GONDOLAANDPROPSTATE_HH__

#define ALTAIRGondolaTotalMass               3.0        // ** AN ESTIMATE FOR NOW **:  3.0 kg
#define ALTAIRGondolaXSecArea                0.2        // ** AN ESTIMATE FOR NOW **:  0.2 m^2

#define ALTAIRPropellerDiameter              0.35560    // 14.0" = 35.560 cm = 0.35560 m
#define ALTAIRPropellerPitch                 0.11938    //  4.7" = 11.938 cm = 0.11938 m
#define ALTAIRFullSpeedAheadPower          800.         // in Watts
#define ALTAIRFullSpeedPropRPM            6000

#define ALTAIRTotalBatteryStoredEnergy  215000.         // in Joules
#define ALTAIRMustTurnOffBelowBattFraction   0.20
#define ALTAIRCanTurnOnAboveBattFraction     0.30

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

#include <stdlib.h>

//              ---------------------
//              -- Class Interface --
//              ---------------------
class   GondolaAndPropState {

    public:

        GondolaAndPropState();

        virtual ~GondolaAndPropState();

        virtual float                  getGondolaTotalMass()    { return  ALTAIRGondolaTotalMass  ; }   // in kg

        virtual float                  getGondolaXSecArea()     { return  ALTAIRGondolaXSecArea   ; }   // in m^2

        virtual float                  getGondolaHeading()      { return _gondolaHeading          ; }   // Present ALTAIR heading in degrees East of true North (range is [  0.,360.) )
        virtual float                  getGondolaUpwardsTilt()  { return _gondolaUpwardsTilt      ; }   // Present ALTAIR upwards tilt in degrees above flat    (range is [-90., 90.] )

        virtual float                  getBatteryStoredEnergy() { return _batteryStoredEnergy     ; }   // the present stored energy in onboard batteries, in Joules (one fully-
                                                                                                        // charged 2700 mAh 11.1 V ALTAIR battery contains slightly over 100 kJ)
        virtual float                  getPropAxleRotAngle()    { return _propAxleRotAngle        ; }   // in degrees, ranging from (-180,180]

        virtual float                  getPropellerDiameter()   { return  ALTAIRPropellerDiameter ; }   // in m
        virtual float                  getPropellerPitch()      { return  ALTAIRPropellerPitch    ; }   // in m

        virtual int                    getPropRotDir(          int   motorNum                   ) ;     // Returns +1 if that propeller turns clockwise (when
                                                                                                        // viewed from above the gondola, if _propAxleRotAngle = 0),
                                                                                                        // and -1 if counter-clockwise.

        virtual int                    getRPMMotor(            int   motorNum                   ) ;     // motorNum goes from 1 to 4 (not 0 to 3)
        virtual int                    getRPMMotor1(                                            ) ;
        virtual int                    getRPMMotor2(                                            ) ;
        virtual int                    getRPMMotor3(                                            ) ;
        virtual int                    getRPMMotor4(                                            ) ;

        virtual float                  getCurrentMotor(        int   motorNum                   ) ;     // motorNum goes from 1 to 4 (not 0 to 3)
        virtual float                  getCurrentMotor1(                                        ) ;
        virtual float                  getCurrentMotor2(                                        ) ;
        virtual float                  getCurrentMotor3(                                        ) ;
        virtual float                  getCurrentMotor4(                                        ) ;

        static  const size_t           numDataElements                = 12                        ;
        virtual void                   setVariable(            size_t variableID ,
                                                               float  dataVariable              ) ;

        virtual void                   setGondolaHeading(      float gondolaHeading             ) ;     // in degrees East of true North (range is [  0.,360.) )
        virtual void                   setGondolaUpwardsTilt(  float gondolaUpwardsTilt         ) ;     // in degrees above flat         (range is [-90., 90.] )

        virtual void                   setBatteryStoredEnergy( float batteryStoredEnergy        ) ;     // in Joules

        virtual void                   setPropAxleRotAngle(    float propAxleRotAngle           ) ;     // in degrees, ranging from (-180,180]

        virtual void                   setRPMMotor(            int   motorNum  , 
                                                               int   rpmMotor                   ) ;     // motorNum goes from 1 to 4 (not 0 to 3)
        virtual void                   setRPMMotor1(           int   rpmMotor1                  ) ;
        virtual void                   setRPMMotor2(           int   rpmMotor2                  ) ;
        virtual void                   setRPMMotor3(           int   rpmMotor3                  ) ;
        virtual void                   setRPMMotor4(           int   rpmMotor4                  ) ;

        virtual void                   setCurrentMotor(        int   motorNum  , 
                                                               float currentMotor               ) ;     // motorNum goes from 1 to 4 (not 0 to 3)
        virtual void                   setCurrentMotor1(       float currentMotor1              ) ;
        virtual void                   setCurrentMotor2(       float currentMotor2              ) ;
        virtual void                   setCurrentMotor3(       float currentMotor3              ) ;
        virtual void                   setCurrentMotor4(       float currentMotor4              ) ;


    private:

#define             GONDOLAANDPROPSTATE_gondolaHeading            0
        float                          _gondolaHeading                                            ;
#define             GONDOLAANDPROPSTATE_gondolaUpwardsTilt        1
        float                          _gondolaUpwardsTilt                                        ;

#define             GONDOLAANDPROPSTATE_propAxleRotAngle          2
        float                          _propAxleRotAngle                                          ;   

#define             GONDOLAANDPROPSTATE_rpmMotor                  3
        int                            _rpmMotor[4]                                               ;
#define             GONDOLAANDPROPSTATE_currentMotor              7
        float                          _currentMotor[4]                                           ;     // current flowing into each motor + ESC combination, in Amps
#define             GONDOLAANDPROPSTATE_batteryStoredEnergy      11
        float                          _batteryStoredEnergy                                       ;     // in Joules

};



#endif // __GONDOLAANDPROPSTATE_HH__
