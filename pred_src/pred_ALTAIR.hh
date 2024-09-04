// --------------------------------------------------------------
// CU Spaceflight Landing Prediction
// Copyright (c) CU Spaceflight 2009, All Right Reserved
//
// Written by Rob Anderson 
// Modified by Fergus Noble
// Modified by Justin Albert
//
// THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
// --------------------------------------------------------------

#ifndef __PRED_ALTAIR_HH__
#define __PRED_ALTAIR_HH__

#define VERSION "0.0.1"

extern FILE* output;
// extern FILE* myoutput;
extern FILE* kml_file;
extern const char* data_dir;

// start and finish KML files, basically just write header and footer in
void start_kml();
void finish_kml();

#endif // __PRED_ALTAIR_HH__


