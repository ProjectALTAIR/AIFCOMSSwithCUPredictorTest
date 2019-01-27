/**************************************************************************/
/*!
    @file     PropulsionUtils.hh
    @author1  Justin Albert (jalbert@uvic.ca)
    @author2  Patrick Ryan  (patrick.ryan5787@gmail.com)
    @author3  Aravind Shaj  (aravindshaj@gmail.com)
    @author4  Hannah Swan   (hannah.swan.3.14@gmail.com)
    @license  GPL

    Justin Albert  jalbert@uvic.ca               started 26 Jan. 2019

    @section  HISTORY

    v1.0  - First version
*/
/**************************************************************************/


#ifndef __PROPULSIONUTILS_HH__
#define __PROPULSIONUTILS_HH__

//              ---------------------
//              -- Class Interface --
//              ---------------------
class PropulsionUtils {

    public:

	static void doUpdate(float *newLat, float *newLon, float *newAlt);  

    protected:


    private:

        PropulsionUtils() {}                    // constructor should never be used
	
};

#endif // __PROPULSIONUTILS_HH__
