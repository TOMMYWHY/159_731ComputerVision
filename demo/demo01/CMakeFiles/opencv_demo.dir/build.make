# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = "/Users/Tommy/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/193.6911.21/CLion.app/Contents/bin/cmake/mac/bin/cmake"

# The command to remove a file.
RM = "/Users/Tommy/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/193.6911.21/CLion.app/Contents/bin/cmake/mac/bin/cmake" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/demo/demo01

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/demo/demo01

# Include any dependencies generated for this target.
include CMakeFiles/opencv_demo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/opencv_demo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/opencv_demo.dir/flags.make

CMakeFiles/opencv_demo.dir/week03/main.cpp.o: CMakeFiles/opencv_demo.dir/flags.make
CMakeFiles/opencv_demo.dir/week03/main.cpp.o: week03/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/demo/demo01/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/opencv_demo.dir/week03/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/opencv_demo.dir/week03/main.cpp.o -c /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/demo/demo01/week03/main.cpp

CMakeFiles/opencv_demo.dir/week03/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/opencv_demo.dir/week03/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/demo/demo01/week03/main.cpp > CMakeFiles/opencv_demo.dir/week03/main.cpp.i

CMakeFiles/opencv_demo.dir/week03/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/opencv_demo.dir/week03/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/demo/demo01/week03/main.cpp -o CMakeFiles/opencv_demo.dir/week03/main.cpp.s

# Object files for target opencv_demo
opencv_demo_OBJECTS = \
"CMakeFiles/opencv_demo.dir/week03/main.cpp.o"

# External object files for target opencv_demo
opencv_demo_EXTERNAL_OBJECTS =

opencv_demo: CMakeFiles/opencv_demo.dir/week03/main.cpp.o
opencv_demo: CMakeFiles/opencv_demo.dir/build.make
opencv_demo: /usr/local/lib/libopencv_gapi.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_stitching.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_aruco.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_bgsegm.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_bioinspired.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_ccalib.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_dnn_objdetect.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_dnn_superres.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_dpm.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_face.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_freetype.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_fuzzy.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_hfs.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_img_hash.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_line_descriptor.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_quality.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_reg.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_rgbd.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_saliency.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_sfm.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_stereo.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_structured_light.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_superres.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_surface_matching.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_tracking.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_videostab.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_xfeatures2d.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_xobjdetect.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_xphoto.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_highgui.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_shape.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_datasets.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_plot.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_text.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_dnn.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_ml.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_phase_unwrapping.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_optflow.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_ximgproc.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_video.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_videoio.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_imgcodecs.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_objdetect.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_calib3d.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_features2d.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_flann.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_photo.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_imgproc.4.2.0.dylib
opencv_demo: /usr/local/lib/libopencv_core.4.2.0.dylib
opencv_demo: CMakeFiles/opencv_demo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/demo/demo01/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable opencv_demo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/opencv_demo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/opencv_demo.dir/build: opencv_demo

.PHONY : CMakeFiles/opencv_demo.dir/build

CMakeFiles/opencv_demo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/opencv_demo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/opencv_demo.dir/clean

CMakeFiles/opencv_demo.dir/depend:
	cd /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/demo/demo01 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/demo/demo01 /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/demo/demo01 /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/demo/demo01 /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/demo/demo01 /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/demo/demo01/CMakeFiles/opencv_demo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/opencv_demo.dir/depend

