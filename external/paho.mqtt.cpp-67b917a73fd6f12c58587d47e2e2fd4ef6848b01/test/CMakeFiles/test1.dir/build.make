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
include external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/depend.make

# Include the progress variables for this target.
include external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/progress.make

# Include the compile flags for this target's objects.
include external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/flags.make

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/test1.c.o: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/flags.make
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/test1.c.o: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/test1.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bele/CMqttSnForwarder/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/test1.c.o"
	cd /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/test1.dir/test1.c.o   -c /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/test1.c

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/test1.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test1.dir/test1.c.i"
	cd /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/test1.c > CMakeFiles/test1.dir/test1.c.i

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/test1.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test1.dir/test1.c.s"
	cd /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/test1.c -o CMakeFiles/test1.dir/test1.c.s

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/test1.c.o.requires:

.PHONY : external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/test1.c.o.requires

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/test1.c.o.provides: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/test1.c.o.requires
	$(MAKE) -f external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/build.make external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/test1.c.o.provides.build
.PHONY : external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/test1.c.o.provides

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/test1.c.o.provides.build: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/test1.c.o


# Object files for target test1
test1_OBJECTS = \
"CMakeFiles/test1.dir/test1.c.o"

# External object files for target test1
test1_EXTERNAL_OBJECTS =

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/test1: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/test1.c.o
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/test1: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/build.make
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/test1: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/test1: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bele/CMqttSnForwarder/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable test1"
	cd /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/build: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/test1

.PHONY : external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/build

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/requires: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/test1.c.o.requires

.PHONY : external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/requires

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/clean:
	cd /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test && $(CMAKE_COMMAND) -P CMakeFiles/test1.dir/cmake_clean.cmake
.PHONY : external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/clean

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/depend:
	cd /home/bele/CMqttSnForwarder && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bele/CMqttSnForwarder /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test /home/bele/CMqttSnForwarder /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/CMakeFiles/test1.dir/depend

