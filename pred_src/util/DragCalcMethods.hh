/**************************************************************************/
/*!
    @file     DragCalcMethods.hh
    @author1  Justin Albert (jalbert@uvic.ca)
    @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
    @author3  Aravind Shaj  (aravindshaj@gmail.com)
    @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
    @license  GPL

    This is the class containing drag calculation methods to determine the
    magnitude (in Newtons) of the drag force acting on the ALTAIR
    payload at a current instant, in terms of variables such as: 1) ALTAIR's
    current elevation above mean sea level, 2) balloon volume, pressure, and 
    internal temp, and 3) current temperature and density.

    @section  HISTORY

    v1.0  - First version
*/
/**************************************************************************/

#ifndef __DRAGCALCMETHODS_HH__
#define __DRAGCALCMETHODS_HH__

//              ---------------------
//              -- Class Interface --
//              ---------------------

class DragCalcMethods {

    public:

        static float getBalloonDrag();		// get the present drag due to the balloon
	
        static float getGondolaDrag();		// currently only returns a dummy value for drag on the ALTAIR gondola

    private:

        DragCalcMethods() {} 			// constructor should never be used
};



#endif // __DRAGCALCMETHODS_HH__

