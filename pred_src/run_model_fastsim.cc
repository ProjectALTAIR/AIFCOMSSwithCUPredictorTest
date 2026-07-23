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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

extern "C" {
#include "wind/fastsim_generate_wind.h"
#include "util/random.h"
}

#include "run_model_fastsim.hh"
#include "pred_fastsim.hh"

#include "state/ALTAIR_state.hh"
#include "state/ExternalEnvironState.hh"
#include "state/GondolaAndPropState.hh"
// #include "util/ThrustCalcMethods.hh"
// #include "util/DragCalcMethods.hh"
#include "util/StationKeepingAlgorithms.hh"
#include "util/PropulsionUtils.hh"
#include "util/SolarPowerCalcMethods.hh"
#include "util/UpdateALTAIRState.hh"

extern int verbosity;

#define RADIUS_OF_EARTH 6371009.f

typedef struct model_state_s model_state_t;
struct model_state_s
{
    float               lat;
    float               lng;
    float               alt;
//    altitude_model_t   *alt_model;
    double              loglik;
};

// Get the distance (in metres) of one degree of latitude and one degree of
// longitude. This varys with height (not much grant you).
static void
_get_frame(float lat, float lng, float alt, 
        float *d_dlat, float *d_dlng)
{
    float theta, r;

    theta = 2.f * M_PI * (90.f - lat) / 360.f;
    r = RADIUS_OF_EARTH + alt;

    // See the differentiation section of
    // http://en.wikipedia.org/wiki/Spherical_coordinate_system

    // d/dv = d/dlat = -d/dtheta
    *d_dlat = (2.f * M_PI) * r / 360.f;

    // d/du = d/dlong = d/dphi
    *d_dlng = (2.f * M_PI) * r * sinf(theta) / 360.f;
}

static int 
_advance_one_timestep(wind_file_cache_t** cacheAddr, 
                      unsigned long delta_t,
                      unsigned long timestamp, unsigned long initial_timestamp,
                      unsigned int n_states, model_state_t* states,
                      float rmserror)
{
    unsigned int i;

    for(i=0; i<n_states; ++i)
    {
        float ddlat, ddlng;
        float wind_u, wind_v, wind_var, pres, temp, wind_z_Pas;
        float prop_u, prop_v, prop_z;
        float u_samp, v_samp, u_lik, v_lik;
        float prev_alt, prev_pres, wind_zvel = 0.;
        float battLevel;
        model_state_t* state = &(states[i]);

        prev_alt  = state->alt;

//        fprintf(stderr, "INFO: Advancing one timestep.\n"); fflush(stderr); // warning: the spew into py_log from uncommenting this line may cause disk space to fill up

// the below both updates and returns the altitude (it does more than just get it -- it updates it!)
//        if(!altitude_model_get_altitude(state->alt_model, 
        assert(!isnan(state->lat));
        assert(!isnan(state->alt));
        if(!altitude_model_get_altitude( 
                                        timestamp - initial_timestamp, &state->alt))
            return 0; // alt < 0; finished

        if(fastsim_generate_wind(state->lat, state->lng, state->alt, timestamp, &wind_u, &wind_v, &wind_var, &pres, &temp, &wind_z_Pas))
            return -1; // error

        PropulsionUtils::getPropVelocityVector(&prop_u, &prop_v, &prop_z);

        _get_frame(state->lat, state->lng, state->alt, &ddlat, &ddlng);

        // NOTE: it this really the right thing to be doing? - think about what
        // happens near the poles

        wind_var += rmserror * rmserror;

        assert(wind_var >= 0.f);

        battLevel  = altairState->getGondAndProp()->getBatteryStoredEnergy();
        battLevel -= delta_t * PropulsionUtils::getExpendedPower();
        if (battLevel < 0.) battLevel = 0.;

        //fprintf(stderr, "U: %f +/- %f, V: %f +/- %f\n",
        //        wind_u, sqrtf(wind_u_var),
        //        wind_v, sqrtf(wind_v_var));

        u_samp      = random_sample_normal(wind_u, wind_var, &u_lik);
        v_samp      = random_sample_normal(wind_v, wind_var, &v_lik);
        if (battLevel > 0.) {
            u_samp += prop_u;
            v_samp += prop_v;
        }
        //u_samp = wind_u;
        //v_samp = wind_v;

        state->lat += v_samp * delta_t / ddlat;
        state->lng += u_samp * delta_t / ddlng;

        prev_pres = altairState->getExtEnv()->getOutsideAirPressure();
        altairState->getExtEnv()->setOutsideAirPressure(pres);
        altairState->getExtEnv()->setOutsideTemp(temp);
// wind_z_Pas is in Pa/s rather than m/s: want to multiply by m/Pa (= d(alt)/d(pres)) to get m/s
        if (pres != prev_pres) wind_zvel = wind_z_Pas * (state->alt - prev_alt) / (pres - prev_pres);
// and make sure it is not a crazy value
        if (wind_zvel > 5. || wind_zvel < -5.) wind_zvel = 0.;
// add in any vertical speed from propulsion
        if (battLevel > 0.) wind_zvel += prop_z;
        state->alt += (wind_zvel * delta_t);
// add battery energy from solar panels
        if (battLevel > 0.) battLevel += delta_t * SolarPowerCalcMethods::getInstantaneousSolarPower();

        UpdateALTAIRState::doUpdate(state->lat, state->lng, state->alt, battLevel);

        state->loglik += (double)(u_lik + v_lik);
    }

    return 1; // OK, and continue
}

static int _state_compare_rev(const void* a, const void *b)
{
    model_state_t* sa = (model_state_t*)a;
    model_state_t* sb = (model_state_t*)b;

    // this returns a value s.t. the states will be sorted so that
    // the maximum likelihood state is at position 0.
    return sb->loglik - sa->loglik;
}

int run_model(wind_file_cache_t** cacheAddr, 
//            altitude_model_t* alt_model,
              float initial_lat, float initial_lng, float initial_alt,
              long int initial_timestamp, float rmswinderror) 
{
    model_state_t* states;
    const unsigned int n_states = 1;
    unsigned int i;

    states = (model_state_t*) malloc( sizeof(model_state_t) * n_states );

    for(i=0; i<n_states; ++i) 
    {
        model_state_t* state = &(states[i]);

        state->alt = initial_alt;
        state->lat = initial_lat;
        state->lng = initial_lng;
//        state->alt_model = alt_model;
        state->loglik = 0.f;
    }

    long int timestamp = initial_timestamp;
    
    int log_counter = 0; // only write position to output files every LOG_DECIMATE timesteps
    int r, return_code = 1;

    while(1)
    {
        if(log_counter%1000 == 0) { fprintf(stderr, "INFO: Am here in the run_model loop.\n"); fflush(stderr); }
        r = _advance_one_timestep(cacheAddr, TIMESTEP, timestamp, initial_timestamp, 
                                  n_states, states, rmswinderror);
        if (r == -1) // error getting wind. Save prediction, but emit error messages
            return_code = 0;

        if (r != 1) // 1 = continue
            break;

        // Sort the array of models in order of log likelihood. 
        qsort(states, n_states, sizeof(model_state_t), _state_compare_rev);

        // write the maximum likelihood state out.
        if (log_counter == LOG_DECIMATE) {
            write_position(states[0].lat, states[0].lng, states[0].alt, timestamp);
            log_counter = 0;
        }

        log_counter++;
        timestamp += TIMESTEP;

//        PropulsionUtils::goFullSpeedAhead();
        StationKeepingAlgorithms::doStationKeep( cacheAddr );
    }

    for(i=0; i<n_states; ++i) 
    {
        model_state_t* state = &(states[i]);
        write_position(state->lat, state->lng, state->alt, timestamp);
    }

    fprintf(stderr, "INFO: Final maximum log lik: %f (=%f)\n", 
            states[0].loglik, exp(states[0].loglik));

    free(states);

    return return_code;
}

void write_position(float lat, float lng, float alt, int timestamp) {
    float                 prop_v, prop_u, prop_z                                 ;

//    float                 thrust        = ThrustCalcMethods::getInterpMethodThrust( );
//    float                 terminalSpeed = DragCalcMethods::getTerminalSpeed( thrust );
//    float                 expPower      = thrust * terminalSpeed / ALTAIRPropulsiveEfficiency;

    GondolaAndPropState*  gondAndProp             = altairState->getGondAndProp();

    // the predictor uses 0<=lng<360; most other things expect -180<lng<=180
    if (lng > 180)
        lng -= 360;

    if (kml_file) {
        fprintf(kml_file, "%g,%g,%g\n", lng, lat, alt);
        if (ferror(kml_file)) {
          fprintf(stderr, "ERROR: error writing to KML file\n");
          exit(1);
        }
    }

//    fprintf(output, "%d,%g,%g,%g\n", timestamp, lat, lng, alt);
//    fprintf(output, "%d,%g,%g,%g,%g\n", timestamp, lat, lng, alt, altairState->getGondAndProp()->getBatteryStoredEnergy());
    PropulsionUtils::getPropVelocityVector(&prop_u, &prop_v, &prop_z);
    fprintf(output, "%11d,%4.4f,%4.4f,%7.1f,%6.0f,%6.3f,%6.3f,%6.3f,%4d,%4d,%4d,%4d\n", timestamp, lat, lng, alt, gondAndProp->getBatteryStoredEnergy(),
            prop_u, prop_v, prop_z, gondAndProp->getRPMMotor1(), gondAndProp->getRPMMotor2(), gondAndProp->getRPMMotor3(), gondAndProp->getRPMMotor4());
    fflush(output);
//    expPower = PropulsionUtils::getExpendedPower();
//    fprintf(output, "%11d,%4.4f,%4.4f,%7.1f,%6.0f,%6.3f,%6.3f,%6.3f,%4d,%4d,%4d,%4d,%6.3f,%6.3f,%6.3f\n", timestamp, lat, lng, alt, gondAndProp->getBatteryStoredEnergy(),
//            prop_u, prop_v, prop_z, gondAndProp->getRPMMotor1(), gondAndProp->getRPMMotor2(), gondAndProp->getRPMMotor3(), gondAndProp->getRPMMotor4(),
//            thrust, terminalSpeed, expPower);
    if (ferror(output)) {
      fprintf(stderr, "ERROR: error writing to CSV file\n");
      exit(1);
    }
}

// vim:sw=4:ts=4:et:cindent
