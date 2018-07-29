/**************************************************************************/
/*!
        @file     ALTAIR_state.hh
        @author1  Justin Albert (jalbert@uvic.ca)
        @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
        @author3  Aravind Shaj  (aravindshaj@gmail.com)
        @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
        @license  GPL

        This is the class containing links to the present state of the 
        ALTAIR payload, gondola, rigging, parafoil, and balloon; as well  
        as a link to the present state of all of ALTAIR's relevant 
        external environmental conditions.  
    
        An example usage:
            #include "ALTAIR_state.hh"
            #include "GondolaAndPropState.hh"

            float rpmOfMotor4 = altairState->getGondAndProp()->getRPMMotor4();

        Justin Albert  jalbert@uvic.ca            first version 5 Jul. 2018

        @section  HISTORY

        v1.0  - First version
*/
/**************************************************************************/


#ifndef __ALTAIR_STATE_HH__
#define __ALTAIR_STATE_HH__

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class   ExternalEnvironState;
class   BalloonAndPfoilState;
class   GondolaAndPropState;
class   OptSourcePayloadState;

//              ---------------------
//              -- Class Interface --
//              ---------------------
class   ALTAIR_state {

    public:

        ALTAIR_state();

        virtual ~ALTAIR_state();

        virtual ExternalEnvironState*  getExtEnv();
        virtual BalloonAndPfoilState*  getBalAndPfoil();
        virtual GondolaAndPropState*   getGondAndProp();
        virtual OptSourcePayloadState* getOptSource();

        virtual void                   setExtEnv(         ExternalEnvironState*  statePointer  );
        virtual void                   setBalAndPfoil(    BalloonAndPfoilState*  statePointer  );
        virtual void                   setGondAndProp(    GondolaAndPropState*   statePointer  );
        virtual void                   setOptSource(      OptSourcePayloadState* statePointer  );


    private:

        ExternalEnvironState*          _externalEnvironState;
        BalloonAndPfoilState*          _balloonAndPfoilState;
        GondolaAndPropState*           _gondolaAndPropState;
        OptSourcePayloadState*         _optSourcePayloadState;

};

//------------------------------------------
// global pointer to present ALTAIR state --
//------------------------------------------
  
extern  ALTAIR_state*                  altairState;



#endif // __ALTAIR_STATE_HH__
