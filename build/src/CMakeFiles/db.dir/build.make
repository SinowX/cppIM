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
include src/CMakeFiles/db.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/db.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/db.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/db.dir/flags.make

src/CMakeFiles/db.dir/database.o: src/CMakeFiles/db.dir/flags.make
src/CMakeFiles/db.dir/database.o: ../src/database.cc
src/CMakeFiles/db.dir/database.o: src/CMakeFiles/db.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sinow/workspace/cpp/IM/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/db.dir/database.o"
	cd /home/sinow/workspace/cpp/IM/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/db.dir/database.o -MF CMakeFiles/db.dir/database.o.d -o CMakeFiles/db.dir/database.o -c /home/sinow/workspace/cpp/IM/src/database.cc

src/CMakeFiles/db.dir/database.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/db.dir/database.i"
	cd /home/sinow/workspace/cpp/IM/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sinow/workspace/cpp/IM/src/database.cc > CMakeFiles/db.dir/database.i

src/CMakeFiles/db.dir/database.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/db.dir/database.s"
	cd /home/sinow/workspace/cpp/IM/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sinow/workspace/cpp/IM/src/database.cc -o CMakeFiles/db.dir/database.s

# Object files for target db
db_OBJECTS = \
"CMakeFiles/db.dir/database.o"

# External object files for target db
db_EXTERNAL_OBJECTS =

src/libdb.so: src/CMakeFiles/db.dir/database.o
src/libdb.so: src/CMakeFiles/db.dir/build.make
src/libdb.so: src/CMakeFiles/db.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sinow/workspace/cpp/IM/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libdb.so"
	cd /home/sinow/workspace/cpp/IM/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/db.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/db.dir/build: src/libdb.so
.PHONY : src/CMakeFiles/db.dir/build

src/CMakeFiles/db.dir/clean:
	cd /home/sinow/workspace/cpp/IM/build/src && $(CMAKE_COMMAND) -P CMakeFiles/db.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/db.dir/clean

src/CMakeFiles/db.dir/depend:
	cd /home/sinow/workspace/cpp/IM/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sinow/workspace/cpp/IM /home/sinow/workspace/cpp/IM/src /home/sinow/workspace/cpp/IM/build /home/sinow/workspace/cpp/IM/build/src /home/sinow/workspace/cpp/IM/build/src/CMakeFiles/db.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/db.dir/depend

