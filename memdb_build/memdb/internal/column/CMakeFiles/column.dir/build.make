# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/leha/Desktop/memdb/memdb

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/leha/Desktop/memdb/memdb/memdb_build

# Include any dependencies generated for this target.
include memdb/internal/column/CMakeFiles/column.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include memdb/internal/column/CMakeFiles/column.dir/compiler_depend.make

# Include the progress variables for this target.
include memdb/internal/column/CMakeFiles/column.dir/progress.make

# Include the compile flags for this target's objects.
include memdb/internal/column/CMakeFiles/column.dir/flags.make

memdb/internal/column/CMakeFiles/column.dir/column.cpp.o: memdb/internal/column/CMakeFiles/column.dir/flags.make
memdb/internal/column/CMakeFiles/column.dir/column.cpp.o: /home/leha/Desktop/memdb/memdb/memdb/internal/column/column.cpp
memdb/internal/column/CMakeFiles/column.dir/column.cpp.o: memdb/internal/column/CMakeFiles/column.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/leha/Desktop/memdb/memdb/memdb_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object memdb/internal/column/CMakeFiles/column.dir/column.cpp.o"
	cd /home/leha/Desktop/memdb/memdb/memdb_build/memdb/internal/column && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT memdb/internal/column/CMakeFiles/column.dir/column.cpp.o -MF CMakeFiles/column.dir/column.cpp.o.d -o CMakeFiles/column.dir/column.cpp.o -c /home/leha/Desktop/memdb/memdb/memdb/internal/column/column.cpp

memdb/internal/column/CMakeFiles/column.dir/column.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/column.dir/column.cpp.i"
	cd /home/leha/Desktop/memdb/memdb/memdb_build/memdb/internal/column && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/leha/Desktop/memdb/memdb/memdb/internal/column/column.cpp > CMakeFiles/column.dir/column.cpp.i

memdb/internal/column/CMakeFiles/column.dir/column.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/column.dir/column.cpp.s"
	cd /home/leha/Desktop/memdb/memdb/memdb_build/memdb/internal/column && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/leha/Desktop/memdb/memdb/memdb/internal/column/column.cpp -o CMakeFiles/column.dir/column.cpp.s

# Object files for target column
column_OBJECTS = \
"CMakeFiles/column.dir/column.cpp.o"

# External object files for target column
column_EXTERNAL_OBJECTS =

memdb/internal/column/libcolumn.a: memdb/internal/column/CMakeFiles/column.dir/column.cpp.o
memdb/internal/column/libcolumn.a: memdb/internal/column/CMakeFiles/column.dir/build.make
memdb/internal/column/libcolumn.a: memdb/internal/column/CMakeFiles/column.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/leha/Desktop/memdb/memdb/memdb_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libcolumn.a"
	cd /home/leha/Desktop/memdb/memdb/memdb_build/memdb/internal/column && $(CMAKE_COMMAND) -P CMakeFiles/column.dir/cmake_clean_target.cmake
	cd /home/leha/Desktop/memdb/memdb/memdb_build/memdb/internal/column && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/column.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
memdb/internal/column/CMakeFiles/column.dir/build: memdb/internal/column/libcolumn.a
.PHONY : memdb/internal/column/CMakeFiles/column.dir/build

memdb/internal/column/CMakeFiles/column.dir/clean:
	cd /home/leha/Desktop/memdb/memdb/memdb_build/memdb/internal/column && $(CMAKE_COMMAND) -P CMakeFiles/column.dir/cmake_clean.cmake
.PHONY : memdb/internal/column/CMakeFiles/column.dir/clean

memdb/internal/column/CMakeFiles/column.dir/depend:
	cd /home/leha/Desktop/memdb/memdb/memdb_build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/leha/Desktop/memdb/memdb /home/leha/Desktop/memdb/memdb/memdb/internal/column /home/leha/Desktop/memdb/memdb/memdb_build /home/leha/Desktop/memdb/memdb/memdb_build/memdb/internal/column /home/leha/Desktop/memdb/memdb/memdb_build/memdb/internal/column/CMakeFiles/column.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : memdb/internal/column/CMakeFiles/column.dir/depend

