# CMake build file for the CUSF landing prediction software
#
# Original version: Rich Wareham <rjw57@cam.ac.uk>

# Use PkgConfig to find the glib libraries
find_package(PkgConfig)

pkg_check_modules(GLIB REQUIRED glib-2.0)

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
	wind/wind_file_cache.c
	wind/wind_file_cache.h
	wind/wind_file.c
	wind/wind_file.h
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

target_link_libraries(pred ${GLIB_LIBRARIES} -lm)

target_link_libraries(pred_ALTAIR ${GLIB_LIBRARIES} -lm)
