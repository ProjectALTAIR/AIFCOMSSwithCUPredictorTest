/**************************************************************************/
/*!
        @file     OptSourcePayloadState.hh
        @author1  Justin Albert (jalbert@uvic.ca)
        @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
        @author3  Aravind Shaj  (aravindshaj@gmail.com)
        @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
        @license  GPL

        This is the class containing the present state of the ALTAIR 
        optical source payload, and of its onboard photodiode measurement.  
    
        An example usage:
            #include "ALTAIR_state.hh"
            #include "OptSourcePayloadState.hh"

            bool redLEDsAreOn = altairState->getOptSource()->getRedLEDsAreOn();

        Justin Albert  jalbert@uvic.ca            first version 7 Jul. 2018

        @section  HISTORY

        v1.0  - First version
*/
/**************************************************************************/


#ifndef __OPTSOURCEPAYLOADSTATE_HH__
#define __OPTSOURCEPAYLOADSTATE_HH__

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//              ---------------------
//              -- Class Interface --
//              ---------------------
class   OptSourcePayloadState {

    public:

        OptSourcePayloadState();

        virtual ~OptSourcePayloadState();

        virtual bool                   getRedLEDsAreOn();
        virtual bool                   getYellowLEDsAreOn();
        virtual bool                   getBlueLEDsAreOn();
        virtual bool                   getGreenLEDsAreOn();

        virtual void                   setRedLEDsAreOn(     bool  redLEDsAreOn    );
        virtual void                   setYellowLEDsAreOn(  bool  yellowLEDsAreOn );
        virtual void                   setBlueLEDsAreOn(    bool  blueLEDsAreOn   );
        virtual void                   setGreenLEDsAreOn(   bool  greenLEDsAreOn  );


    private:

        bool                           _redLEDsAreOn;
        bool                           _yellowLEDsAreOn;
        bool                           _blueLEDsAreOn;
        bool                           _greenLEDsAreOn;

};



#endif // __OPTSOURCEPAYLOADSTATE_HH__
