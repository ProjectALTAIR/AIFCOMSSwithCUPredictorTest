# CMake build file for the CUSF landing prediction software
#
# Original version: Rich Wareham <rjw57@cam.ac.uk>
# This version: Justin Albert <jalbert@uvic.ca>

# Tested with CMake 3.24 -- but I think all versions 3.0 and above are fine for this.
cmake_minimum_required(VERSION 3.0)
# CMake requires having a project(ProjectName) line, for some reason (otherwise a warning is produced).
project(AIFCOMSS)

# Use PkgConfig to find the glib libraries
find_package(PkgConfig REQUIRED)

pkg_check_modules(GLIB REQUIRED glib-2.0)
# pkg_check_modules(deps REQUIRED IMPORTED_TARGET glib-2.0)
# target_link_libraries(pred PkgConfig::deps)
# target_link_libraries(pred_ALTAIR PkgConfig::deps)
# target_link_libraries(pred_StationKeep PkgConfig::deps)
# find_path(GLIB_INCLUDE_DIRS NAMES glib.h PATH_SUFFIXES glib-2.0)

include_directories(${GLIB_INCLUDE_DIRS})
link_directories(${GLIB_LIBRARY_DIRS})

add_executable(pred
	util/gopt.c
	util/getdelim.c
	util/random.h
	util/gopt.h
	util/getline.h
	util/getline.c
	util/getdelim.h
	util/random.c
	altitude.h
	wind/wind_file_cache.c
	wind/wind_file_cache.h
	wind/wind_file.c
	wind/wind_file.h
	altitude.c
	pred.c
	run_model.c
	pred.h
	run_model.h
	ini/iniparser.c
	ini/iniparser.h
	ini/dictionary.h
	ini/dictionary.c
)

add_executable(pred_ALTAIR
        state/ALTAIR_state.cc
        state/ALTAIR_state.hh
        state/ExternalEnvironState.cc
        state/ExternalEnvironState.hh
        state/BalloonAndPfoilState.cc
        state/BalloonAndPfoilState.hh
        state/GondolaAndPropState.cc
        state/GondolaAndPropState.hh
        state/OptSourcePayloadState.cc
        state/OptSourcePayloadState.hh
        util/BalloonPropertiesCalcMethods.cc
        util/BalloonPropertiesCalcMethods.hh
        util/ThrustCalcMethods.cc
        util/ThrustCalcMethods.hh
        util/DragCalcMethods.cc
        util/DragCalcMethods.hh
        util/SolarPowerCalcMethods.cc
        util/SolarPowerCalcMethods.hh
        util/AscentAndBurstCalcMethods.cc
        util/AscentAndBurstCalcMethods.hh
        util/HeliumLossCalcMethods.cc
        util/HeliumLossCalcMethods.hh
        util/PropulsionUtils.cc
        util/PropulsionUtils.hh
        util/UpdateALTAIRState.cc
        util/UpdateALTAIRState.hh
	util/gopt.c
	util/getdelim.c
	util/random.h
	util/gopt.h
	util/getline.h
	util/getline.c
	util/getdelim.h
	util/random.c
	altitude_ALTAIR.hh
	wind/wind_file_cache_ALTAIR.c
	wind/wind_file_cache_ALTAIR.h
	wind/wind_file_ALTAIR.c
	wind/wind_file_ALTAIR.h
	altitude_ALTAIR.cc
	pred_ALTAIR.cc
	run_model_ALTAIR.cc
	pred_ALTAIR.hh
	run_model_ALTAIR.hh
	ini/iniparser.c
	ini/iniparser.h
	ini/dictionary.h
	ini/dictionary.c
)

add_executable(pred_StationKeep
        state/ALTAIR_state.cc
        state/ALTAIR_state.hh
        state/ExternalEnvironState.cc
        state/ExternalEnvironState.hh
        state/BalloonAndPfoilState.cc
        state/BalloonAndPfoilState.hh
        state/GondolaAndPropState.cc
        state/GondolaAndPropState.hh
        state/OptSourcePayloadState.cc
        state/OptSourcePayloadState.hh
        util/BalloonPropertiesCalcMethods.cc
        util/BalloonPropertiesCalcMethods.hh
        util/ThrustCalcMethods.cc
        util/ThrustCalcMethods.hh
        util/DragCalcMethods.cc
        util/DragCalcMethods.hh
        util/SolarPowerCalcMethods.cc
        util/SolarPowerCalcMethods.hh
        util/AscentAndBurstCalcMethods.cc
        util/AscentAndBurstCalcMethods.hh
        util/HeliumLossCalcMethods.cc
        util/HeliumLossCalcMethods.hh
        util/PropulsionUtils.cc
        util/PropulsionUtils.hh
        util/StationKeepingFigureOfMerit.cc
        util/StationKeepingFigureOfMerit.hh
        util/StationKeepingAlgorithms.cc
        util/StationKeepingAlgorithms.hh
        util/UpdateALTAIRState.cc
        util/UpdateALTAIRState.hh
	util/gopt.c
	util/getdelim.c
	util/random.h
	util/gopt.h
	util/getline.h
	util/getline.c
	util/getdelim.h
	util/random.c
	altitude_StationKeep.hh
	wind/wind_file_cache_ALTAIR.c
	wind/wind_file_cache_ALTAIR.h
	wind/wind_file_ALTAIR.c
	wind/wind_file_ALTAIR.h
	altitude_StationKeep.cc
	pred_StationKeep.cc
	run_model_StationKeep.cc
	pred_StationKeep.hh
	run_model_StationKeep.hh
	ini/iniparser.c
	ini/iniparser.h
	ini/dictionary.h
	ini/dictionary.c
)

target_link_libraries(pred ${GLIB_LIBRARIES} -lm)

target_link_libraries(pred_ALTAIR ${GLIB_LIBRARIES} -lm)

target_link_libraries(pred_StationKeep ${GLIB_LIBRARIES} -lm)
