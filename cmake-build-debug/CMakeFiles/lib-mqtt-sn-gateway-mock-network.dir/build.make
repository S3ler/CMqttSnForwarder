# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /home/bele/JetBrains/Clion/clion-2018.3.4/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/bele/JetBrains/Clion/clion-2018.3.4/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/bele/CLionProjects/CMqttSnForwarder

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bele/CLionProjects/CMqttSnForwarder/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/flags.make

CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/test/network/gateway/MockGatewayNetwork/MockGateway.cpp.o: CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/flags.make
CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/test/network/gateway/MockGatewayNetwork/MockGateway.cpp.o: ../test/network/gateway/MockGatewayNetwork/MockGateway.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bele/CLionProjects/CMqttSnForwarder/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/test/network/gateway/MockGatewayNetwork/MockGateway.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/test/network/gateway/MockGatewayNetwork/MockGateway.cpp.o -c /home/bele/CLionProjects/CMqttSnForwarder/test/network/gateway/MockGatewayNetwork/MockGateway.cpp

CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/test/network/gateway/MockGatewayNetwork/MockGateway.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/test/network/gateway/MockGatewayNetwork/MockGateway.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bele/CLionProjects/CMqttSnForwarder/test/network/gateway/MockGatewayNetwork/MockGateway.cpp > CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/test/network/gateway/MockGatewayNetwork/MockGateway.cpp.i

CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/test/network/gateway/MockGatewayNetwork/MockGateway.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/test/network/gateway/MockGatewayNetwork/MockGateway.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bele/CLionProjects/CMqttSnForwarder/test/network/gateway/MockGatewayNetwork/MockGateway.cpp -o CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/test/network/gateway/MockGatewayNetwork/MockGateway.cpp.s

# Object files for target lib-mqtt-sn-gateway-mock-network
lib__mqtt__sn__gateway__mock__network_OBJECTS = \
"CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/test/network/gateway/MockGatewayNetwork/MockGateway.cpp.o"

# External object files for target lib-mqtt-sn-gateway-mock-network
lib__mqtt__sn__gateway__mock__network_EXTERNAL_OBJECTS =

liblib-mqtt-sn-gateway-mock-network.a: CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/test/network/gateway/MockGatewayNetwork/MockGateway.cpp.o
liblib-mqtt-sn-gateway-mock-network.a: CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/build.make
liblib-mqtt-sn-gateway-mock-network.a: CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bele/CLionProjects/CMqttSnForwarder/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library liblib-mqtt-sn-gateway-mock-network.a"
	$(CMAKE_COMMAND) -P CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/build: liblib-mqtt-sn-gateway-mock-network.a

.PHONY : CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/build

CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/cmake_clean.cmake
.PHONY : CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/clean

CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/depend:
	cd /home/bele/CLionProjects/CMqttSnForwarder/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bele/CLionProjects/CMqttSnForwarder /home/bele/CLionProjects/CMqttSnForwarder /home/bele/CLionProjects/CMqttSnForwarder/cmake-build-debug /home/bele/CLionProjects/CMqttSnForwarder/cmake-build-debug /home/bele/CLionProjects/CMqttSnForwarder/cmake-build-debug/CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/lib-mqtt-sn-gateway-mock-network.dir/depend

