# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/sinow/workspace/cpp/IM

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sinow/workspace/cpp/IM/build

# Include any dependencies generated for this target.
include src/CMakeFiles/main.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/main.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/main.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/main.dir/flags.make

src/CMakeFiles/main.dir/main.o: src/CMakeFiles/main.dir/flags.make
src/CMakeFiles/main.dir/main.o: ../src/main.cc
src/CMakeFiles/main.dir/main.o: src/CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sinow/workspace/cpp/IM/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/main.dir/main.o"
	cd /home/sinow/workspace/cpp/IM/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/main.dir/main.o -MF CMakeFiles/main.dir/main.o.d -o CMakeFiles/main.dir/main.o -c /home/sinow/workspace/cpp/IM/src/main.cc

src/CMakeFiles/main.dir/main.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/main.i"
	cd /home/sinow/workspace/cpp/IM/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sinow/workspace/cpp/IM/src/main.cc > CMakeFiles/main.dir/main.i

src/CMakeFiles/main.dir/main.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/main.s"
	cd /home/sinow/workspace/cpp/IM/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sinow/workspace/cpp/IM/src/main.cc -o CMakeFiles/main.dir/main.s

# Object files for target main
main_OBJECTS = \
"CMakeFiles/main.dir/main.o"

# External object files for target main
main_EXTERNAL_OBJECTS =

../bin/main: src/CMakeFiles/main.dir/main.o
../bin/main: src/CMakeFiles/main.dir/build.make
../bin/main: ../lib/libdtr.so
../bin/main: ../lib/libworker.so
../bin/main: ../lib/libutils.so
../bin/main: ../lib/libuser.so
../bin/main: ../lib/libdb.so
../bin/main: src/CMakeFiles/main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sinow/workspace/cpp/IM/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/main"
	cd /home/sinow/workspace/cpp/IM/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/main.dir/build: ../bin/main
.PHONY : src/CMakeFiles/main.dir/build

src/CMakeFiles/main.dir/clean:
	cd /home/sinow/workspace/cpp/IM/build/src && $(CMAKE_COMMAND) -P CMakeFiles/main.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/main.dir/clean

src/CMakeFiles/main.dir/depend:
	cd /home/sinow/workspace/cpp/IM/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sinow/workspace/cpp/IM /home/sinow/workspace/cpp/IM/src /home/sinow/workspace/cpp/IM/build /home/sinow/workspace/cpp/IM/build/src /home/sinow/workspace/cpp/IM/build/src/CMakeFiles/main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/main.dir/depend
