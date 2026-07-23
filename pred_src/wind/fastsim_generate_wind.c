
// --------------------------------------------------------------
// Written by Justin Albert <jalbert@uvic.ca>
//
// THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
// --------------------------------------------------------------

#include "fastsim_generate_wind.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

// As per the usual input to and output from get_wind in the full simulation:
// lat and lng are respectively in (float decimal) degrees N of the equator (range [-90.:90.]) and degrees E of the prime meridian (range [0.:360.))
// alt is in (float decimal) m above mean sea level
// timestamp is in (long integer) seconds since the start of the Unix Epoch, i.e. since midnight UTC on Jan. 1, 1970
// *wind_u and *wind_v are, respectively, in m/s eastward and northward
// *wind_var is in m/s
// *pres is in pascals
// *temp is in celsius
// *windz is in Pa/s (rather than in m/s!!! -- would need to multiply by m/Pa (= d(alt)/d(pres)) to get m/s -- see e.g. line 141 in run_model_fastsim.cc)

int 
fastsim_generate_wind(float lat, float lng, float alt, long int timestamp,
                      float* wind_u, float* wind_v, float *wind_var, float *pres, float *temp, float *windz)
{
        *wind_u = 0.;
        *wind_v = 1.;
        *wind_var = 0.;
        *pres = 101250.*exp(-alt/8800.); // an isothermal atmosphere model with scale height (R T/mu g) of 8.8 km
        *temp = 0.;
        *windz = 0.;

        return 0;
}


