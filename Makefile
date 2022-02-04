# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

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
CMAKE_COMMAND = /home/frey/.local/bin/cmake

# The command to remove a file.
RM = /home/frey/.local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/frey/gh/indiana-jones-1978

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/frey/gh/indiana-jones-1978

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/home/frey/.local/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/home/frey/.local/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/frey/gh/indiana-jones-1978/CMakeFiles /home/frey/gh/indiana-jones-1978//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/frey/gh/indiana-jones-1978/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named IJ78

# Build rule for target.
IJ78: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 IJ78
.PHONY : IJ78

# fast build rule for target.
IJ78/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/build
.PHONY : IJ78/fast

#=============================================================================
# Target rules for targets named glad

# Build rule for target.
glad: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 glad
.PHONY : glad

# fast build rule for target.
glad/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/glad.dir/build.make CMakeFiles/glad.dir/build
.PHONY : glad/fast

#=============================================================================
# Target rules for targets named glfw

# Build rule for target.
glfw: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 glfw
.PHONY : glfw

# fast build rule for target.
glfw/fast:
	$(MAKE) $(MAKESILENT) -f libraries/glfw/src/CMakeFiles/glfw.dir/build.make libraries/glfw/src/CMakeFiles/glfw.dir/build
.PHONY : glfw/fast

libraries/glad/src/glad.o: libraries/glad/src/glad.c.o
.PHONY : libraries/glad/src/glad.o

# target to build an object file
libraries/glad/src/glad.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/glad.dir/build.make CMakeFiles/glad.dir/libraries/glad/src/glad.c.o
.PHONY : libraries/glad/src/glad.c.o

libraries/glad/src/glad.i: libraries/glad/src/glad.c.i
.PHONY : libraries/glad/src/glad.i

# target to preprocess a source file
libraries/glad/src/glad.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/glad.dir/build.make CMakeFiles/glad.dir/libraries/glad/src/glad.c.i
.PHONY : libraries/glad/src/glad.c.i

libraries/glad/src/glad.s: libraries/glad/src/glad.c.s
.PHONY : libraries/glad/src/glad.s

# target to generate assembly for a file
libraries/glad/src/glad.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/glad.dir/build.make CMakeFiles/glad.dir/libraries/glad/src/glad.c.s
.PHONY : libraries/glad/src/glad.c.s

src/game.o: src/game.cpp.o
.PHONY : src/game.o

# target to build an object file
src/game.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/game.cpp.o
.PHONY : src/game.cpp.o

src/game.i: src/game.cpp.i
.PHONY : src/game.i

# target to preprocess a source file
src/game.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/game.cpp.i
.PHONY : src/game.cpp.i

src/game.s: src/game.cpp.s
.PHONY : src/game.s

# target to generate assembly for a file
src/game.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/game.cpp.s
.PHONY : src/game.cpp.s

src/game_level.o: src/game_level.cpp.o
.PHONY : src/game_level.o

# target to build an object file
src/game_level.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/game_level.cpp.o
.PHONY : src/game_level.cpp.o

src/game_level.i: src/game_level.cpp.i
.PHONY : src/game_level.i

# target to preprocess a source file
src/game_level.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/game_level.cpp.i
.PHONY : src/game_level.cpp.i

src/game_level.s: src/game_level.cpp.s
.PHONY : src/game_level.s

# target to generate assembly for a file
src/game_level.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/game_level.cpp.s
.PHONY : src/game_level.cpp.s

src/game_object.o: src/game_object.cpp.o
.PHONY : src/game_object.o

# target to build an object file
src/game_object.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/game_object.cpp.o
.PHONY : src/game_object.cpp.o

src/game_object.i: src/game_object.cpp.i
.PHONY : src/game_object.i

# target to preprocess a source file
src/game_object.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/game_object.cpp.i
.PHONY : src/game_object.cpp.i

src/game_object.s: src/game_object.cpp.s
.PHONY : src/game_object.s

# target to generate assembly for a file
src/game_object.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/game_object.cpp.s
.PHONY : src/game_object.cpp.s

src/main.o: src/main.cpp.o
.PHONY : src/main.o

# target to build an object file
src/main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/main.cpp.o
.PHONY : src/main.cpp.o

src/main.i: src/main.cpp.i
.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s
.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

src/resource_manager.o: src/resource_manager.cpp.o
.PHONY : src/resource_manager.o

# target to build an object file
src/resource_manager.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/resource_manager.cpp.o
.PHONY : src/resource_manager.cpp.o

src/resource_manager.i: src/resource_manager.cpp.i
.PHONY : src/resource_manager.i

# target to preprocess a source file
src/resource_manager.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/resource_manager.cpp.i
.PHONY : src/resource_manager.cpp.i

src/resource_manager.s: src/resource_manager.cpp.s
.PHONY : src/resource_manager.s

# target to generate assembly for a file
src/resource_manager.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/resource_manager.cpp.s
.PHONY : src/resource_manager.cpp.s

src/shader.o: src/shader.cpp.o
.PHONY : src/shader.o

# target to build an object file
src/shader.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/shader.cpp.o
.PHONY : src/shader.cpp.o

src/shader.i: src/shader.cpp.i
.PHONY : src/shader.i

# target to preprocess a source file
src/shader.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/shader.cpp.i
.PHONY : src/shader.cpp.i

src/shader.s: src/shader.cpp.s
.PHONY : src/shader.s

# target to generate assembly for a file
src/shader.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/shader.cpp.s
.PHONY : src/shader.cpp.s

src/sprite_renderer.o: src/sprite_renderer.cpp.o
.PHONY : src/sprite_renderer.o

# target to build an object file
src/sprite_renderer.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/sprite_renderer.cpp.o
.PHONY : src/sprite_renderer.cpp.o

src/sprite_renderer.i: src/sprite_renderer.cpp.i
.PHONY : src/sprite_renderer.i

# target to preprocess a source file
src/sprite_renderer.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/sprite_renderer.cpp.i
.PHONY : src/sprite_renderer.cpp.i

src/sprite_renderer.s: src/sprite_renderer.cpp.s
.PHONY : src/sprite_renderer.s

# target to generate assembly for a file
src/sprite_renderer.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/sprite_renderer.cpp.s
.PHONY : src/sprite_renderer.cpp.s

src/stb_image.o: src/stb_image.cpp.o
.PHONY : src/stb_image.o

# target to build an object file
src/stb_image.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/stb_image.cpp.o
.PHONY : src/stb_image.cpp.o

src/stb_image.i: src/stb_image.cpp.i
.PHONY : src/stb_image.i

# target to preprocess a source file
src/stb_image.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/stb_image.cpp.i
.PHONY : src/stb_image.cpp.i

src/stb_image.s: src/stb_image.cpp.s
.PHONY : src/stb_image.s

# target to generate assembly for a file
src/stb_image.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/stb_image.cpp.s
.PHONY : src/stb_image.cpp.s

src/text_renderer.o: src/text_renderer.cpp.o
.PHONY : src/text_renderer.o

# target to build an object file
src/text_renderer.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/text_renderer.cpp.o
.PHONY : src/text_renderer.cpp.o

src/text_renderer.i: src/text_renderer.cpp.i
.PHONY : src/text_renderer.i

# target to preprocess a source file
src/text_renderer.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/text_renderer.cpp.i
.PHONY : src/text_renderer.cpp.i

src/text_renderer.s: src/text_renderer.cpp.s
.PHONY : src/text_renderer.s

# target to generate assembly for a file
src/text_renderer.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/text_renderer.cpp.s
.PHONY : src/text_renderer.cpp.s

src/texture.o: src/texture.cpp.o
.PHONY : src/texture.o

# target to build an object file
src/texture.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/texture.cpp.o
.PHONY : src/texture.cpp.o

src/texture.i: src/texture.cpp.i
.PHONY : src/texture.i

# target to preprocess a source file
src/texture.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/texture.cpp.i
.PHONY : src/texture.cpp.i

src/texture.s: src/texture.cpp.s
.PHONY : src/texture.s

# target to generate assembly for a file
src/texture.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/IJ78.dir/build.make CMakeFiles/IJ78.dir/src/texture.cpp.s
.PHONY : src/texture.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... IJ78"
	@echo "... glad"
	@echo "... glfw"
	@echo "... libraries/glad/src/glad.o"
	@echo "... libraries/glad/src/glad.i"
	@echo "... libraries/glad/src/glad.s"
	@echo "... src/game.o"
	@echo "... src/game.i"
	@echo "... src/game.s"
	@echo "... src/game_level.o"
	@echo "... src/game_level.i"
	@echo "... src/game_level.s"
	@echo "... src/game_object.o"
	@echo "... src/game_object.i"
	@echo "... src/game_object.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
	@echo "... src/resource_manager.o"
	@echo "... src/resource_manager.i"
	@echo "... src/resource_manager.s"
	@echo "... src/shader.o"
	@echo "... src/shader.i"
	@echo "... src/shader.s"
	@echo "... src/sprite_renderer.o"
	@echo "... src/sprite_renderer.i"
	@echo "... src/sprite_renderer.s"
	@echo "... src/stb_image.o"
	@echo "... src/stb_image.i"
	@echo "... src/stb_image.s"
	@echo "... src/text_renderer.o"
	@echo "... src/text_renderer.i"
	@echo "... src/text_renderer.s"
	@echo "... src/texture.o"
	@echo "... src/texture.i"
	@echo "... src/texture.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

