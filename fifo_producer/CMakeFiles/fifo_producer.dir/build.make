# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.8.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.8.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/everyoo/Movies/Code/fifo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/everyoo/Movies/Code/fifo

# Include any dependencies generated for this target.
include CMakeFiles/fifo_producer.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/fifo_producer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/fifo_producer.dir/flags.make

CMakeFiles/fifo_producer.dir/lock_set.c.o: CMakeFiles/fifo_producer.dir/flags.make
CMakeFiles/fifo_producer.dir/lock_set.c.o: lock_set.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/everyoo/Movies/Code/fifo/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/fifo_producer.dir/lock_set.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/fifo_producer.dir/lock_set.c.o   -c /Users/everyoo/Movies/Code/fifo/lock_set.c

CMakeFiles/fifo_producer.dir/lock_set.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/fifo_producer.dir/lock_set.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/everyoo/Movies/Code/fifo/lock_set.c > CMakeFiles/fifo_producer.dir/lock_set.c.i

CMakeFiles/fifo_producer.dir/lock_set.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/fifo_producer.dir/lock_set.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/everyoo/Movies/Code/fifo/lock_set.c -o CMakeFiles/fifo_producer.dir/lock_set.c.s

CMakeFiles/fifo_producer.dir/lock_set.c.o.requires:

.PHONY : CMakeFiles/fifo_producer.dir/lock_set.c.o.requires

CMakeFiles/fifo_producer.dir/lock_set.c.o.provides: CMakeFiles/fifo_producer.dir/lock_set.c.o.requires
	$(MAKE) -f CMakeFiles/fifo_producer.dir/build.make CMakeFiles/fifo_producer.dir/lock_set.c.o.provides.build
.PHONY : CMakeFiles/fifo_producer.dir/lock_set.c.o.provides

CMakeFiles/fifo_producer.dir/lock_set.c.o.provides.build: CMakeFiles/fifo_producer.dir/lock_set.c.o


CMakeFiles/fifo_producer.dir/producer.c.o: CMakeFiles/fifo_producer.dir/flags.make
CMakeFiles/fifo_producer.dir/producer.c.o: producer.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/everyoo/Movies/Code/fifo/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/fifo_producer.dir/producer.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/fifo_producer.dir/producer.c.o   -c /Users/everyoo/Movies/Code/fifo/producer.c

CMakeFiles/fifo_producer.dir/producer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/fifo_producer.dir/producer.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/everyoo/Movies/Code/fifo/producer.c > CMakeFiles/fifo_producer.dir/producer.c.i

CMakeFiles/fifo_producer.dir/producer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/fifo_producer.dir/producer.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/everyoo/Movies/Code/fifo/producer.c -o CMakeFiles/fifo_producer.dir/producer.c.s

CMakeFiles/fifo_producer.dir/producer.c.o.requires:

.PHONY : CMakeFiles/fifo_producer.dir/producer.c.o.requires

CMakeFiles/fifo_producer.dir/producer.c.o.provides: CMakeFiles/fifo_producer.dir/producer.c.o.requires
	$(MAKE) -f CMakeFiles/fifo_producer.dir/build.make CMakeFiles/fifo_producer.dir/producer.c.o.provides.build
.PHONY : CMakeFiles/fifo_producer.dir/producer.c.o.provides

CMakeFiles/fifo_producer.dir/producer.c.o.provides.build: CMakeFiles/fifo_producer.dir/producer.c.o


# Object files for target fifo_producer
fifo_producer_OBJECTS = \
"CMakeFiles/fifo_producer.dir/lock_set.c.o" \
"CMakeFiles/fifo_producer.dir/producer.c.o"

# External object files for target fifo_producer
fifo_producer_EXTERNAL_OBJECTS =

fifo_producer: CMakeFiles/fifo_producer.dir/lock_set.c.o
fifo_producer: CMakeFiles/fifo_producer.dir/producer.c.o
fifo_producer: CMakeFiles/fifo_producer.dir/build.make
fifo_producer: CMakeFiles/fifo_producer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/everyoo/Movies/Code/fifo/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable fifo_producer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fifo_producer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/fifo_producer.dir/build: fifo_producer

.PHONY : CMakeFiles/fifo_producer.dir/build

CMakeFiles/fifo_producer.dir/requires: CMakeFiles/fifo_producer.dir/lock_set.c.o.requires
CMakeFiles/fifo_producer.dir/requires: CMakeFiles/fifo_producer.dir/producer.c.o.requires

.PHONY : CMakeFiles/fifo_producer.dir/requires

CMakeFiles/fifo_producer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/fifo_producer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/fifo_producer.dir/clean

CMakeFiles/fifo_producer.dir/depend:
	cd /Users/everyoo/Movies/Code/fifo && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/everyoo/Movies/Code/fifo /Users/everyoo/Movies/Code/fifo /Users/everyoo/Movies/Code/fifo /Users/everyoo/Movies/Code/fifo /Users/everyoo/Movies/Code/fifo/CMakeFiles/fifo_producer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/fifo_producer.dir/depend

