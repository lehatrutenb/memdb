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
include memdb/internal/dataBase/CMakeFiles/Memdb_dbTest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include memdb/internal/dataBase/CMakeFiles/Memdb_dbTest.dir/compiler_depend.make

# Include the progress variables for this target.
include memdb/internal/dataBase/CMakeFiles/Memdb_dbTest.dir/progress.make

# Include the compile flags for this target's objects.
include memdb/internal/dataBase/CMakeFiles/Memdb_dbTest.dir/flags.make

memdb/internal/dataBase/CMakeFiles/Memdb_dbTest.dir/test.cpp.o: memdb/internal/dataBase/CMakeFiles/Memdb_dbTest.dir/flags.make
memdb/internal/dataBase/CMakeFiles/Memdb_dbTest.dir/test.cpp.o: /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/dataBase/test.cpp
memdb/internal/dataBase/CMakeFiles/Memdb_dbTest.dir/test.cpp.o: memdb/internal/dataBase/CMakeFiles/Memdb_dbTest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/lehatrutenb/Desktop/prog/memdb/memdb_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object memdb/internal/dataBase/CMakeFiles/Memdb_dbTest.dir/test.cpp.o"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/dataBase && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT memdb/internal/dataBase/CMakeFiles/Memdb_dbTest.dir/test.cpp.o -MF CMakeFiles/Memdb_dbTest.dir/test.cpp.o.d -o CMakeFiles/Memdb_dbTest.dir/test.cpp.o -c /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/dataBase/test.cpp

memdb/internal/dataBase/CMakeFiles/Memdb_dbTest.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Memdb_dbTest.dir/test.cpp.i"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/dataBase && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/dataBase/test.cpp > CMakeFiles/Memdb_dbTest.dir/test.cpp.i

memdb/internal/dataBase/CMakeFiles/Memdb_dbTest.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Memdb_dbTest.dir/test.cpp.s"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/dataBase && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/dataBase/test.cpp -o CMakeFiles/Memdb_dbTest.dir/test.cpp.s

# Object files for target Memdb_dbTest
Memdb_dbTest_OBJECTS = \
"CMakeFiles/Memdb_dbTest.dir/test.cpp.o"

# External object files for target Memdb_dbTest
Memdb_dbTest_EXTERNAL_OBJECTS =

memdb/internal/dataBase/Memdb_dbTest: memdb/internal/dataBase/CMakeFiles/Memdb_dbTest.dir/test.cpp.o
memdb/internal/dataBase/Memdb_dbTest: memdb/internal/dataBase/CMakeFiles/Memdb_dbTest.dir/build.make
memdb/internal/dataBase/Memdb_dbTest: lib/libgtest_main.a
memdb/internal/dataBase/Memdb_dbTest: memdb/internal/dataBase/libMemdb_db.a
memdb/internal/dataBase/Memdb_dbTest: lib/libgtest.a
memdb/internal/dataBase/Memdb_dbTest: memdb/internal/parser/libparser.a
memdb/internal/dataBase/Memdb_dbTest: memdb/internal/type/libtype.a
memdb/internal/dataBase/Memdb_dbTest: memdb/internal/column/libcolumn.a
memdb/internal/dataBase/Memdb_dbTest: memdb/internal/table/libtable.a
memdb/internal/dataBase/Memdb_dbTest: memdb/internal/dataBase/CMakeFiles/Memdb_dbTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/lehatrutenb/Desktop/prog/memdb/memdb_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Memdb_dbTest"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/dataBase && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Memdb_dbTest.dir/link.txt --verbose=$(VERBOSE)
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/dataBase && /opt/homebrew/Cellar/cmake/3.28.3/bin/cmake -D TEST_TARGET=Memdb_dbTest -D TEST_EXECUTABLE=/Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/dataBase/Memdb_dbTest -D TEST_EXECUTOR= -D TEST_WORKING_DIR=/Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/dataBase -D TEST_EXTRA_ARGS= -D TEST_PROPERTIES= -D TEST_PREFIX= -D TEST_SUFFIX= -D TEST_FILTER= -D NO_PRETTY_TYPES=FALSE -D NO_PRETTY_VALUES=FALSE -D TEST_LIST=Memdb_dbTest_TESTS -D CTEST_FILE=/Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/dataBase/Memdb_dbTest[1]_tests.cmake -D TEST_DISCOVERY_TIMEOUT=5 -D TEST_XML_OUTPUT_DIR= -P /opt/homebrew/Cellar/cmake/3.28.3/share/cmake/Modules/GoogleTestAddTests.cmake

# Rule to build all files generated by this target.
memdb/internal/dataBase/CMakeFiles/Memdb_dbTest.dir/build: memdb/internal/dataBase/Memdb_dbTest
.PHONY : memdb/internal/dataBase/CMakeFiles/Memdb_dbTest.dir/build

memdb/internal/dataBase/CMakeFiles/Memdb_dbTest.dir/clean:
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/dataBase && $(CMAKE_COMMAND) -P CMakeFiles/Memdb_dbTest.dir/cmake_clean.cmake
.PHONY : memdb/internal/dataBase/CMakeFiles/Memdb_dbTest.dir/clean

memdb/internal/dataBase/CMakeFiles/Memdb_dbTest.dir/depend:
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/lehatrutenb/Desktop/prog/memdb /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/dataBase /Users/lehatrutenb/Desktop/prog/memdb/memdb_build /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/dataBase /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/dataBase/CMakeFiles/Memdb_dbTest.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : memdb/internal/dataBase/CMakeFiles/Memdb_dbTest.dir/depend

