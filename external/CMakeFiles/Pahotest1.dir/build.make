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
include external/CMakeFiles/Pahotest1.dir/depend.make

# Include the progress variables for this target.
include external/CMakeFiles/Pahotest1.dir/progress.make

# Include the compile flags for this target's objects.
include external/CMakeFiles/Pahotest1.dir/flags.make

external/CMakeFiles/Pahotest1.dir/__/shorttests/pahotest1main.c.o: external/CMakeFiles/Pahotest1.dir/flags.make
external/CMakeFiles/Pahotest1.dir/__/shorttests/pahotest1main.c.o: shorttests/pahotest1main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bele/CMqttSnForwarder/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object external/CMakeFiles/Pahotest1.dir/__/shorttests/pahotest1main.c.o"
	cd /home/bele/CMqttSnForwarder/external && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Pahotest1.dir/__/shorttests/pahotest1main.c.o   -c /home/bele/CMqttSnForwarder/shorttests/pahotest1main.c

external/CMakeFiles/Pahotest1.dir/__/shorttests/pahotest1main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Pahotest1.dir/__/shorttests/pahotest1main.c.i"
	cd /home/bele/CMqttSnForwarder/external && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bele/CMqttSnForwarder/shorttests/pahotest1main.c > CMakeFiles/Pahotest1.dir/__/shorttests/pahotest1main.c.i

external/CMakeFiles/Pahotest1.dir/__/shorttests/pahotest1main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Pahotest1.dir/__/shorttests/pahotest1main.c.s"
	cd /home/bele/CMqttSnForwarder/external && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bele/CMqttSnForwarder/shorttests/pahotest1main.c -o CMakeFiles/Pahotest1.dir/__/shorttests/pahotest1main.c.s

external/CMakeFiles/Pahotest1.dir/__/shorttests/pahotest1main.c.o.requires:

.PHONY : external/CMakeFiles/Pahotest1.dir/__/shorttests/pahotest1main.c.o.requires

external/CMakeFiles/Pahotest1.dir/__/shorttests/pahotest1main.c.o.provides: external/CMakeFiles/Pahotest1.dir/__/shorttests/pahotest1main.c.o.requires
	$(MAKE) -f external/CMakeFiles/Pahotest1.dir/build.make external/CMakeFiles/Pahotest1.dir/__/shorttests/pahotest1main.c.o.provides.build
.PHONY : external/CMakeFiles/Pahotest1.dir/__/shorttests/pahotest1main.c.o.provides

external/CMakeFiles/Pahotest1.dir/__/shorttests/pahotest1main.c.o.provides.build: external/CMakeFiles/Pahotest1.dir/__/shorttests/pahotest1main.c.o


# Object files for target Pahotest1
Pahotest1_OBJECTS = \
"CMakeFiles/Pahotest1.dir/__/shorttests/pahotest1main.c.o"

# External object files for target Pahotest1
Pahotest1_EXTERNAL_OBJECTS =

external/Pahotest1: external/CMakeFiles/Pahotest1.dir/__/shorttests/pahotest1main.c.o
external/Pahotest1: external/CMakeFiles/Pahotest1.dir/build.make
external/Pahotest1: external/libLibPahotest1.a
external/Pahotest1: external/libLibPahoMQTTSNPacketClient.so
external/Pahotest1: external/libLibPahoMQTTSNPacketServer.so
external/Pahotest1: external/CMakeFiles/Pahotest1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bele/CMqttSnForwarder/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable Pahotest1"
	cd /home/bele/CMqttSnForwarder/external && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Pahotest1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
external/CMakeFiles/Pahotest1.dir/build: external/Pahotest1

.PHONY : external/CMakeFiles/Pahotest1.dir/build

external/CMakeFiles/Pahotest1.dir/requires: external/CMakeFiles/Pahotest1.dir/__/shorttests/pahotest1main.c.o.requires

.PHONY : external/CMakeFiles/Pahotest1.dir/requires

external/CMakeFiles/Pahotest1.dir/clean:
	cd /home/bele/CMqttSnForwarder/external && $(CMAKE_COMMAND) -P CMakeFiles/Pahotest1.dir/cmake_clean.cmake
.PHONY : external/CMakeFiles/Pahotest1.dir/clean

external/CMakeFiles/Pahotest1.dir/depend:
	cd /home/bele/CMqttSnForwarder && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bele/CMqttSnForwarder /home/bele/CMqttSnForwarder/external /home/bele/CMqttSnForwarder /home/bele/CMqttSnForwarder/external /home/bele/CMqttSnForwarder/external/CMakeFiles/Pahotest1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : external/CMakeFiles/Pahotest1.dir/depend

