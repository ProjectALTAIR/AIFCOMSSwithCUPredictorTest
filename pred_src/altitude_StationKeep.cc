// --------------------------------------------------------------
// CU Spaceflight Landing Prediction
// Copyright (c) CU Spaceflight 2009, All Right Reserved
//
// Written by Rob Anderson 
// Modified by Fergus Noble
//
// THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
// --------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "altitude_StationKeep.hh"
#include "run_model_StationKeep.hh"
#include "pred_StationKeep.hh"

#include "state/ALTAIR_state.hh"
#include "state/ExternalEnvironState.hh"
#include "state/BalloonAndPfoilState.hh"
#include "util/UpdateALTAIRState.hh"
#include "util/AscentAndBurstCalcMethods.hh"
#include "util/DragCalcMethods.hh"

// get density of atmosphere at a given altitude
// uses NASA model from http://www.grc.nasa.gov/WWW/K-12/airplane/atmosmet.html
// units of degrees celcius, metres, KPa and Kg/metre cubed
static float get_density(float altitude);

#define G 9.8

struct altitude_model_s
{
    float   burst_altitude;
    float   ascent_rate;
    float   drag_coeff;
    int     descent_mode;

    float   initial_alt;
    int     burst_time;
};

altitude_model_t*
altitude_model_new(int dec_mode, float burst_alt, float asc_rate, float drag_co) 
{
    altitude_model_t* self = (altitude_model_t*)malloc(sizeof(altitude_model_t));

    // this function doesn't do anything much at the moment
    // but will prove useful in the future
    
    self->burst_altitude = burst_alt;
    self->ascent_rate = asc_rate;
    self->drag_coeff = drag_co;
    self->descent_mode = dec_mode;

    return self;
}

void
altitude_model_free(altitude_model_t* self)
{
    if(!self)
        return;

    free(self);
}

int 
// altitude_model_get_altitude(altitude_model_t* self, int time_into_flight, float* alt) {
altitude_model_get_altitude(int time_into_flight, float* alt) {
    // TODO: this section needs some work to make it more flexible
    
    // time == 0 so setup initial altitude stuff
    if (time_into_flight == 0) {
       UpdateALTAIRState::initializeState();
       altairState->getBalAndPfoil()->setAreSolarPanelsInstalled(true);
       altairState->getBalAndPfoil()->setNumOfSolarPanels(3);
//       altairState->getBalAndPfoil()->setBalloonLatexMass(2.0);
//       altairState->getBalAndPfoil()->setBalloonUnstretchedRadius(1.02);
       altairState->getBalAndPfoil()->setBalloonLatexMass(3.0);
       altairState->getBalAndPfoil()->setBalloonHeliumMass(2.9);
       altairState->getBalAndPfoil()->setBalloonUnstretchedRadius(1.143);
//        self->initial_alt = *alt;
//        self->burst_time = (self->burst_altitude - self->initial_alt) / self->ascent_rate;
    }

    ExternalEnvironState* extEnv      = altairState->getExtEnv()                          ;
    BalloonAndPfoilState* balAndPfoil = altairState->getBalAndPfoil()                     ;
    
    float ascentRate = extEnv->getAscentRate();
    *alt += TIMESTEP * ascentRate;                                          

//    *alt += TIMESTEP * AscentAndBurstCalcMethods::getEquilibAscentRate();    // We want predicted, _not_ measured, ascent rate, so this is right.
                                                                             // But ultimately we should account for accel and decel here,
                                                                             // _not_ just have ascent rate of 5 and drag coeff of 5.5225,
                                                                             // have an accurate rather than estimated air density model, etc etc.

    if (AscentAndBurstCalcMethods::areWeAtBurstAltitude()) {                 // Comment these
        balAndPfoil->setIsCutdown(true);                                     // lines out if 
        extEnv->setAscentRate(0.);                                           // & when running
        fprintf(stderr, "INFO: My goodness, the balloon popped!!!\n");       // tests that 
        fflush(stderr);                                                      // require the balloon
    }                                                                        // to never burst.

    ascentRate       =   AscentAndBurstCalcMethods::getPresentAscentTerminalVelocity();    // Velocity is effectively always terminal.
//    ascentRate      += TIMESTEP * AscentAndBurstCalcMethods::getVerticalAcceleration();  // Ultimately we need an accurate rather than estimated air density model for this.
    if (balAndPfoil->getIsCutdown()) {
//        ascentRate   = -(DragCalcMethods::getParafoilDrag(5.))/sqrt(extEnv->getOutsideAirDensity());
        ascentRate   =   AscentAndBurstCalcMethods::getPresentDescentTerminalVelocity();   // Velocity is effectively always terminal.
    }
    extEnv->setAscentRate(ascentRate);         
/*
    // If we are not doing a descending mode sim then start going up
    // at out ascent rate. The ascent rate is constant to a good approximation.
    if (self->descent_mode == DESCENT_MODE_NORMAL) 
        if (time_into_flight <= self->burst_time) {
            *alt = self->initial_alt + time_into_flight*self->ascent_rate;
            return 1;
        }
    
    // Descent - just assume its at terminal velocity (which varies with altitude)
    // this is a pretty darn good approximation for high-ish drag e.g. under parachute
    // still converges to T.V. quickly (i.e. less than a minute) for low drag.
    // terminal velocity = -drag_coeff/sqrt(get_density(*alt));
    *alt += TIMESTEP * -self->drag_coeff/sqrt(get_density(*alt));
*/
    
    /*
    // Rob's method - is this just freefall until we reach terminal velocity?
	vertical_speed = -drag_coeff/sqrt(get_density(*alt));
	// TODO: inaccurate if still accelerating
	if (vertical_speed < previous_vertical_speed - G*TIMESTEP)    // still accelerating
		vertical_speed = previous_vertical_speed - G*TIMESTEP;
	if(vertical_speed > previous_vertical_speed + G*TIMESTEP)     // still accelerating
		vertical_speed = previous_vertical_speed + G*TIMESTEP;
	previous_vertical_speed = vertical_speed;
	*alt += TIMESTEP * vertical_speed;
	*/

    
//    if (*alt <= 0)
    if (*alt <= altairState->getExtEnv()->getTerrainHeight())
        return 0;

    return 1;
    
}

float get_density(float altitude) {
    
    float temp = 0.f, pressure = 0.f;
    
    if (altitude > 25000) {
        temp = -131.21 + 0.00299 * altitude;
        pressure = 2.488*pow((temp+273.1)/216.6,-11.388);
    }
    if (altitude <=25000 && altitude > 11000) {
        temp = -56.46;
        pressure = 22.65 * exp(1.73-0.000157*altitude);
    }
    if (altitude <=11000) {
        temp = 15.04 - 0.00649 * altitude;
        pressure = 101.29 * pow((temp + 273.1)/288.08,5.256);
    }
    
    return pressure/(0.2869*(temp+273.1));
}
