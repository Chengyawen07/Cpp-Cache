# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/ubuntu/Cpp-Cache

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ubuntu/Cpp-Cache/build

# Include any dependencies generated for this target.
include test/CMakeFiles/test_LruCache.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/test_LruCache.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/test_LruCache.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/test_LruCache.dir/flags.make

test/CMakeFiles/test_LruCache.dir/test_LruCache.cpp.o: test/CMakeFiles/test_LruCache.dir/flags.make
test/CMakeFiles/test_LruCache.dir/test_LruCache.cpp.o: ../test/test_LruCache.cpp
test/CMakeFiles/test_LruCache.dir/test_LruCache.cpp.o: test/CMakeFiles/test_LruCache.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/Cpp-Cache/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/test_LruCache.dir/test_LruCache.cpp.o"
	cd /home/ubuntu/Cpp-Cache/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/test_LruCache.dir/test_LruCache.cpp.o -MF CMakeFiles/test_LruCache.dir/test_LruCache.cpp.o.d -o CMakeFiles/test_LruCache.dir/test_LruCache.cpp.o -c /home/ubuntu/Cpp-Cache/test/test_LruCache.cpp

test/CMakeFiles/test_LruCache.dir/test_LruCache.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_LruCache.dir/test_LruCache.cpp.i"
	cd /home/ubuntu/Cpp-Cache/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/Cpp-Cache/test/test_LruCache.cpp > CMakeFiles/test_LruCache.dir/test_LruCache.cpp.i

test/CMakeFiles/test_LruCache.dir/test_LruCache.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_LruCache.dir/test_LruCache.cpp.s"
	cd /home/ubuntu/Cpp-Cache/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/Cpp-Cache/test/test_LruCache.cpp -o CMakeFiles/test_LruCache.dir/test_LruCache.cpp.s

# Object files for target test_LruCache
test_LruCache_OBJECTS = \
"CMakeFiles/test_LruCache.dir/test_LruCache.cpp.o"

# External object files for target test_LruCache
test_LruCache_EXTERNAL_OBJECTS =

test/test_LruCache: test/CMakeFiles/test_LruCache.dir/test_LruCache.cpp.o
test/test_LruCache: test/CMakeFiles/test_LruCache.dir/build.make
test/test_LruCache: /usr/lib/x86_64-linux-gnu/libgtest_main.a
test/test_LruCache: /usr/lib/x86_64-linux-gnu/libgtest.a
test/test_LruCache: test/CMakeFiles/test_LruCache.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ubuntu/Cpp-Cache/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_LruCache"
	cd /home/ubuntu/Cpp-Cache/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_LruCache.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/test_LruCache.dir/build: test/test_LruCache
.PHONY : test/CMakeFiles/test_LruCache.dir/build

test/CMakeFiles/test_LruCache.dir/clean:
	cd /home/ubuntu/Cpp-Cache/build/test && $(CMAKE_COMMAND) -P CMakeFiles/test_LruCache.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/test_LruCache.dir/clean

test/CMakeFiles/test_LruCache.dir/depend:
	cd /home/ubuntu/Cpp-Cache/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/Cpp-Cache /home/ubuntu/Cpp-Cache/test /home/ubuntu/Cpp-Cache/build /home/ubuntu/Cpp-Cache/build/test /home/ubuntu/Cpp-Cache/build/test/CMakeFiles/test_LruCache.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/test_LruCache.dir/depend

