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
include external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/depend.make

# Include the progress variables for this target.
include external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/progress.make

# Include the compile flags for this target's objects.
include external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/flags.make

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/MQTTVersion.c.o: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/flags.make
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/MQTTVersion.c.o: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/MQTTVersion.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bele/CMqttSnForwarder/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/MQTTVersion.c.o"
	cd /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/MQTTVersion.dir/MQTTVersion.c.o   -c /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/MQTTVersion.c

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/MQTTVersion.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/MQTTVersion.dir/MQTTVersion.c.i"
	cd /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/MQTTVersion.c > CMakeFiles/MQTTVersion.dir/MQTTVersion.c.i

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/MQTTVersion.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/MQTTVersion.dir/MQTTVersion.c.s"
	cd /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/MQTTVersion.c -o CMakeFiles/MQTTVersion.dir/MQTTVersion.c.s

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/MQTTVersion.c.o.requires:

.PHONY : external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/MQTTVersion.c.o.requires

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/MQTTVersion.c.o.provides: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/MQTTVersion.c.o.requires
	$(MAKE) -f external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/build.make external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/MQTTVersion.c.o.provides.build
.PHONY : external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/MQTTVersion.c.o.provides

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/MQTTVersion.c.o.provides.build: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/MQTTVersion.c.o


# Object files for target MQTTVersion
MQTTVersion_OBJECTS = \
"CMakeFiles/MQTTVersion.dir/MQTTVersion.c.o"

# External object files for target MQTTVersion
MQTTVersion_EXTERNAL_OBJECTS =

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/MQTTVersion: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/MQTTVersion.c.o
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/MQTTVersion: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/build.make
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/MQTTVersion: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3a.so.1.3.0
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/MQTTVersion: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/MQTTVersion: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bele/CMqttSnForwarder/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable MQTTVersion"
	cd /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MQTTVersion.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/build: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/MQTTVersion

.PHONY : external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/build

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/requires: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/MQTTVersion.c.o.requires

.PHONY : external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/requires

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/clean:
	cd /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src && $(CMAKE_COMMAND) -P CMakeFiles/MQTTVersion.dir/cmake_clean.cmake
.PHONY : external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/clean

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/depend:
	cd /home/bele/CMqttSnForwarder && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bele/CMqttSnForwarder /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src /home/bele/CMqttSnForwarder /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/MQTTVersion.dir/depend
