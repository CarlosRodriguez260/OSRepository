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
CMAKE_SOURCE_DIR = /home/juan/Documents/OS/class_examples/complete_examples/ipc/mq_receiver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/juan/Documents/OS/class_examples/complete_examples/ipc/mq_receiver/build

# Include any dependencies generated for this target.
include CMakeFiles/mq_receiver.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/mq_receiver.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/mq_receiver.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mq_receiver.dir/flags.make

CMakeFiles/mq_receiver.dir/src/main.c.o: CMakeFiles/mq_receiver.dir/flags.make
CMakeFiles/mq_receiver.dir/src/main.c.o: /home/juan/Documents/OS/class_examples/complete_examples/ipc/mq_receiver/src/main.c
CMakeFiles/mq_receiver.dir/src/main.c.o: CMakeFiles/mq_receiver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/juan/Documents/OS/class_examples/complete_examples/ipc/mq_receiver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/mq_receiver.dir/src/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/mq_receiver.dir/src/main.c.o -MF CMakeFiles/mq_receiver.dir/src/main.c.o.d -o CMakeFiles/mq_receiver.dir/src/main.c.o -c /home/juan/Documents/OS/class_examples/complete_examples/ipc/mq_receiver/src/main.c

CMakeFiles/mq_receiver.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/mq_receiver.dir/src/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/juan/Documents/OS/class_examples/complete_examples/ipc/mq_receiver/src/main.c > CMakeFiles/mq_receiver.dir/src/main.c.i

CMakeFiles/mq_receiver.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/mq_receiver.dir/src/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/juan/Documents/OS/class_examples/complete_examples/ipc/mq_receiver/src/main.c -o CMakeFiles/mq_receiver.dir/src/main.c.s

# Object files for target mq_receiver
mq_receiver_OBJECTS = \
"CMakeFiles/mq_receiver.dir/src/main.c.o"

# External object files for target mq_receiver
mq_receiver_EXTERNAL_OBJECTS =

mq_receiver: CMakeFiles/mq_receiver.dir/src/main.c.o
mq_receiver: CMakeFiles/mq_receiver.dir/build.make
mq_receiver: CMakeFiles/mq_receiver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/juan/Documents/OS/class_examples/complete_examples/ipc/mq_receiver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable mq_receiver"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mq_receiver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mq_receiver.dir/build: mq_receiver
.PHONY : CMakeFiles/mq_receiver.dir/build

CMakeFiles/mq_receiver.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mq_receiver.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mq_receiver.dir/clean

CMakeFiles/mq_receiver.dir/depend:
	cd /home/juan/Documents/OS/class_examples/complete_examples/ipc/mq_receiver/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/juan/Documents/OS/class_examples/complete_examples/ipc/mq_receiver /home/juan/Documents/OS/class_examples/complete_examples/ipc/mq_receiver /home/juan/Documents/OS/class_examples/complete_examples/ipc/mq_receiver/build /home/juan/Documents/OS/class_examples/complete_examples/ipc/mq_receiver/build /home/juan/Documents/OS/class_examples/complete_examples/ipc/mq_receiver/build/CMakeFiles/mq_receiver.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/mq_receiver.dir/depend

