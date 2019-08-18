/**************************************************************************/
/*!
    @file     StationKeepingFigureOfMerit.hh
    @license  GPL


    @section  HISTORY

    v1.0  - First version
*/
/**************************************************************************/

#ifndef __STATIONKEEPINGFIGUREOFMERIT_HH__
#define __STATIONKEEPINGFIGUREOFMERIT_HH__

//              ---------------------
//              -- Class Interface --
//              ---------------------
class StationKeepingFigureOfMerit {

    public:

	static float presentFigureOfMerit();
	static float presentdFoMdlat();                          // derivative with respect to latitude
	static float presentdFoMdlon();                          // derivative with respect to longitude
	static float presentdFoMdalt();                          // derivative with respect to altitude

    protected:

    private:

        StationKeepingFigureOfMerit() {}                         // constructor should never be used
	
};

#endif // __STATIONKEEPINGFIGUREOFMERIT_HH__
