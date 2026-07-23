// --------------------------------------------------------------
//
// Written by Justin Albert <jalbert@uvic.ca>
//
// THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
// --------------------------------------------------------------

#ifndef __FASTSIM_GENERATE_WIND_H__
#define __FASTSIM_GENERATE_WIND_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Generate the wind values in the u and v directions at a point in space and time.
int fastsim_generate_wind(float lat, float lng, float alt, long int timestamp,
                          float* wind_u, float* wind_v, float *wind_var, float *pres, float *temp, float *windz);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __FASTSIM_GENERATE_WIND_H__

