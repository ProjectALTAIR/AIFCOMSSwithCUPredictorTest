// --------------------------------------------------------------
// CU Spaceflight Landing Prediction
// Copyright (c) CU Spaceflight 2009, All Right Reserved
//
// Written by Rich Wareham <rjw57@cam.ac.uk>
//
// THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
// --------------------------------------------------------------

#include "random.h"

// #include <glib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static double _randfrom(double min, double max) 
{
    double range = (max - min); 
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

// Sample from a normal distribution with zero mean and unit variance.
// See http://en.wikipedia.org/wiki/Normal_distribution
//                              #Generating_values_for_normal_random_variables
static float _random_sample_normal_intl(float* loglik)
{
    double u, v = 0.0;
    static const double k = 0.918938533204673; // = 0.5 * (log(2) + log(pi)), see below.

    // u = g_random_double();
    // v = g_random_double();
    u = _randfrom(0., 1.);
    v = _randfrom(0., 1.);
    // v = sqrt(-2.0 * log(u)) * cos(2.0 * G_PI * v);
    v = sqrt(-2.0 * log(u)) * cos(2.0 * M_PI * v);

    // actual likelihood is 1/sqrt(2*pi) exp(-(x^2)) since mu = 0 and sigma^2 = 1.
    // log-likelihood is therefore:
    //   \ell(x) = - (x^2) - 0.5 * (log(2) + log(pi)) = - (x^2) - k 

    if(loglik)
        *loglik = (float) ( - (v*v) - k );

    return v;
}

float random_sample_normal(float mu, float sigma2, float *loglik)
{
    // Sample from our base case.
    float v = _random_sample_normal_intl(loglik);

    // Transform into appropriate range.
    v *= sqrt(sigma2);
    v += mu;

    return v;
}

// vim:sw=4:ts=4:et:cindent
