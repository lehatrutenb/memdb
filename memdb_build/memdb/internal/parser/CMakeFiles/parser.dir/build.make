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
include memdb/internal/parser/CMakeFiles/parser.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include memdb/internal/parser/CMakeFiles/parser.dir/compiler_depend.make

# Include the progress variables for this target.
include memdb/internal/parser/CMakeFiles/parser.dir/progress.make

# Include the compile flags for this target's objects.
include memdb/internal/parser/CMakeFiles/parser.dir/flags.make

memdb/internal/parser/CMakeFiles/parser.dir/lexer.cpp.o: memdb/internal/parser/CMakeFiles/parser.dir/flags.make
memdb/internal/parser/CMakeFiles/parser.dir/lexer.cpp.o: /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/lexer.cpp
memdb/internal/parser/CMakeFiles/parser.dir/lexer.cpp.o: memdb/internal/parser/CMakeFiles/parser.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/lehatrutenb/Desktop/prog/memdb/memdb_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object memdb/internal/parser/CMakeFiles/parser.dir/lexer.cpp.o"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT memdb/internal/parser/CMakeFiles/parser.dir/lexer.cpp.o -MF CMakeFiles/parser.dir/lexer.cpp.o.d -o CMakeFiles/parser.dir/lexer.cpp.o -c /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/lexer.cpp

memdb/internal/parser/CMakeFiles/parser.dir/lexer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/parser.dir/lexer.cpp.i"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/lexer.cpp > CMakeFiles/parser.dir/lexer.cpp.i

memdb/internal/parser/CMakeFiles/parser.dir/lexer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/parser.dir/lexer.cpp.s"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/lexer.cpp -o CMakeFiles/parser.dir/lexer.cpp.s

memdb/internal/parser/CMakeFiles/parser.dir/assignments.cpp.o: memdb/internal/parser/CMakeFiles/parser.dir/flags.make
memdb/internal/parser/CMakeFiles/parser.dir/assignments.cpp.o: /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/assignments.cpp
memdb/internal/parser/CMakeFiles/parser.dir/assignments.cpp.o: memdb/internal/parser/CMakeFiles/parser.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/lehatrutenb/Desktop/prog/memdb/memdb_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object memdb/internal/parser/CMakeFiles/parser.dir/assignments.cpp.o"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT memdb/internal/parser/CMakeFiles/parser.dir/assignments.cpp.o -MF CMakeFiles/parser.dir/assignments.cpp.o.d -o CMakeFiles/parser.dir/assignments.cpp.o -c /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/assignments.cpp

memdb/internal/parser/CMakeFiles/parser.dir/assignments.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/parser.dir/assignments.cpp.i"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/assignments.cpp > CMakeFiles/parser.dir/assignments.cpp.i

memdb/internal/parser/CMakeFiles/parser.dir/assignments.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/parser.dir/assignments.cpp.s"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/assignments.cpp -o CMakeFiles/parser.dir/assignments.cpp.s

memdb/internal/parser/CMakeFiles/parser.dir/condition.cpp.o: memdb/internal/parser/CMakeFiles/parser.dir/flags.make
memdb/internal/parser/CMakeFiles/parser.dir/condition.cpp.o: /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/condition.cpp
memdb/internal/parser/CMakeFiles/parser.dir/condition.cpp.o: memdb/internal/parser/CMakeFiles/parser.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/lehatrutenb/Desktop/prog/memdb/memdb_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object memdb/internal/parser/CMakeFiles/parser.dir/condition.cpp.o"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT memdb/internal/parser/CMakeFiles/parser.dir/condition.cpp.o -MF CMakeFiles/parser.dir/condition.cpp.o.d -o CMakeFiles/parser.dir/condition.cpp.o -c /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/condition.cpp

memdb/internal/parser/CMakeFiles/parser.dir/condition.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/parser.dir/condition.cpp.i"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/condition.cpp > CMakeFiles/parser.dir/condition.cpp.i

memdb/internal/parser/CMakeFiles/parser.dir/condition.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/parser.dir/condition.cpp.s"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/condition.cpp -o CMakeFiles/parser.dir/condition.cpp.s

memdb/internal/parser/CMakeFiles/parser.dir/createColumns.cpp.o: memdb/internal/parser/CMakeFiles/parser.dir/flags.make
memdb/internal/parser/CMakeFiles/parser.dir/createColumns.cpp.o: /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/createColumns.cpp
memdb/internal/parser/CMakeFiles/parser.dir/createColumns.cpp.o: memdb/internal/parser/CMakeFiles/parser.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/lehatrutenb/Desktop/prog/memdb/memdb_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object memdb/internal/parser/CMakeFiles/parser.dir/createColumns.cpp.o"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT memdb/internal/parser/CMakeFiles/parser.dir/createColumns.cpp.o -MF CMakeFiles/parser.dir/createColumns.cpp.o.d -o CMakeFiles/parser.dir/createColumns.cpp.o -c /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/createColumns.cpp

memdb/internal/parser/CMakeFiles/parser.dir/createColumns.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/parser.dir/createColumns.cpp.i"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/createColumns.cpp > CMakeFiles/parser.dir/createColumns.cpp.i

memdb/internal/parser/CMakeFiles/parser.dir/createColumns.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/parser.dir/createColumns.cpp.s"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/createColumns.cpp -o CMakeFiles/parser.dir/createColumns.cpp.s

memdb/internal/parser/CMakeFiles/parser.dir/insertIndex.cpp.o: memdb/internal/parser/CMakeFiles/parser.dir/flags.make
memdb/internal/parser/CMakeFiles/parser.dir/insertIndex.cpp.o: /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/insertIndex.cpp
memdb/internal/parser/CMakeFiles/parser.dir/insertIndex.cpp.o: memdb/internal/parser/CMakeFiles/parser.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/lehatrutenb/Desktop/prog/memdb/memdb_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object memdb/internal/parser/CMakeFiles/parser.dir/insertIndex.cpp.o"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT memdb/internal/parser/CMakeFiles/parser.dir/insertIndex.cpp.o -MF CMakeFiles/parser.dir/insertIndex.cpp.o.d -o CMakeFiles/parser.dir/insertIndex.cpp.o -c /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/insertIndex.cpp

memdb/internal/parser/CMakeFiles/parser.dir/insertIndex.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/parser.dir/insertIndex.cpp.i"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/insertIndex.cpp > CMakeFiles/parser.dir/insertIndex.cpp.i

memdb/internal/parser/CMakeFiles/parser.dir/insertIndex.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/parser.dir/insertIndex.cpp.s"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/insertIndex.cpp -o CMakeFiles/parser.dir/insertIndex.cpp.s

memdb/internal/parser/CMakeFiles/parser.dir/selectValues.cpp.o: memdb/internal/parser/CMakeFiles/parser.dir/flags.make
memdb/internal/parser/CMakeFiles/parser.dir/selectValues.cpp.o: /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/selectValues.cpp
memdb/internal/parser/CMakeFiles/parser.dir/selectValues.cpp.o: memdb/internal/parser/CMakeFiles/parser.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/lehatrutenb/Desktop/prog/memdb/memdb_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object memdb/internal/parser/CMakeFiles/parser.dir/selectValues.cpp.o"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT memdb/internal/parser/CMakeFiles/parser.dir/selectValues.cpp.o -MF CMakeFiles/parser.dir/selectValues.cpp.o.d -o CMakeFiles/parser.dir/selectValues.cpp.o -c /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/selectValues.cpp

memdb/internal/parser/CMakeFiles/parser.dir/selectValues.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/parser.dir/selectValues.cpp.i"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/selectValues.cpp > CMakeFiles/parser.dir/selectValues.cpp.i

memdb/internal/parser/CMakeFiles/parser.dir/selectValues.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/parser.dir/selectValues.cpp.s"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/selectValues.cpp -o CMakeFiles/parser.dir/selectValues.cpp.s

memdb/internal/parser/CMakeFiles/parser.dir/parser.cpp.o: memdb/internal/parser/CMakeFiles/parser.dir/flags.make
memdb/internal/parser/CMakeFiles/parser.dir/parser.cpp.o: /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/parser.cpp
memdb/internal/parser/CMakeFiles/parser.dir/parser.cpp.o: memdb/internal/parser/CMakeFiles/parser.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/lehatrutenb/Desktop/prog/memdb/memdb_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object memdb/internal/parser/CMakeFiles/parser.dir/parser.cpp.o"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT memdb/internal/parser/CMakeFiles/parser.dir/parser.cpp.o -MF CMakeFiles/parser.dir/parser.cpp.o.d -o CMakeFiles/parser.dir/parser.cpp.o -c /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/parser.cpp

memdb/internal/parser/CMakeFiles/parser.dir/parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/parser.dir/parser.cpp.i"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/parser.cpp > CMakeFiles/parser.dir/parser.cpp.i

memdb/internal/parser/CMakeFiles/parser.dir/parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/parser.dir/parser.cpp.s"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser/parser.cpp -o CMakeFiles/parser.dir/parser.cpp.s

# Object files for target parser
parser_OBJECTS = \
"CMakeFiles/parser.dir/lexer.cpp.o" \
"CMakeFiles/parser.dir/assignments.cpp.o" \
"CMakeFiles/parser.dir/condition.cpp.o" \
"CMakeFiles/parser.dir/createColumns.cpp.o" \
"CMakeFiles/parser.dir/insertIndex.cpp.o" \
"CMakeFiles/parser.dir/selectValues.cpp.o" \
"CMakeFiles/parser.dir/parser.cpp.o"

# External object files for target parser
parser_EXTERNAL_OBJECTS =

memdb/internal/parser/libparser.a: memdb/internal/parser/CMakeFiles/parser.dir/lexer.cpp.o
memdb/internal/parser/libparser.a: memdb/internal/parser/CMakeFiles/parser.dir/assignments.cpp.o
memdb/internal/parser/libparser.a: memdb/internal/parser/CMakeFiles/parser.dir/condition.cpp.o
memdb/internal/parser/libparser.a: memdb/internal/parser/CMakeFiles/parser.dir/createColumns.cpp.o
memdb/internal/parser/libparser.a: memdb/internal/parser/CMakeFiles/parser.dir/insertIndex.cpp.o
memdb/internal/parser/libparser.a: memdb/internal/parser/CMakeFiles/parser.dir/selectValues.cpp.o
memdb/internal/parser/libparser.a: memdb/internal/parser/CMakeFiles/parser.dir/parser.cpp.o
memdb/internal/parser/libparser.a: memdb/internal/parser/CMakeFiles/parser.dir/build.make
memdb/internal/parser/libparser.a: memdb/internal/parser/CMakeFiles/parser.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/lehatrutenb/Desktop/prog/memdb/memdb_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX static library libparser.a"
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/parser && $(CMAKE_COMMAND) -P CMakeFiles/parser.dir/cmake_clean_target.cmake
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/parser && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/parser.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
memdb/internal/parser/CMakeFiles/parser.dir/build: memdb/internal/parser/libparser.a
.PHONY : memdb/internal/parser/CMakeFiles/parser.dir/build

memdb/internal/parser/CMakeFiles/parser.dir/clean:
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/parser && $(CMAKE_COMMAND) -P CMakeFiles/parser.dir/cmake_clean.cmake
.PHONY : memdb/internal/parser/CMakeFiles/parser.dir/clean

memdb/internal/parser/CMakeFiles/parser.dir/depend:
	cd /Users/lehatrutenb/Desktop/prog/memdb/memdb_build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/lehatrutenb/Desktop/prog/memdb /Users/lehatrutenb/Desktop/prog/memdb/memdb/internal/parser /Users/lehatrutenb/Desktop/prog/memdb/memdb_build /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/parser /Users/lehatrutenb/Desktop/prog/memdb/memdb_build/memdb/internal/parser/CMakeFiles/parser.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : memdb/internal/parser/CMakeFiles/parser.dir/depend
