# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/bele/CMqttSnForwarder

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bele/CMqttSnForwarder

# Include any dependencies generated for this target.
include external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/depend.make

# Include the progress variables for this target.
include external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/progress.make

# Include the compile flags for this target's objects.
include external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/flags.make

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fmkfs.c.o: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/flags.make
external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fmkfs.c.o: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fmkfs.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bele/CMqttSnForwarder/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fmkfs.c.o"
	cd /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/gadget114514_fmkfs.dir/fmkfs.c.o   -c /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fmkfs.c

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fmkfs.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gadget114514_fmkfs.dir/fmkfs.c.i"
	cd /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fmkfs.c > CMakeFiles/gadget114514_fmkfs.dir/fmkfs.c.i

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fmkfs.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gadget114514_fmkfs.dir/fmkfs.c.s"
	cd /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fmkfs.c -o CMakeFiles/gadget114514_fmkfs.dir/fmkfs.c.s

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fmkfs.c.o.requires:

.PHONY : external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fmkfs.c.o.requires

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fmkfs.c.o.provides: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fmkfs.c.o.requires
	$(MAKE) -f external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/build.make external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fmkfs.c.o.provides.build
.PHONY : external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fmkfs.c.o.provides

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fmkfs.c.o.provides.build: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fmkfs.c.o


external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/diskio.c.o: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/flags.make
external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/diskio.c.o: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/diskio.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bele/CMqttSnForwarder/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/diskio.c.o"
	cd /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/diskio.c.o   -c /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/diskio.c

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/diskio.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/diskio.c.i"
	cd /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/diskio.c > CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/diskio.c.i

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/diskio.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/diskio.c.s"
	cd /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/diskio.c -o CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/diskio.c.s

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/diskio.c.o.requires:

.PHONY : external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/diskio.c.o.requires

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/diskio.c.o.provides: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/diskio.c.o.requires
	$(MAKE) -f external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/build.make external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/diskio.c.o.provides.build
.PHONY : external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/diskio.c.o.provides

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/diskio.c.o.provides.build: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/diskio.c.o


external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ff.c.o: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/flags.make
external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ff.c.o: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/ff.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bele/CMqttSnForwarder/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ff.c.o"
	cd /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ff.c.o   -c /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/ff.c

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ff.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ff.c.i"
	cd /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/ff.c > CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ff.c.i

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ff.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ff.c.s"
	cd /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/ff.c -o CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ff.c.s

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ff.c.o.requires:

.PHONY : external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ff.c.o.requires

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ff.c.o.provides: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ff.c.o.requires
	$(MAKE) -f external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/build.make external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ff.c.o.provides.build
.PHONY : external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ff.c.o.provides

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ff.c.o.provides.build: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ff.c.o


external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffsystem.c.o: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/flags.make
external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffsystem.c.o: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/ffsystem.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bele/CMqttSnForwarder/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffsystem.c.o"
	cd /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffsystem.c.o   -c /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/ffsystem.c

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffsystem.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffsystem.c.i"
	cd /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/ffsystem.c > CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffsystem.c.i

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffsystem.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffsystem.c.s"
	cd /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/ffsystem.c -o CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffsystem.c.s

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffsystem.c.o.requires:

.PHONY : external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffsystem.c.o.requires

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffsystem.c.o.provides: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffsystem.c.o.requires
	$(MAKE) -f external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/build.make external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffsystem.c.o.provides.build
.PHONY : external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffsystem.c.o.provides

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffsystem.c.o.provides.build: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffsystem.c.o


external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffunicode.c.o: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/flags.make
external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffunicode.c.o: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/ffunicode.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bele/CMqttSnForwarder/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffunicode.c.o"
	cd /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffunicode.c.o   -c /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/ffunicode.c

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffunicode.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffunicode.c.i"
	cd /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/ffunicode.c > CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffunicode.c.i

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffunicode.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffunicode.c.s"
	cd /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/ffunicode.c -o CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffunicode.c.s

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffunicode.c.o.requires:

.PHONY : external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffunicode.c.o.requires

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffunicode.c.o.provides: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffunicode.c.o.requires
	$(MAKE) -f external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/build.make external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffunicode.c.o.provides.build
.PHONY : external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffunicode.c.o.provides

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffunicode.c.o.provides.build: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffunicode.c.o


# Object files for target gadget114514_fmkfs
gadget114514_fmkfs_OBJECTS = \
"CMakeFiles/gadget114514_fmkfs.dir/fmkfs.c.o" \
"CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/diskio.c.o" \
"CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ff.c.o" \
"CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffsystem.c.o" \
"CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffunicode.c.o"

# External object files for target gadget114514_fmkfs
gadget114514_fmkfs_EXTERNAL_OBJECTS =

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/gadget114514_fmkfs: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fmkfs.c.o
external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/gadget114514_fmkfs: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/diskio.c.o
external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/gadget114514_fmkfs: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ff.c.o
external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/gadget114514_fmkfs: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffsystem.c.o
external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/gadget114514_fmkfs: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffunicode.c.o
external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/gadget114514_fmkfs: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/build.make
external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/gadget114514_fmkfs: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bele/CMqttSnForwarder/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable gadget114514_fmkfs"
	cd /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gadget114514_fmkfs.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/build: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/gadget114514_fmkfs

.PHONY : external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/build

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/requires: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fmkfs.c.o.requires
external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/requires: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/diskio.c.o.requires
external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/requires: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ff.c.o.requires
external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/requires: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffsystem.c.o.requires
external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/requires: external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/fatfs/source/ffunicode.c.o.requires

.PHONY : external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/requires

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/clean:
	cd /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201 && $(CMAKE_COMMAND) -P CMakeFiles/gadget114514_fmkfs.dir/cmake_clean.cmake
.PHONY : external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/clean

external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/depend:
	cd /home/bele/CMqttSnForwarder && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bele/CMqttSnForwarder /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201 /home/bele/CMqttSnForwarder /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201 /home/bele/CMqttSnForwarder/external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/CMakeFiles/gadget114514_fmkfs.dir/depend
