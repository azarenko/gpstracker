# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/user/projects/gpstracker

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/user/projects/gpstracker/build

# Include any dependencies generated for this target.
include CMakeFiles/gpstracker.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/gpstracker.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/gpstracker.dir/flags.make

CMakeFiles/gpstracker.dir/main.c.o: CMakeFiles/gpstracker.dir/flags.make
CMakeFiles/gpstracker.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/projects/gpstracker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/gpstracker.dir/main.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/gpstracker.dir/main.c.o   -c /home/user/projects/gpstracker/main.c

CMakeFiles/gpstracker.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gpstracker.dir/main.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/projects/gpstracker/main.c > CMakeFiles/gpstracker.dir/main.c.i

CMakeFiles/gpstracker.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gpstracker.dir/main.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/projects/gpstracker/main.c -o CMakeFiles/gpstracker.dir/main.c.s

CMakeFiles/gpstracker.dir/main.c.o.requires:

.PHONY : CMakeFiles/gpstracker.dir/main.c.o.requires

CMakeFiles/gpstracker.dir/main.c.o.provides: CMakeFiles/gpstracker.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/gpstracker.dir/build.make CMakeFiles/gpstracker.dir/main.c.o.provides.build
.PHONY : CMakeFiles/gpstracker.dir/main.c.o.provides

CMakeFiles/gpstracker.dir/main.c.o.provides.build: CMakeFiles/gpstracker.dir/main.c.o


CMakeFiles/gpstracker.dir/cmdparam.c.o: CMakeFiles/gpstracker.dir/flags.make
CMakeFiles/gpstracker.dir/cmdparam.c.o: ../cmdparam.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/projects/gpstracker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/gpstracker.dir/cmdparam.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/gpstracker.dir/cmdparam.c.o   -c /home/user/projects/gpstracker/cmdparam.c

CMakeFiles/gpstracker.dir/cmdparam.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gpstracker.dir/cmdparam.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/projects/gpstracker/cmdparam.c > CMakeFiles/gpstracker.dir/cmdparam.c.i

CMakeFiles/gpstracker.dir/cmdparam.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gpstracker.dir/cmdparam.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/projects/gpstracker/cmdparam.c -o CMakeFiles/gpstracker.dir/cmdparam.c.s

CMakeFiles/gpstracker.dir/cmdparam.c.o.requires:

.PHONY : CMakeFiles/gpstracker.dir/cmdparam.c.o.requires

CMakeFiles/gpstracker.dir/cmdparam.c.o.provides: CMakeFiles/gpstracker.dir/cmdparam.c.o.requires
	$(MAKE) -f CMakeFiles/gpstracker.dir/build.make CMakeFiles/gpstracker.dir/cmdparam.c.o.provides.build
.PHONY : CMakeFiles/gpstracker.dir/cmdparam.c.o.provides

CMakeFiles/gpstracker.dir/cmdparam.c.o.provides.build: CMakeFiles/gpstracker.dir/cmdparam.c.o


CMakeFiles/gpstracker.dir/settings.c.o: CMakeFiles/gpstracker.dir/flags.make
CMakeFiles/gpstracker.dir/settings.c.o: ../settings.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/projects/gpstracker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/gpstracker.dir/settings.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/gpstracker.dir/settings.c.o   -c /home/user/projects/gpstracker/settings.c

CMakeFiles/gpstracker.dir/settings.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gpstracker.dir/settings.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/projects/gpstracker/settings.c > CMakeFiles/gpstracker.dir/settings.c.i

CMakeFiles/gpstracker.dir/settings.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gpstracker.dir/settings.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/projects/gpstracker/settings.c -o CMakeFiles/gpstracker.dir/settings.c.s

CMakeFiles/gpstracker.dir/settings.c.o.requires:

.PHONY : CMakeFiles/gpstracker.dir/settings.c.o.requires

CMakeFiles/gpstracker.dir/settings.c.o.provides: CMakeFiles/gpstracker.dir/settings.c.o.requires
	$(MAKE) -f CMakeFiles/gpstracker.dir/build.make CMakeFiles/gpstracker.dir/settings.c.o.provides.build
.PHONY : CMakeFiles/gpstracker.dir/settings.c.o.provides

CMakeFiles/gpstracker.dir/settings.c.o.provides.build: CMakeFiles/gpstracker.dir/settings.c.o


CMakeFiles/gpstracker.dir/sockutils.c.o: CMakeFiles/gpstracker.dir/flags.make
CMakeFiles/gpstracker.dir/sockutils.c.o: ../sockutils.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/projects/gpstracker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/gpstracker.dir/sockutils.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/gpstracker.dir/sockutils.c.o   -c /home/user/projects/gpstracker/sockutils.c

CMakeFiles/gpstracker.dir/sockutils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gpstracker.dir/sockutils.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/projects/gpstracker/sockutils.c > CMakeFiles/gpstracker.dir/sockutils.c.i

CMakeFiles/gpstracker.dir/sockutils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gpstracker.dir/sockutils.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/projects/gpstracker/sockutils.c -o CMakeFiles/gpstracker.dir/sockutils.c.s

CMakeFiles/gpstracker.dir/sockutils.c.o.requires:

.PHONY : CMakeFiles/gpstracker.dir/sockutils.c.o.requires

CMakeFiles/gpstracker.dir/sockutils.c.o.provides: CMakeFiles/gpstracker.dir/sockutils.c.o.requires
	$(MAKE) -f CMakeFiles/gpstracker.dir/build.make CMakeFiles/gpstracker.dir/sockutils.c.o.provides.build
.PHONY : CMakeFiles/gpstracker.dir/sockutils.c.o.provides

CMakeFiles/gpstracker.dir/sockutils.c.o.provides.build: CMakeFiles/gpstracker.dir/sockutils.c.o


CMakeFiles/gpstracker.dir/fifo.c.o: CMakeFiles/gpstracker.dir/flags.make
CMakeFiles/gpstracker.dir/fifo.c.o: ../fifo.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/projects/gpstracker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/gpstracker.dir/fifo.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/gpstracker.dir/fifo.c.o   -c /home/user/projects/gpstracker/fifo.c

CMakeFiles/gpstracker.dir/fifo.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gpstracker.dir/fifo.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/projects/gpstracker/fifo.c > CMakeFiles/gpstracker.dir/fifo.c.i

CMakeFiles/gpstracker.dir/fifo.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gpstracker.dir/fifo.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/projects/gpstracker/fifo.c -o CMakeFiles/gpstracker.dir/fifo.c.s

CMakeFiles/gpstracker.dir/fifo.c.o.requires:

.PHONY : CMakeFiles/gpstracker.dir/fifo.c.o.requires

CMakeFiles/gpstracker.dir/fifo.c.o.provides: CMakeFiles/gpstracker.dir/fifo.c.o.requires
	$(MAKE) -f CMakeFiles/gpstracker.dir/build.make CMakeFiles/gpstracker.dir/fifo.c.o.provides.build
.PHONY : CMakeFiles/gpstracker.dir/fifo.c.o.provides

CMakeFiles/gpstracker.dir/fifo.c.o.provides.build: CMakeFiles/gpstracker.dir/fifo.c.o


CMakeFiles/gpstracker.dir/devices.c.o: CMakeFiles/gpstracker.dir/flags.make
CMakeFiles/gpstracker.dir/devices.c.o: ../devices.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/projects/gpstracker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/gpstracker.dir/devices.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/gpstracker.dir/devices.c.o   -c /home/user/projects/gpstracker/devices.c

CMakeFiles/gpstracker.dir/devices.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gpstracker.dir/devices.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/projects/gpstracker/devices.c > CMakeFiles/gpstracker.dir/devices.c.i

CMakeFiles/gpstracker.dir/devices.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gpstracker.dir/devices.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/projects/gpstracker/devices.c -o CMakeFiles/gpstracker.dir/devices.c.s

CMakeFiles/gpstracker.dir/devices.c.o.requires:

.PHONY : CMakeFiles/gpstracker.dir/devices.c.o.requires

CMakeFiles/gpstracker.dir/devices.c.o.provides: CMakeFiles/gpstracker.dir/devices.c.o.requires
	$(MAKE) -f CMakeFiles/gpstracker.dir/build.make CMakeFiles/gpstracker.dir/devices.c.o.provides.build
.PHONY : CMakeFiles/gpstracker.dir/devices.c.o.provides

CMakeFiles/gpstracker.dir/devices.c.o.provides.build: CMakeFiles/gpstracker.dir/devices.c.o


CMakeFiles/gpstracker.dir/devices/fm1100.c.o: CMakeFiles/gpstracker.dir/flags.make
CMakeFiles/gpstracker.dir/devices/fm1100.c.o: ../devices/fm1100.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/projects/gpstracker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/gpstracker.dir/devices/fm1100.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/gpstracker.dir/devices/fm1100.c.o   -c /home/user/projects/gpstracker/devices/fm1100.c

CMakeFiles/gpstracker.dir/devices/fm1100.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gpstracker.dir/devices/fm1100.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/user/projects/gpstracker/devices/fm1100.c > CMakeFiles/gpstracker.dir/devices/fm1100.c.i

CMakeFiles/gpstracker.dir/devices/fm1100.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gpstracker.dir/devices/fm1100.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/user/projects/gpstracker/devices/fm1100.c -o CMakeFiles/gpstracker.dir/devices/fm1100.c.s

CMakeFiles/gpstracker.dir/devices/fm1100.c.o.requires:

.PHONY : CMakeFiles/gpstracker.dir/devices/fm1100.c.o.requires

CMakeFiles/gpstracker.dir/devices/fm1100.c.o.provides: CMakeFiles/gpstracker.dir/devices/fm1100.c.o.requires
	$(MAKE) -f CMakeFiles/gpstracker.dir/build.make CMakeFiles/gpstracker.dir/devices/fm1100.c.o.provides.build
.PHONY : CMakeFiles/gpstracker.dir/devices/fm1100.c.o.provides

CMakeFiles/gpstracker.dir/devices/fm1100.c.o.provides.build: CMakeFiles/gpstracker.dir/devices/fm1100.c.o


# Object files for target gpstracker
gpstracker_OBJECTS = \
"CMakeFiles/gpstracker.dir/main.c.o" \
"CMakeFiles/gpstracker.dir/cmdparam.c.o" \
"CMakeFiles/gpstracker.dir/settings.c.o" \
"CMakeFiles/gpstracker.dir/sockutils.c.o" \
"CMakeFiles/gpstracker.dir/fifo.c.o" \
"CMakeFiles/gpstracker.dir/devices.c.o" \
"CMakeFiles/gpstracker.dir/devices/fm1100.c.o"

# External object files for target gpstracker
gpstracker_EXTERNAL_OBJECTS =

gpstracker: CMakeFiles/gpstracker.dir/main.c.o
gpstracker: CMakeFiles/gpstracker.dir/cmdparam.c.o
gpstracker: CMakeFiles/gpstracker.dir/settings.c.o
gpstracker: CMakeFiles/gpstracker.dir/sockutils.c.o
gpstracker: CMakeFiles/gpstracker.dir/fifo.c.o
gpstracker: CMakeFiles/gpstracker.dir/devices.c.o
gpstracker: CMakeFiles/gpstracker.dir/devices/fm1100.c.o
gpstracker: CMakeFiles/gpstracker.dir/build.make
gpstracker: CMakeFiles/gpstracker.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/user/projects/gpstracker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking C executable gpstracker"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gpstracker.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/gpstracker.dir/build: gpstracker

.PHONY : CMakeFiles/gpstracker.dir/build

CMakeFiles/gpstracker.dir/requires: CMakeFiles/gpstracker.dir/main.c.o.requires
CMakeFiles/gpstracker.dir/requires: CMakeFiles/gpstracker.dir/cmdparam.c.o.requires
CMakeFiles/gpstracker.dir/requires: CMakeFiles/gpstracker.dir/settings.c.o.requires
CMakeFiles/gpstracker.dir/requires: CMakeFiles/gpstracker.dir/sockutils.c.o.requires
CMakeFiles/gpstracker.dir/requires: CMakeFiles/gpstracker.dir/fifo.c.o.requires
CMakeFiles/gpstracker.dir/requires: CMakeFiles/gpstracker.dir/devices.c.o.requires
CMakeFiles/gpstracker.dir/requires: CMakeFiles/gpstracker.dir/devices/fm1100.c.o.requires

.PHONY : CMakeFiles/gpstracker.dir/requires

CMakeFiles/gpstracker.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/gpstracker.dir/cmake_clean.cmake
.PHONY : CMakeFiles/gpstracker.dir/clean

CMakeFiles/gpstracker.dir/depend:
	cd /home/user/projects/gpstracker/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/user/projects/gpstracker /home/user/projects/gpstracker /home/user/projects/gpstracker/build /home/user/projects/gpstracker/build /home/user/projects/gpstracker/build/CMakeFiles/gpstracker.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/gpstracker.dir/depend
