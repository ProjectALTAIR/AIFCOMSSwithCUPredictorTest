/**************************************************************************/
/*!
        @file     BalloonAndPfoilState.hh
        @author1  Justin Albert (jalbert@uvic.ca)
        @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
        @author3  Aravind Shaj  (aravindshaj@gmail.com)
        @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
        @license  GPL

        This is the class containing the present state of the ALTAIR
        parafoil, balloon, rigging, and helium bleed valve.  
    
        An example usage:
            #include "ALTAIR_state.hh"
            #include "BalloonAndPfoilState.hh"

            bool isCutdown = altairState->getBalAndPfoil()->getIsCutdown();

        Justin Albert  jalbert@uvic.ca            first version 7 Jul. 2018

        @section  HISTORY

        v1.0  - First version
*/
/**************************************************************************/


#ifndef __BALLOONANDPFOILSTATE_HH__
#define __BALLOONANDPFOILSTATE_HH__

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//              ---------------------
//              -- Class Interface --
//              ---------------------
class   BalloonAndPfoilState {

    public:

        BalloonAndPfoilState();

        virtual ~BalloonAndPfoilState();

        virtual bool                   getIsCutdown();

        virtual void                   setIsCutdown(    bool  isCutdown  );


    private:

        bool                           _isCutdown;

};



#endif // __BALLOONANDPFOILSTATE_HH__
