# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.25.0/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.25.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/anya/Pico/ece4760-finalproj

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/anya/Pico/ece4760-finalproj/build

# Utility rule file for animation_vsync_pio_h.

# Include any custom commands dependencies for this target.
include Lab_2/Animation_Demo/CMakeFiles/animation_vsync_pio_h.dir/compiler_depend.make

# Include the progress variables for this target.
include Lab_2/Animation_Demo/CMakeFiles/animation_vsync_pio_h.dir/progress.make

Lab_2/Animation_Demo/CMakeFiles/animation_vsync_pio_h: Lab_2/Animation_Demo/vsync.pio.h

Lab_2/Animation_Demo/vsync.pio.h: /Users/anya/Pico/ece4760-finalproj/Lab_2/Animation_Demo/vsync.pio
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/anya/Pico/ece4760-finalproj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating vsync.pio.h"
	cd /Users/anya/Pico/ece4760-finalproj/build/Lab_2/Animation_Demo && ../../pioasm/pioasm -o c-sdk /Users/anya/Pico/ece4760-finalproj/Lab_2/Animation_Demo/vsync.pio /Users/anya/Pico/ece4760-finalproj/build/Lab_2/Animation_Demo/vsync.pio.h

animation_vsync_pio_h: Lab_2/Animation_Demo/CMakeFiles/animation_vsync_pio_h
animation_vsync_pio_h: Lab_2/Animation_Demo/vsync.pio.h
animation_vsync_pio_h: Lab_2/Animation_Demo/CMakeFiles/animation_vsync_pio_h.dir/build.make
.PHONY : animation_vsync_pio_h

# Rule to build all files generated by this target.
Lab_2/Animation_Demo/CMakeFiles/animation_vsync_pio_h.dir/build: animation_vsync_pio_h
.PHONY : Lab_2/Animation_Demo/CMakeFiles/animation_vsync_pio_h.dir/build

Lab_2/Animation_Demo/CMakeFiles/animation_vsync_pio_h.dir/clean:
	cd /Users/anya/Pico/ece4760-finalproj/build/Lab_2/Animation_Demo && $(CMAKE_COMMAND) -P CMakeFiles/animation_vsync_pio_h.dir/cmake_clean.cmake
.PHONY : Lab_2/Animation_Demo/CMakeFiles/animation_vsync_pio_h.dir/clean

Lab_2/Animation_Demo/CMakeFiles/animation_vsync_pio_h.dir/depend:
	cd /Users/anya/Pico/ece4760-finalproj/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/anya/Pico/ece4760-finalproj /Users/anya/Pico/ece4760-finalproj/Lab_2/Animation_Demo /Users/anya/Pico/ece4760-finalproj/build /Users/anya/Pico/ece4760-finalproj/build/Lab_2/Animation_Demo /Users/anya/Pico/ece4760-finalproj/build/Lab_2/Animation_Demo/CMakeFiles/animation_vsync_pio_h.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Lab_2/Animation_Demo/CMakeFiles/animation_vsync_pio_h.dir/depend

