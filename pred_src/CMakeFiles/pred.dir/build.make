# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /hepuser/jalbert/cusf-standalone-predictor-master/pred_src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /hepuser/jalbert/cusf-standalone-predictor-master/pred_src

# Include any dependencies generated for this target.
include CMakeFiles/pred.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/pred.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/pred.dir/flags.make

CMakeFiles/pred.dir/util/gopt.o: CMakeFiles/pred.dir/flags.make
CMakeFiles/pred.dir/util/gopt.o: util/gopt.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/hepuser/jalbert/cusf-standalone-predictor-master/pred_src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/pred.dir/util/gopt.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/pred.dir/util/gopt.o   -c /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/util/gopt.c

CMakeFiles/pred.dir/util/gopt.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pred.dir/util/gopt.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/util/gopt.c > CMakeFiles/pred.dir/util/gopt.i

CMakeFiles/pred.dir/util/gopt.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pred.dir/util/gopt.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/util/gopt.c -o CMakeFiles/pred.dir/util/gopt.s

CMakeFiles/pred.dir/util/gopt.o.requires:

.PHONY : CMakeFiles/pred.dir/util/gopt.o.requires

CMakeFiles/pred.dir/util/gopt.o.provides: CMakeFiles/pred.dir/util/gopt.o.requires
	$(MAKE) -f CMakeFiles/pred.dir/build.make CMakeFiles/pred.dir/util/gopt.o.provides.build
.PHONY : CMakeFiles/pred.dir/util/gopt.o.provides

CMakeFiles/pred.dir/util/gopt.o.provides.build: CMakeFiles/pred.dir/util/gopt.o


CMakeFiles/pred.dir/util/getdelim.o: CMakeFiles/pred.dir/flags.make
CMakeFiles/pred.dir/util/getdelim.o: util/getdelim.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/hepuser/jalbert/cusf-standalone-predictor-master/pred_src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/pred.dir/util/getdelim.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/pred.dir/util/getdelim.o   -c /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/util/getdelim.c

CMakeFiles/pred.dir/util/getdelim.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pred.dir/util/getdelim.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/util/getdelim.c > CMakeFiles/pred.dir/util/getdelim.i

CMakeFiles/pred.dir/util/getdelim.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pred.dir/util/getdelim.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/util/getdelim.c -o CMakeFiles/pred.dir/util/getdelim.s

CMakeFiles/pred.dir/util/getdelim.o.requires:

.PHONY : CMakeFiles/pred.dir/util/getdelim.o.requires

CMakeFiles/pred.dir/util/getdelim.o.provides: CMakeFiles/pred.dir/util/getdelim.o.requires
	$(MAKE) -f CMakeFiles/pred.dir/build.make CMakeFiles/pred.dir/util/getdelim.o.provides.build
.PHONY : CMakeFiles/pred.dir/util/getdelim.o.provides

CMakeFiles/pred.dir/util/getdelim.o.provides.build: CMakeFiles/pred.dir/util/getdelim.o


CMakeFiles/pred.dir/util/getline.o: CMakeFiles/pred.dir/flags.make
CMakeFiles/pred.dir/util/getline.o: util/getline.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/hepuser/jalbert/cusf-standalone-predictor-master/pred_src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/pred.dir/util/getline.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/pred.dir/util/getline.o   -c /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/util/getline.c

CMakeFiles/pred.dir/util/getline.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pred.dir/util/getline.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/util/getline.c > CMakeFiles/pred.dir/util/getline.i

CMakeFiles/pred.dir/util/getline.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pred.dir/util/getline.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/util/getline.c -o CMakeFiles/pred.dir/util/getline.s

CMakeFiles/pred.dir/util/getline.o.requires:

.PHONY : CMakeFiles/pred.dir/util/getline.o.requires

CMakeFiles/pred.dir/util/getline.o.provides: CMakeFiles/pred.dir/util/getline.o.requires
	$(MAKE) -f CMakeFiles/pred.dir/build.make CMakeFiles/pred.dir/util/getline.o.provides.build
.PHONY : CMakeFiles/pred.dir/util/getline.o.provides

CMakeFiles/pred.dir/util/getline.o.provides.build: CMakeFiles/pred.dir/util/getline.o


CMakeFiles/pred.dir/util/random.o: CMakeFiles/pred.dir/flags.make
CMakeFiles/pred.dir/util/random.o: util/random.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/hepuser/jalbert/cusf-standalone-predictor-master/pred_src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/pred.dir/util/random.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/pred.dir/util/random.o   -c /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/util/random.c

CMakeFiles/pred.dir/util/random.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pred.dir/util/random.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/util/random.c > CMakeFiles/pred.dir/util/random.i

CMakeFiles/pred.dir/util/random.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pred.dir/util/random.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/util/random.c -o CMakeFiles/pred.dir/util/random.s

CMakeFiles/pred.dir/util/random.o.requires:

.PHONY : CMakeFiles/pred.dir/util/random.o.requires

CMakeFiles/pred.dir/util/random.o.provides: CMakeFiles/pred.dir/util/random.o.requires
	$(MAKE) -f CMakeFiles/pred.dir/build.make CMakeFiles/pred.dir/util/random.o.provides.build
.PHONY : CMakeFiles/pred.dir/util/random.o.provides

CMakeFiles/pred.dir/util/random.o.provides.build: CMakeFiles/pred.dir/util/random.o


CMakeFiles/pred.dir/wind/wind_file_cache.o: CMakeFiles/pred.dir/flags.make
CMakeFiles/pred.dir/wind/wind_file_cache.o: wind/wind_file_cache.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/hepuser/jalbert/cusf-standalone-predictor-master/pred_src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/pred.dir/wind/wind_file_cache.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/pred.dir/wind/wind_file_cache.o   -c /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/wind/wind_file_cache.c

CMakeFiles/pred.dir/wind/wind_file_cache.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pred.dir/wind/wind_file_cache.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/wind/wind_file_cache.c > CMakeFiles/pred.dir/wind/wind_file_cache.i

CMakeFiles/pred.dir/wind/wind_file_cache.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pred.dir/wind/wind_file_cache.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/wind/wind_file_cache.c -o CMakeFiles/pred.dir/wind/wind_file_cache.s

CMakeFiles/pred.dir/wind/wind_file_cache.o.requires:

.PHONY : CMakeFiles/pred.dir/wind/wind_file_cache.o.requires

CMakeFiles/pred.dir/wind/wind_file_cache.o.provides: CMakeFiles/pred.dir/wind/wind_file_cache.o.requires
	$(MAKE) -f CMakeFiles/pred.dir/build.make CMakeFiles/pred.dir/wind/wind_file_cache.o.provides.build
.PHONY : CMakeFiles/pred.dir/wind/wind_file_cache.o.provides

CMakeFiles/pred.dir/wind/wind_file_cache.o.provides.build: CMakeFiles/pred.dir/wind/wind_file_cache.o


CMakeFiles/pred.dir/wind/wind_file.o: CMakeFiles/pred.dir/flags.make
CMakeFiles/pred.dir/wind/wind_file.o: wind/wind_file.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/hepuser/jalbert/cusf-standalone-predictor-master/pred_src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/pred.dir/wind/wind_file.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/pred.dir/wind/wind_file.o   -c /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/wind/wind_file.c

CMakeFiles/pred.dir/wind/wind_file.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pred.dir/wind/wind_file.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/wind/wind_file.c > CMakeFiles/pred.dir/wind/wind_file.i

CMakeFiles/pred.dir/wind/wind_file.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pred.dir/wind/wind_file.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/wind/wind_file.c -o CMakeFiles/pred.dir/wind/wind_file.s

CMakeFiles/pred.dir/wind/wind_file.o.requires:

.PHONY : CMakeFiles/pred.dir/wind/wind_file.o.requires

CMakeFiles/pred.dir/wind/wind_file.o.provides: CMakeFiles/pred.dir/wind/wind_file.o.requires
	$(MAKE) -f CMakeFiles/pred.dir/build.make CMakeFiles/pred.dir/wind/wind_file.o.provides.build
.PHONY : CMakeFiles/pred.dir/wind/wind_file.o.provides

CMakeFiles/pred.dir/wind/wind_file.o.provides.build: CMakeFiles/pred.dir/wind/wind_file.o


CMakeFiles/pred.dir/altitude.o: CMakeFiles/pred.dir/flags.make
CMakeFiles/pred.dir/altitude.o: altitude.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/hepuser/jalbert/cusf-standalone-predictor-master/pred_src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/pred.dir/altitude.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/pred.dir/altitude.o   -c /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/altitude.c

CMakeFiles/pred.dir/altitude.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pred.dir/altitude.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/altitude.c > CMakeFiles/pred.dir/altitude.i

CMakeFiles/pred.dir/altitude.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pred.dir/altitude.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/altitude.c -o CMakeFiles/pred.dir/altitude.s

CMakeFiles/pred.dir/altitude.o.requires:

.PHONY : CMakeFiles/pred.dir/altitude.o.requires

CMakeFiles/pred.dir/altitude.o.provides: CMakeFiles/pred.dir/altitude.o.requires
	$(MAKE) -f CMakeFiles/pred.dir/build.make CMakeFiles/pred.dir/altitude.o.provides.build
.PHONY : CMakeFiles/pred.dir/altitude.o.provides

CMakeFiles/pred.dir/altitude.o.provides.build: CMakeFiles/pred.dir/altitude.o


CMakeFiles/pred.dir/pred.o: CMakeFiles/pred.dir/flags.make
CMakeFiles/pred.dir/pred.o: pred.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/hepuser/jalbert/cusf-standalone-predictor-master/pred_src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/pred.dir/pred.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/pred.dir/pred.o   -c /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/pred.c

CMakeFiles/pred.dir/pred.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pred.dir/pred.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/pred.c > CMakeFiles/pred.dir/pred.i

CMakeFiles/pred.dir/pred.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pred.dir/pred.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/pred.c -o CMakeFiles/pred.dir/pred.s

CMakeFiles/pred.dir/pred.o.requires:

.PHONY : CMakeFiles/pred.dir/pred.o.requires

CMakeFiles/pred.dir/pred.o.provides: CMakeFiles/pred.dir/pred.o.requires
	$(MAKE) -f CMakeFiles/pred.dir/build.make CMakeFiles/pred.dir/pred.o.provides.build
.PHONY : CMakeFiles/pred.dir/pred.o.provides

CMakeFiles/pred.dir/pred.o.provides.build: CMakeFiles/pred.dir/pred.o


CMakeFiles/pred.dir/run_model.o: CMakeFiles/pred.dir/flags.make
CMakeFiles/pred.dir/run_model.o: run_model.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/hepuser/jalbert/cusf-standalone-predictor-master/pred_src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/pred.dir/run_model.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/pred.dir/run_model.o   -c /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/run_model.c

CMakeFiles/pred.dir/run_model.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pred.dir/run_model.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/run_model.c > CMakeFiles/pred.dir/run_model.i

CMakeFiles/pred.dir/run_model.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pred.dir/run_model.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/run_model.c -o CMakeFiles/pred.dir/run_model.s

CMakeFiles/pred.dir/run_model.o.requires:

.PHONY : CMakeFiles/pred.dir/run_model.o.requires

CMakeFiles/pred.dir/run_model.o.provides: CMakeFiles/pred.dir/run_model.o.requires
	$(MAKE) -f CMakeFiles/pred.dir/build.make CMakeFiles/pred.dir/run_model.o.provides.build
.PHONY : CMakeFiles/pred.dir/run_model.o.provides

CMakeFiles/pred.dir/run_model.o.provides.build: CMakeFiles/pred.dir/run_model.o


CMakeFiles/pred.dir/ini/iniparser.o: CMakeFiles/pred.dir/flags.make
CMakeFiles/pred.dir/ini/iniparser.o: ini/iniparser.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/hepuser/jalbert/cusf-standalone-predictor-master/pred_src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object CMakeFiles/pred.dir/ini/iniparser.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/pred.dir/ini/iniparser.o   -c /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/ini/iniparser.c

CMakeFiles/pred.dir/ini/iniparser.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pred.dir/ini/iniparser.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/ini/iniparser.c > CMakeFiles/pred.dir/ini/iniparser.i

CMakeFiles/pred.dir/ini/iniparser.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pred.dir/ini/iniparser.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/ini/iniparser.c -o CMakeFiles/pred.dir/ini/iniparser.s

CMakeFiles/pred.dir/ini/iniparser.o.requires:

.PHONY : CMakeFiles/pred.dir/ini/iniparser.o.requires

CMakeFiles/pred.dir/ini/iniparser.o.provides: CMakeFiles/pred.dir/ini/iniparser.o.requires
	$(MAKE) -f CMakeFiles/pred.dir/build.make CMakeFiles/pred.dir/ini/iniparser.o.provides.build
.PHONY : CMakeFiles/pred.dir/ini/iniparser.o.provides

CMakeFiles/pred.dir/ini/iniparser.o.provides.build: CMakeFiles/pred.dir/ini/iniparser.o


CMakeFiles/pred.dir/ini/dictionary.o: CMakeFiles/pred.dir/flags.make
CMakeFiles/pred.dir/ini/dictionary.o: ini/dictionary.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/hepuser/jalbert/cusf-standalone-predictor-master/pred_src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building C object CMakeFiles/pred.dir/ini/dictionary.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/pred.dir/ini/dictionary.o   -c /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/ini/dictionary.c

CMakeFiles/pred.dir/ini/dictionary.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pred.dir/ini/dictionary.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/ini/dictionary.c > CMakeFiles/pred.dir/ini/dictionary.i

CMakeFiles/pred.dir/ini/dictionary.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pred.dir/ini/dictionary.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/ini/dictionary.c -o CMakeFiles/pred.dir/ini/dictionary.s

CMakeFiles/pred.dir/ini/dictionary.o.requires:

.PHONY : CMakeFiles/pred.dir/ini/dictionary.o.requires

CMakeFiles/pred.dir/ini/dictionary.o.provides: CMakeFiles/pred.dir/ini/dictionary.o.requires
	$(MAKE) -f CMakeFiles/pred.dir/build.make CMakeFiles/pred.dir/ini/dictionary.o.provides.build
.PHONY : CMakeFiles/pred.dir/ini/dictionary.o.provides

CMakeFiles/pred.dir/ini/dictionary.o.provides.build: CMakeFiles/pred.dir/ini/dictionary.o


# Object files for target pred
pred_OBJECTS = \
"CMakeFiles/pred.dir/util/gopt.o" \
"CMakeFiles/pred.dir/util/getdelim.o" \
"CMakeFiles/pred.dir/util/getline.o" \
"CMakeFiles/pred.dir/util/random.o" \
"CMakeFiles/pred.dir/wind/wind_file_cache.o" \
"CMakeFiles/pred.dir/wind/wind_file.o" \
"CMakeFiles/pred.dir/altitude.o" \
"CMakeFiles/pred.dir/pred.o" \
"CMakeFiles/pred.dir/run_model.o" \
"CMakeFiles/pred.dir/ini/iniparser.o" \
"CMakeFiles/pred.dir/ini/dictionary.o"

# External object files for target pred
pred_EXTERNAL_OBJECTS =

pred: CMakeFiles/pred.dir/util/gopt.o
pred: CMakeFiles/pred.dir/util/getdelim.o
pred: CMakeFiles/pred.dir/util/getline.o
pred: CMakeFiles/pred.dir/util/random.o
pred: CMakeFiles/pred.dir/wind/wind_file_cache.o
pred: CMakeFiles/pred.dir/wind/wind_file.o
pred: CMakeFiles/pred.dir/altitude.o
pred: CMakeFiles/pred.dir/pred.o
pred: CMakeFiles/pred.dir/run_model.o
pred: CMakeFiles/pred.dir/ini/iniparser.o
pred: CMakeFiles/pred.dir/ini/dictionary.o
pred: CMakeFiles/pred.dir/build.make
pred: CMakeFiles/pred.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/hepuser/jalbert/cusf-standalone-predictor-master/pred_src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking C executable pred"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pred.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/pred.dir/build: pred

.PHONY : CMakeFiles/pred.dir/build

CMakeFiles/pred.dir/requires: CMakeFiles/pred.dir/util/gopt.o.requires
CMakeFiles/pred.dir/requires: CMakeFiles/pred.dir/util/getdelim.o.requires
CMakeFiles/pred.dir/requires: CMakeFiles/pred.dir/util/getline.o.requires
CMakeFiles/pred.dir/requires: CMakeFiles/pred.dir/util/random.o.requires
CMakeFiles/pred.dir/requires: CMakeFiles/pred.dir/wind/wind_file_cache.o.requires
CMakeFiles/pred.dir/requires: CMakeFiles/pred.dir/wind/wind_file.o.requires
CMakeFiles/pred.dir/requires: CMakeFiles/pred.dir/altitude.o.requires
CMakeFiles/pred.dir/requires: CMakeFiles/pred.dir/pred.o.requires
CMakeFiles/pred.dir/requires: CMakeFiles/pred.dir/run_model.o.requires
CMakeFiles/pred.dir/requires: CMakeFiles/pred.dir/ini/iniparser.o.requires
CMakeFiles/pred.dir/requires: CMakeFiles/pred.dir/ini/dictionary.o.requires

.PHONY : CMakeFiles/pred.dir/requires

CMakeFiles/pred.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/pred.dir/cmake_clean.cmake
.PHONY : CMakeFiles/pred.dir/clean

CMakeFiles/pred.dir/depend:
	cd /hepuser/jalbert/cusf-standalone-predictor-master/pred_src && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /hepuser/jalbert/cusf-standalone-predictor-master/pred_src /hepuser/jalbert/cusf-standalone-predictor-master/pred_src /hepuser/jalbert/cusf-standalone-predictor-master/pred_src /hepuser/jalbert/cusf-standalone-predictor-master/pred_src /hepuser/jalbert/cusf-standalone-predictor-master/pred_src/CMakeFiles/pred.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/pred.dir/depend

