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
include external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/depend.make

# Include the progress variables for this target.
include external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/progress.make

# Include the compile flags for this target's objects.
include external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/flags.make

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/MQTTClient.c.o: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/flags.make
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/MQTTClient.c.o: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/MQTTClient.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bele/CMqttSnForwarder/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/MQTTClient.c.o"
	cd /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/paho-mqtt3c.dir/MQTTClient.c.o   -c /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/MQTTClient.c

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/MQTTClient.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/paho-mqtt3c.dir/MQTTClient.c.i"
	cd /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/MQTTClient.c > CMakeFiles/paho-mqtt3c.dir/MQTTClient.c.i

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/MQTTClient.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/paho-mqtt3c.dir/MQTTClient.c.s"
	cd /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/MQTTClient.c -o CMakeFiles/paho-mqtt3c.dir/MQTTClient.c.s

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/MQTTClient.c.o.requires:

.PHONY : external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/MQTTClient.c.o.requires

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/MQTTClient.c.o.provides: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/MQTTClient.c.o.requires
	$(MAKE) -f external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/build.make external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/MQTTClient.c.o.provides.build
.PHONY : external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/MQTTClient.c.o.provides

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/MQTTClient.c.o.provides.build: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/MQTTClient.c.o


# Object files for target paho-mqtt3c
paho__mqtt3c_OBJECTS = \
"CMakeFiles/paho-mqtt3c.dir/MQTTClient.c.o"

# External object files for target paho-mqtt3c
paho__mqtt3c_EXTERNAL_OBJECTS = \
"/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/MQTTProtocolClient.c.o" \
"/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/Clients.c.o" \
"/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/utf-8.c.o" \
"/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/StackTrace.c.o" \
"/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/MQTTPacket.c.o" \
"/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/MQTTPacketOut.c.o" \
"/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/Messages.c.o" \
"/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/Tree.c.o" \
"/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/Socket.c.o" \
"/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/Log.c.o" \
"/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/MQTTPersistence.c.o" \
"/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/Thread.c.o" \
"/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/MQTTProtocolOut.c.o" \
"/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/MQTTPersistenceDefault.c.o" \
"/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/SocketBuffer.c.o" \
"/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/Heap.c.o" \
"/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/LinkedList.c.o" \
"/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/MQTTProperties.c.o" \
"/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/MQTTReasonCodes.c.o" \
"/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/Base64.c.o" \
"/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/SHA1.c.o" \
"/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/WebSocket.c.o"

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/MQTTClient.c.o
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/MQTTProtocolClient.c.o
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/Clients.c.o
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/utf-8.c.o
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/StackTrace.c.o
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/MQTTPacket.c.o
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/MQTTPacketOut.c.o
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/Messages.c.o
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/Tree.c.o
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/Socket.c.o
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/Log.c.o
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/MQTTPersistence.c.o
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/Thread.c.o
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/MQTTProtocolOut.c.o
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/MQTTPersistenceDefault.c.o
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/SocketBuffer.c.o
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/Heap.c.o
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/LinkedList.c.o
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/MQTTProperties.c.o
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/MQTTReasonCodes.c.o
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/Base64.c.o
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/SHA1.c.o
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/common_obj.dir/WebSocket.c.o
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/build.make
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bele/CMqttSnForwarder/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C shared library libpaho-mqtt3c.so"
	cd /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/paho-mqtt3c.dir/link.txt --verbose=$(VERBOSE)
	cd /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src && $(CMAKE_COMMAND) -E cmake_symlink_library libpaho-mqtt3c.so.1.3.0 libpaho-mqtt3c.so.1 libpaho-mqtt3c.so

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0
	@$(CMAKE_COMMAND) -E touch_nocreate external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so.1.3.0
	@$(CMAKE_COMMAND) -E touch_nocreate external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so

# Rule to build all files generated by this target.
external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/build: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/libpaho-mqtt3c.so

.PHONY : external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/build

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/requires: external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/MQTTClient.c.o.requires

.PHONY : external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/requires

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/clean:
	cd /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src && $(CMAKE_COMMAND) -P CMakeFiles/paho-mqtt3c.dir/cmake_clean.cmake
.PHONY : external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/clean

external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/depend:
	cd /home/bele/CMqttSnForwarder && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bele/CMqttSnForwarder /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src /home/bele/CMqttSnForwarder /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/CMakeFiles/paho-mqtt3c.dir/depend

