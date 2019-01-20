/**************************************************************************/
/*!
    @file     UpdateALTAIRState.cc
    @author1  Justin Albert (jalbert@uvic.ca)
    @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
    @author3  Aravind Shaj  (aravindshaj@gmail.com)
    @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
    @license  GPL

    This is the class containing 

    @section  HISTORY

    v1.0  - First version
*/
/**************************************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
using namespace std;

//-----------------------
// This Class's Header --
//-----------------------

#include "UpdateALTAIRState.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

#include "../state/ALTAIR_state.hh"
#include "../state/ExternalEnvironState.hh"
#include "../state/BalloonAndPfoilState.hh"
#include "../state/GondolaAndPropState.hh"
#include "../state/OptSourcePayloadState.hh"

//              ----------------------------------------
//              -- Public Member Function Definitions --
//              ----------------------------------------
//
//
void UpdateALTAIRState::initializeState()
{
        ifstream               altairDataFile                                  ;
        float                  altairDataElement                               ;
        string                 altairRawData     , altairDataElemStr           ;
        string                 delimiter      = " "                            ;
        size_t                 pos            = 0, elemID  = 0                 ;
	ExternalEnvironState*  extEnv 	      = new ExternalEnvironState       ;
	BalloonAndPfoilState*  balAndPfoil    = new BalloonAndPfoilState       ;
        GondolaAndPropState*   gondAndProp    = new GondolaAndPropState        ;
        OptSourcePayloadState* optSource      = new OptSourcePayloadState      ;
        altairState->setExtEnv(                     extEnv                )    ;
        altairState->setBalAndPfoil(                balAndPfoil           )    ;
        altairState->setGondAndProp(                gondAndProp           )    ;
        altairState->setOptSource(                  optSource             )    ;

        altairDataFile.open("./predict/altairdata.txt"        )                ;
        getline(            altairDataFile    , altairRawData )                ;

        while ((pos = altairRawData.find(delimiter)) != string::npos) {
            altairDataElemStr = altairRawData.substr(0, pos)                   ;
            altairDataElement = stof(altairDataElemStr)                        ;
            setALTAIRDataElement(elemID, altairDataElement)                    ;
            altairRawData.erase(0, pos + delimiter.length())                   ;
            elemID++                                                           ;
        }

	return;
}

void UpdateALTAIRState::doUpdate( float newLat, float newLon, float newAlt )
{
	ExternalEnvironState*  extEnv 	      = altairState->getExtEnv()       ;
	BalloonAndPfoilState*  balAndPfoil    = altairState->getBalAndPfoil()  ;
        GondolaAndPropState*   gondAndProp    = altairState->getGondAndProp()  ;
        OptSourcePayloadState* optSource      = altairState->getOptSource()    ;

        extEnv->setCurrentLat(   newLat );
        extEnv->setCurrentLon(   newLon );
        extEnv->setElevationASL( newAlt );

/*
        if (newAlt >  25000) {
            extEnv->setOutsideTemp(-131.21 + 0.00299 * newAlt);
//            extEnv->setOutsideAirPressure(    2488.  * pow(( 0.00299 * newAlt + 141.89)/216.60, -11.388));
        }
        if (newAlt <= 25000 && newAlt > 11000) {
            extEnv->setOutsideTemp( -56.46                   );
//            extEnv->setOutsideAirPressure(   22650.  * exp(1.73-0.000157*newAlt));
        }
        if (newAlt <= 11000) {
            extEnv->setOutsideTemp(  15.04 - 0.00649 * newAlt);
//            extEnv->setOutsideAirPressure(  101290.  * pow((-0.00649 * newAlt + 288.14)/288.08,   5.256));
        }
*/

	return;
}



//              ----------------------------------------
//              -- Private Member Function Definitions --
//              ----------------------------------------
//
//
void UpdateALTAIRState::setALTAIRDataElement( size_t  elemID ,
                                              float   altairDataElement )
{
        size_t                 variableID     = elemID                         ;
	ExternalEnvironState*  extEnv 	      = altairState->getExtEnv()       ;
	BalloonAndPfoilState*  balAndPfoil    = altairState->getBalAndPfoil()  ;
        GondolaAndPropState*   gondAndProp    = altairState->getGondAndProp()  ;
        OptSourcePayloadState* optSource      = altairState->getOptSource()    ;

        if (variableID < ExternalEnvironState::numDataElements) {
            extEnv->setVariable(variableID, altairDataElement);
            return;
        } else {
            variableID -= ExternalEnvironState::numDataElements;
        }

        if (variableID < BalloonAndPfoilState::numDataElements) {
            balAndPfoil->setVariable(variableID, altairDataElement);
            return;
        } else {
            variableID -= BalloonAndPfoilState::numDataElements;
        }

        if (variableID < GondolaAndPropState::numDataElements) {
            gondAndProp->setVariable(variableID, altairDataElement);
            return;
        } else {
            variableID -= GondolaAndPropState::numDataElements;
        }

        if (variableID < OptSourcePayloadState::numDataElements) {
            optSource->setVariable(variableID, altairDataElement);
            return;
        } else {
            variableID -= OptSourcePayloadState::numDataElements;
        }

        return;  // really should be an error at this point ...
}
