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
CMAKE_SOURCE_DIR = /home/cory/Workspace/luaclip

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cory/Workspace/luaclip/build

# Include any dependencies generated for this target.
include CMakeFiles/luaclip.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/luaclip.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/luaclip.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/luaclip.dir/flags.make

CMakeFiles/luaclip.dir/src/buffer.cpp.o: CMakeFiles/luaclip.dir/flags.make
CMakeFiles/luaclip.dir/src/buffer.cpp.o: /home/cory/Workspace/luaclip/src/buffer.cpp
CMakeFiles/luaclip.dir/src/buffer.cpp.o: CMakeFiles/luaclip.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/cory/Workspace/luaclip/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/luaclip.dir/src/buffer.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/luaclip.dir/src/buffer.cpp.o -MF CMakeFiles/luaclip.dir/src/buffer.cpp.o.d -o CMakeFiles/luaclip.dir/src/buffer.cpp.o -c /home/cory/Workspace/luaclip/src/buffer.cpp

CMakeFiles/luaclip.dir/src/buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/luaclip.dir/src/buffer.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cory/Workspace/luaclip/src/buffer.cpp > CMakeFiles/luaclip.dir/src/buffer.cpp.i

CMakeFiles/luaclip.dir/src/buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/luaclip.dir/src/buffer.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cory/Workspace/luaclip/src/buffer.cpp -o CMakeFiles/luaclip.dir/src/buffer.cpp.s

CMakeFiles/luaclip.dir/src/luaclip.cpp.o: CMakeFiles/luaclip.dir/flags.make
CMakeFiles/luaclip.dir/src/luaclip.cpp.o: /home/cory/Workspace/luaclip/src/luaclip.cpp
CMakeFiles/luaclip.dir/src/luaclip.cpp.o: CMakeFiles/luaclip.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/cory/Workspace/luaclip/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/luaclip.dir/src/luaclip.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/luaclip.dir/src/luaclip.cpp.o -MF CMakeFiles/luaclip.dir/src/luaclip.cpp.o.d -o CMakeFiles/luaclip.dir/src/luaclip.cpp.o -c /home/cory/Workspace/luaclip/src/luaclip.cpp

CMakeFiles/luaclip.dir/src/luaclip.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/luaclip.dir/src/luaclip.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cory/Workspace/luaclip/src/luaclip.cpp > CMakeFiles/luaclip.dir/src/luaclip.cpp.i

CMakeFiles/luaclip.dir/src/luaclip.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/luaclip.dir/src/luaclip.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cory/Workspace/luaclip/src/luaclip.cpp -o CMakeFiles/luaclip.dir/src/luaclip.cpp.s

# Object files for target luaclip
luaclip_OBJECTS = \
"CMakeFiles/luaclip.dir/src/buffer.cpp.o" \
"CMakeFiles/luaclip.dir/src/luaclip.cpp.o"

# External object files for target luaclip
luaclip_EXTERNAL_OBJECTS =

libluaclip.so: CMakeFiles/luaclip.dir/src/buffer.cpp.o
libluaclip.so: CMakeFiles/luaclip.dir/src/luaclip.cpp.o
libluaclip.so: CMakeFiles/luaclip.dir/build.make
libluaclip.so: /usr/lib/liblua5.4.so
libluaclip.so: /usr/lib/libm.so
libluaclip.so: CMakeFiles/luaclip.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/cory/Workspace/luaclip/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library libluaclip.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/luaclip.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/luaclip.dir/build: libluaclip.so
.PHONY : CMakeFiles/luaclip.dir/build

CMakeFiles/luaclip.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/luaclip.dir/cmake_clean.cmake
.PHONY : CMakeFiles/luaclip.dir/clean

CMakeFiles/luaclip.dir/depend:
	cd /home/cory/Workspace/luaclip/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cory/Workspace/luaclip /home/cory/Workspace/luaclip /home/cory/Workspace/luaclip/build /home/cory/Workspace/luaclip/build /home/cory/Workspace/luaclip/build/CMakeFiles/luaclip.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/luaclip.dir/depend

