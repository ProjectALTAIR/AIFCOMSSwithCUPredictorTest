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

#define ALTAIRPropulsiveEfficiency     0.70     // In actuality this will be a function of the
                                                // specific operating conditions at the time,
                                                // but we will use 80% as an estimate for now.
//              ---------------------
//              -- Class Interface --
//              ---------------------
class PropulsionUtils {

    public:

	static void  getPropVelocityVector(float *prop_u, float *prop_v, float *prop_z);  // in m/s

        static float getExpendedPower(                                                );  // in watts

        static int   setSpeedTo(           float  speed                               );  // speed should be in m/s
        static int   goFullSpeedAhead(                                                );  // go as fast as one can go without completely emptying the batteries

    protected:

    private:

        PropulsionUtils() {}                    // constructor should never be used
	
};

#endif // __PROPULSIONUTILS_HH__
