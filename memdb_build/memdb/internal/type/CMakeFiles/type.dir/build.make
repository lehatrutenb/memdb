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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.28.3/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.28.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/lehatrutenb/Desktop/prog/memdb

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/lehatrutenb/Desktop/prog/memdb/memdb_build

# Include any dependencies generated for this target.
include memdb/internal/type/CMakeFiles/type.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include memdb/internal/type/CMakeFiles/type.dir/compiler_depend.make

# Include the progress variables for this target.
include memdb/internal/type/CMakeFiles/type.dir/progress.make

# Include the compile flags for this target's objects.
include memdb/internal/type/CMakeFiles/type.dir/flags.make

memdb/internal/type/CMakeFiles/type.dir/type.cpp.o: memdb/internal/type/CMakeFiles/type.dir/flags.make
memdb/internal/type/CMakeFiles/type.dir/type.cpp.o: /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/type/type.cpp
memdb/internal/type/CMakeFiles/type.dir/type.cpp.o: memdb/internal/type/CMakeFiles/type.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/lehatrutenb/Desktop/prog/memdb/memdb_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object memdb/internal/type/CMakeFiles/type.dir/type.cpp.o"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/type && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT memdb/internal/type/CMakeFiles/type.dir/type.cpp.o -MF CMakeFiles/type.dir/type.cpp.o.d -o CMakeFiles/type.dir/type.cpp.o -c /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/type/type.cpp

memdb/internal/type/CMakeFiles/type.dir/type.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/type.dir/type.cpp.i"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/type && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/type/type.cpp > CMakeFiles/type.dir/type.cpp.i

memdb/internal/type/CMakeFiles/type.dir/type.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/type.dir/type.cpp.s"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/type && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/type/type.cpp -o CMakeFiles/type.dir/type.cpp.s

# Object files for target type
type_OBJECTS = \
"CMakeFiles/type.dir/type.cpp.o"

# External object files for target type
type_EXTERNAL_OBJECTS =

memdb/internal/type/libtype.a: memdb/internal/type/CMakeFiles/type.dir/type.cpp.o
memdb/internal/type/libtype.a: memdb/internal/type/CMakeFiles/type.dir/build.make
memdb/internal/type/libtype.a: memdb/internal/type/CMakeFiles/type.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/lehatrutenb/Desktop/prog/memdb/memdb_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libtype.a"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/type && $(CMAKE_COMMAND) -P CMakeFiles/type.dir/cmake_clean_target.cmake
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/type && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/type.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
memdb/internal/type/CMakeFiles/type.dir/build: memdb/internal/type/libtype.a
.PHONY : memdb/internal/type/CMakeFiles/type.dir/build

memdb/internal/type/CMakeFiles/type.dir/clean:
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/type && $(CMAKE_COMMAND) -P CMakeFiles/type.dir/cmake_clean.cmake
.PHONY : memdb/internal/type/CMakeFiles/type.dir/clean

memdb/internal/type/CMakeFiles/type.dir/depend:
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/lehatrutenb/Desktop/prog/memdb /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/type /Users/lehatrutenb/Desktop/prog/memdb/memdb_build /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/type /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/type/CMakeFiles/type.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : memdb/internal/type/CMakeFiles/type.dir/depend

