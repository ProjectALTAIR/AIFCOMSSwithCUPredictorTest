/**************************************************************************/
/*!
    @file     StationKeepingAlgorithms.hh
    @license  GPL


    @section  HISTORY

    v1.0  - First version
*/
/**************************************************************************/

#ifndef __STATIONKEEPINGALGORITHMS_HH__
#define __STATIONKEEPINGALGORITHMS_HH__

extern "C" {
#include "../wind/wind_file_cache_ALTAIR.h"
}

//              ---------------------
//              -- Class Interface --
//              ---------------------
class StationKeepingAlgorithms {

    public:

	static void  doStationKeep(wind_file_cache_t* cache);

    protected:

    private:

        StationKeepingAlgorithms() {}                                                     // constructor should never be used
	
};

#endif // __STATIONKEEPINGALGORITHMS_HH__
