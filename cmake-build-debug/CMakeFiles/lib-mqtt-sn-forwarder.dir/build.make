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
CMAKE_SOURCE_DIR = /tmp/tmp.ycYCPoRMKx

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/tmp.ycYCPoRMKx/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/lib-mqtt-sn-forwarder.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/lib-mqtt-sn-forwarder.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lib-mqtt-sn-forwarder.dir/flags.make

CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnFordwarder.c.o: CMakeFiles/lib-mqtt-sn-forwarder.dir/flags.make
CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnFordwarder.c.o: ../forwarder/MqttSnFordwarder.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.ycYCPoRMKx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnFordwarder.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnFordwarder.c.o   -c /tmp/tmp.ycYCPoRMKx/forwarder/MqttSnFordwarder.c

CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnFordwarder.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnFordwarder.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /tmp/tmp.ycYCPoRMKx/forwarder/MqttSnFordwarder.c > CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnFordwarder.c.i

CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnFordwarder.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnFordwarder.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /tmp/tmp.ycYCPoRMKx/forwarder/MqttSnFordwarder.c -o CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnFordwarder.c.s

CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnFordwarder.c.o.requires:

.PHONY : CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnFordwarder.c.o.requires

CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnFordwarder.c.o.provides: CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnFordwarder.c.o.requires
	$(MAKE) -f CMakeFiles/lib-mqtt-sn-forwarder.dir/build.make CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnFordwarder.c.o.provides.build
.PHONY : CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnFordwarder.c.o.provides

CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnFordwarder.c.o.provides.build: CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnFordwarder.c.o


CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnGatewayNetworkInterface.c.o: CMakeFiles/lib-mqtt-sn-forwarder.dir/flags.make
CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnGatewayNetworkInterface.c.o: ../forwarder/MqttSnGatewayNetworkInterface.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.ycYCPoRMKx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnGatewayNetworkInterface.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnGatewayNetworkInterface.c.o   -c /tmp/tmp.ycYCPoRMKx/forwarder/MqttSnGatewayNetworkInterface.c

CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnGatewayNetworkInterface.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnGatewayNetworkInterface.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /tmp/tmp.ycYCPoRMKx/forwarder/MqttSnGatewayNetworkInterface.c > CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnGatewayNetworkInterface.c.i

CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnGatewayNetworkInterface.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnGatewayNetworkInterface.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /tmp/tmp.ycYCPoRMKx/forwarder/MqttSnGatewayNetworkInterface.c -o CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnGatewayNetworkInterface.c.s

CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnGatewayNetworkInterface.c.o.requires:

.PHONY : CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnGatewayNetworkInterface.c.o.requires

CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnGatewayNetworkInterface.c.o.provides: CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnGatewayNetworkInterface.c.o.requires
	$(MAKE) -f CMakeFiles/lib-mqtt-sn-forwarder.dir/build.make CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnGatewayNetworkInterface.c.o.provides.build
.PHONY : CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnGatewayNetworkInterface.c.o.provides

CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnGatewayNetworkInterface.c.o.provides.build: CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnGatewayNetworkInterface.c.o


CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnClientNetworkInterface.c.o: CMakeFiles/lib-mqtt-sn-forwarder.dir/flags.make
CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnClientNetworkInterface.c.o: ../forwarder/MqttSnClientNetworkInterface.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.ycYCPoRMKx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnClientNetworkInterface.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnClientNetworkInterface.c.o   -c /tmp/tmp.ycYCPoRMKx/forwarder/MqttSnClientNetworkInterface.c

CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnClientNetworkInterface.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnClientNetworkInterface.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /tmp/tmp.ycYCPoRMKx/forwarder/MqttSnClientNetworkInterface.c > CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnClientNetworkInterface.c.i

CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnClientNetworkInterface.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnClientNetworkInterface.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /tmp/tmp.ycYCPoRMKx/forwarder/MqttSnClientNetworkInterface.c -o CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnClientNetworkInterface.c.s

CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnClientNetworkInterface.c.o.requires:

.PHONY : CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnClientNetworkInterface.c.o.requires

CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnClientNetworkInterface.c.o.provides: CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnClientNetworkInterface.c.o.requires
	$(MAKE) -f CMakeFiles/lib-mqtt-sn-forwarder.dir/build.make CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnClientNetworkInterface.c.o.provides.build
.PHONY : CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnClientNetworkInterface.c.o.provides

CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnClientNetworkInterface.c.o.provides.build: CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnClientNetworkInterface.c.o


# Object files for target lib-mqtt-sn-forwarder
lib__mqtt__sn__forwarder_OBJECTS = \
"CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnFordwarder.c.o" \
"CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnGatewayNetworkInterface.c.o" \
"CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnClientNetworkInterface.c.o"

# External object files for target lib-mqtt-sn-forwarder
lib__mqtt__sn__forwarder_EXTERNAL_OBJECTS =

liblib-mqtt-sn-forwarder.a: CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnFordwarder.c.o
liblib-mqtt-sn-forwarder.a: CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnGatewayNetworkInterface.c.o
liblib-mqtt-sn-forwarder.a: CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnClientNetworkInterface.c.o
liblib-mqtt-sn-forwarder.a: CMakeFiles/lib-mqtt-sn-forwarder.dir/build.make
liblib-mqtt-sn-forwarder.a: CMakeFiles/lib-mqtt-sn-forwarder.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/tmp.ycYCPoRMKx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C static library liblib-mqtt-sn-forwarder.a"
	$(CMAKE_COMMAND) -P CMakeFiles/lib-mqtt-sn-forwarder.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lib-mqtt-sn-forwarder.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lib-mqtt-sn-forwarder.dir/build: liblib-mqtt-sn-forwarder.a

.PHONY : CMakeFiles/lib-mqtt-sn-forwarder.dir/build

CMakeFiles/lib-mqtt-sn-forwarder.dir/requires: CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnFordwarder.c.o.requires
CMakeFiles/lib-mqtt-sn-forwarder.dir/requires: CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnGatewayNetworkInterface.c.o.requires
CMakeFiles/lib-mqtt-sn-forwarder.dir/requires: CMakeFiles/lib-mqtt-sn-forwarder.dir/forwarder/MqttSnClientNetworkInterface.c.o.requires

.PHONY : CMakeFiles/lib-mqtt-sn-forwarder.dir/requires

CMakeFiles/lib-mqtt-sn-forwarder.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/lib-mqtt-sn-forwarder.dir/cmake_clean.cmake
.PHONY : CMakeFiles/lib-mqtt-sn-forwarder.dir/clean

CMakeFiles/lib-mqtt-sn-forwarder.dir/depend:
	cd /tmp/tmp.ycYCPoRMKx/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/tmp.ycYCPoRMKx /tmp/tmp.ycYCPoRMKx /tmp/tmp.ycYCPoRMKx/cmake-build-debug /tmp/tmp.ycYCPoRMKx/cmake-build-debug /tmp/tmp.ycYCPoRMKx/cmake-build-debug/CMakeFiles/lib-mqtt-sn-forwarder.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/lib-mqtt-sn-forwarder.dir/depend

