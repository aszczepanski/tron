# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kareth/shared

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kareth/shared

# Include any dependencies generated for this target.
include src/CMakeFiles/main.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/main.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/main.dir/flags.make

src/CMakeFiles/main.dir/main.cpp.o: src/CMakeFiles/main.dir/flags.make
src/CMakeFiles/main.dir/main.cpp.o: src/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/kareth/shared/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/CMakeFiles/main.dir/main.cpp.o"
	cd /home/kareth/shared/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/main.dir/main.cpp.o -c /home/kareth/shared/src/main.cpp

src/CMakeFiles/main.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/main.cpp.i"
	cd /home/kareth/shared/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/kareth/shared/src/main.cpp > CMakeFiles/main.dir/main.cpp.i

src/CMakeFiles/main.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/main.cpp.s"
	cd /home/kareth/shared/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/kareth/shared/src/main.cpp -o CMakeFiles/main.dir/main.cpp.s

src/CMakeFiles/main.dir/main.cpp.o.requires:
.PHONY : src/CMakeFiles/main.dir/main.cpp.o.requires

src/CMakeFiles/main.dir/main.cpp.o.provides: src/CMakeFiles/main.dir/main.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/main.dir/build.make src/CMakeFiles/main.dir/main.cpp.o.provides.build
.PHONY : src/CMakeFiles/main.dir/main.cpp.o.provides

src/CMakeFiles/main.dir/main.cpp.o.provides.build: src/CMakeFiles/main.dir/main.cpp.o

src/CMakeFiles/main.dir/common/ithread.cpp.o: src/CMakeFiles/main.dir/flags.make
src/CMakeFiles/main.dir/common/ithread.cpp.o: src/common/ithread.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/kareth/shared/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/CMakeFiles/main.dir/common/ithread.cpp.o"
	cd /home/kareth/shared/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/main.dir/common/ithread.cpp.o -c /home/kareth/shared/src/common/ithread.cpp

src/CMakeFiles/main.dir/common/ithread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/common/ithread.cpp.i"
	cd /home/kareth/shared/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/kareth/shared/src/common/ithread.cpp > CMakeFiles/main.dir/common/ithread.cpp.i

src/CMakeFiles/main.dir/common/ithread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/common/ithread.cpp.s"
	cd /home/kareth/shared/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/kareth/shared/src/common/ithread.cpp -o CMakeFiles/main.dir/common/ithread.cpp.s

src/CMakeFiles/main.dir/common/ithread.cpp.o.requires:
.PHONY : src/CMakeFiles/main.dir/common/ithread.cpp.o.requires

src/CMakeFiles/main.dir/common/ithread.cpp.o.provides: src/CMakeFiles/main.dir/common/ithread.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/main.dir/build.make src/CMakeFiles/main.dir/common/ithread.cpp.o.provides.build
.PHONY : src/CMakeFiles/main.dir/common/ithread.cpp.o.provides

src/CMakeFiles/main.dir/common/ithread.cpp.o.provides.build: src/CMakeFiles/main.dir/common/ithread.cpp.o

src/CMakeFiles/main.dir/common/logger.cpp.o: src/CMakeFiles/main.dir/flags.make
src/CMakeFiles/main.dir/common/logger.cpp.o: src/common/logger.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/kareth/shared/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/CMakeFiles/main.dir/common/logger.cpp.o"
	cd /home/kareth/shared/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/main.dir/common/logger.cpp.o -c /home/kareth/shared/src/common/logger.cpp

src/CMakeFiles/main.dir/common/logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/common/logger.cpp.i"
	cd /home/kareth/shared/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/kareth/shared/src/common/logger.cpp > CMakeFiles/main.dir/common/logger.cpp.i

src/CMakeFiles/main.dir/common/logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/common/logger.cpp.s"
	cd /home/kareth/shared/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/kareth/shared/src/common/logger.cpp -o CMakeFiles/main.dir/common/logger.cpp.s

src/CMakeFiles/main.dir/common/logger.cpp.o.requires:
.PHONY : src/CMakeFiles/main.dir/common/logger.cpp.o.requires

src/CMakeFiles/main.dir/common/logger.cpp.o.provides: src/CMakeFiles/main.dir/common/logger.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/main.dir/build.make src/CMakeFiles/main.dir/common/logger.cpp.o.provides.build
.PHONY : src/CMakeFiles/main.dir/common/logger.cpp.o.provides

src/CMakeFiles/main.dir/common/logger.cpp.o.provides.build: src/CMakeFiles/main.dir/common/logger.cpp.o

src/CMakeFiles/main.dir/common/mutex.cpp.o: src/CMakeFiles/main.dir/flags.make
src/CMakeFiles/main.dir/common/mutex.cpp.o: src/common/mutex.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/kareth/shared/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/CMakeFiles/main.dir/common/mutex.cpp.o"
	cd /home/kareth/shared/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/main.dir/common/mutex.cpp.o -c /home/kareth/shared/src/common/mutex.cpp

src/CMakeFiles/main.dir/common/mutex.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/common/mutex.cpp.i"
	cd /home/kareth/shared/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/kareth/shared/src/common/mutex.cpp > CMakeFiles/main.dir/common/mutex.cpp.i

src/CMakeFiles/main.dir/common/mutex.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/common/mutex.cpp.s"
	cd /home/kareth/shared/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/kareth/shared/src/common/mutex.cpp -o CMakeFiles/main.dir/common/mutex.cpp.s

src/CMakeFiles/main.dir/common/mutex.cpp.o.requires:
.PHONY : src/CMakeFiles/main.dir/common/mutex.cpp.o.requires

src/CMakeFiles/main.dir/common/mutex.cpp.o.provides: src/CMakeFiles/main.dir/common/mutex.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/main.dir/build.make src/CMakeFiles/main.dir/common/mutex.cpp.o.provides.build
.PHONY : src/CMakeFiles/main.dir/common/mutex.cpp.o.provides

src/CMakeFiles/main.dir/common/mutex.cpp.o.provides.build: src/CMakeFiles/main.dir/common/mutex.cpp.o

# Object files for target main
main_OBJECTS = \
"CMakeFiles/main.dir/main.cpp.o" \
"CMakeFiles/main.dir/common/ithread.cpp.o" \
"CMakeFiles/main.dir/common/logger.cpp.o" \
"CMakeFiles/main.dir/common/mutex.cpp.o"

# External object files for target main
main_EXTERNAL_OBJECTS =

main: src/CMakeFiles/main.dir/main.cpp.o
main: src/CMakeFiles/main.dir/common/ithread.cpp.o
main: src/CMakeFiles/main.dir/common/logger.cpp.o
main: src/CMakeFiles/main.dir/common/mutex.cpp.o
main: lib/libapplication.a
main: lib/libserver.a
main: lib/libclient.a
main: src/CMakeFiles/main.dir/build.make
main: src/CMakeFiles/main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../main"
	cd /home/kareth/shared/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/main.dir/build: main
.PHONY : src/CMakeFiles/main.dir/build

src/CMakeFiles/main.dir/requires: src/CMakeFiles/main.dir/main.cpp.o.requires
src/CMakeFiles/main.dir/requires: src/CMakeFiles/main.dir/common/ithread.cpp.o.requires
src/CMakeFiles/main.dir/requires: src/CMakeFiles/main.dir/common/logger.cpp.o.requires
src/CMakeFiles/main.dir/requires: src/CMakeFiles/main.dir/common/mutex.cpp.o.requires
.PHONY : src/CMakeFiles/main.dir/requires

src/CMakeFiles/main.dir/clean:
	cd /home/kareth/shared/src && $(CMAKE_COMMAND) -P CMakeFiles/main.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/main.dir/clean

src/CMakeFiles/main.dir/depend:
	cd /home/kareth/shared && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kareth/shared /home/kareth/shared/src /home/kareth/shared /home/kareth/shared/src /home/kareth/shared/src/CMakeFiles/main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/main.dir/depend

