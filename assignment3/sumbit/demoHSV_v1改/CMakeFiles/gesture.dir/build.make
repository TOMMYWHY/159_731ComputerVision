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
CMAKE_SOURCE_DIR = /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment3/sumbit/demoHSV_v1改

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment3/sumbit/demoHSV_v1改

# Include any dependencies generated for this target.
include CMakeFiles/gesture.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/gesture.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/gesture.dir/flags.make

CMakeFiles/gesture.dir/src/demo.cpp.o: CMakeFiles/gesture.dir/flags.make
CMakeFiles/gesture.dir/src/demo.cpp.o: src/demo.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment3/sumbit/demoHSV_v1改/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/gesture.dir/src/demo.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gesture.dir/src/demo.cpp.o -c /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment3/sumbit/demoHSV_v1改/src/demo.cpp

CMakeFiles/gesture.dir/src/demo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gesture.dir/src/demo.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment3/sumbit/demoHSV_v1改/src/demo.cpp > CMakeFiles/gesture.dir/src/demo.cpp.i

CMakeFiles/gesture.dir/src/demo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gesture.dir/src/demo.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment3/sumbit/demoHSV_v1改/src/demo.cpp -o CMakeFiles/gesture.dir/src/demo.cpp.s

CMakeFiles/gesture.dir/src/LoadData.cpp.o: CMakeFiles/gesture.dir/flags.make
CMakeFiles/gesture.dir/src/LoadData.cpp.o: src/LoadData.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment3/sumbit/demoHSV_v1改/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/gesture.dir/src/LoadData.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gesture.dir/src/LoadData.cpp.o -c /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment3/sumbit/demoHSV_v1改/src/LoadData.cpp

CMakeFiles/gesture.dir/src/LoadData.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gesture.dir/src/LoadData.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment3/sumbit/demoHSV_v1改/src/LoadData.cpp > CMakeFiles/gesture.dir/src/LoadData.cpp.i

CMakeFiles/gesture.dir/src/LoadData.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gesture.dir/src/LoadData.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment3/sumbit/demoHSV_v1改/src/LoadData.cpp -o CMakeFiles/gesture.dir/src/LoadData.cpp.s

# Object files for target gesture
gesture_OBJECTS = \
"CMakeFiles/gesture.dir/src/demo.cpp.o" \
"CMakeFiles/gesture.dir/src/LoadData.cpp.o"

# External object files for target gesture
gesture_EXTERNAL_OBJECTS =

gesture: CMakeFiles/gesture.dir/src/demo.cpp.o
gesture: CMakeFiles/gesture.dir/src/LoadData.cpp.o
gesture: CMakeFiles/gesture.dir/build.make
gesture: /usr/local/lib/libopencv_gapi.4.2.0.dylib
gesture: /usr/local/lib/libopencv_stitching.4.2.0.dylib
gesture: /usr/local/lib/libopencv_aruco.4.2.0.dylib
gesture: /usr/local/lib/libopencv_bgsegm.4.2.0.dylib
gesture: /usr/local/lib/libopencv_bioinspired.4.2.0.dylib
gesture: /usr/local/lib/libopencv_ccalib.4.2.0.dylib
gesture: /usr/local/lib/libopencv_dnn_objdetect.4.2.0.dylib
gesture: /usr/local/lib/libopencv_dnn_superres.4.2.0.dylib
gesture: /usr/local/lib/libopencv_dpm.4.2.0.dylib
gesture: /usr/local/lib/libopencv_face.4.2.0.dylib
gesture: /usr/local/lib/libopencv_freetype.4.2.0.dylib
gesture: /usr/local/lib/libopencv_fuzzy.4.2.0.dylib
gesture: /usr/local/lib/libopencv_hfs.4.2.0.dylib
gesture: /usr/local/lib/libopencv_img_hash.4.2.0.dylib
gesture: /usr/local/lib/libopencv_line_descriptor.4.2.0.dylib
gesture: /usr/local/lib/libopencv_quality.4.2.0.dylib
gesture: /usr/local/lib/libopencv_reg.4.2.0.dylib
gesture: /usr/local/lib/libopencv_rgbd.4.2.0.dylib
gesture: /usr/local/lib/libopencv_saliency.4.2.0.dylib
gesture: /usr/local/lib/libopencv_sfm.4.2.0.dylib
gesture: /usr/local/lib/libopencv_stereo.4.2.0.dylib
gesture: /usr/local/lib/libopencv_structured_light.4.2.0.dylib
gesture: /usr/local/lib/libopencv_superres.4.2.0.dylib
gesture: /usr/local/lib/libopencv_surface_matching.4.2.0.dylib
gesture: /usr/local/lib/libopencv_tracking.4.2.0.dylib
gesture: /usr/local/lib/libopencv_videostab.4.2.0.dylib
gesture: /usr/local/lib/libopencv_xfeatures2d.4.2.0.dylib
gesture: /usr/local/lib/libopencv_xobjdetect.4.2.0.dylib
gesture: /usr/local/lib/libopencv_xphoto.4.2.0.dylib
gesture: /usr/local/lib/libopencv_highgui.4.2.0.dylib
gesture: /usr/local/lib/libopencv_shape.4.2.0.dylib
gesture: /usr/local/lib/libopencv_datasets.4.2.0.dylib
gesture: /usr/local/lib/libopencv_plot.4.2.0.dylib
gesture: /usr/local/lib/libopencv_text.4.2.0.dylib
gesture: /usr/local/lib/libopencv_dnn.4.2.0.dylib
gesture: /usr/local/lib/libopencv_ml.4.2.0.dylib
gesture: /usr/local/lib/libopencv_phase_unwrapping.4.2.0.dylib
gesture: /usr/local/lib/libopencv_optflow.4.2.0.dylib
gesture: /usr/local/lib/libopencv_ximgproc.4.2.0.dylib
gesture: /usr/local/lib/libopencv_video.4.2.0.dylib
gesture: /usr/local/lib/libopencv_videoio.4.2.0.dylib
gesture: /usr/local/lib/libopencv_imgcodecs.4.2.0.dylib
gesture: /usr/local/lib/libopencv_objdetect.4.2.0.dylib
gesture: /usr/local/lib/libopencv_calib3d.4.2.0.dylib
gesture: /usr/local/lib/libopencv_features2d.4.2.0.dylib
gesture: /usr/local/lib/libopencv_flann.4.2.0.dylib
gesture: /usr/local/lib/libopencv_photo.4.2.0.dylib
gesture: /usr/local/lib/libopencv_imgproc.4.2.0.dylib
gesture: /usr/local/lib/libopencv_core.4.2.0.dylib
gesture: CMakeFiles/gesture.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment3/sumbit/demoHSV_v1改/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable gesture"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gesture.dir/link.txt --verbose=$(VERBOSE)
	"/Users/Tommy/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/193.6911.21/CLion.app/Contents/bin/cmake/mac/bin/cmake" -E copy_directory /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment3/sumbit/demoHSV_v1改/res /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment3/sumbit/demoHSV_v1改/res

# Rule to build all files generated by this target.
CMakeFiles/gesture.dir/build: gesture

.PHONY : CMakeFiles/gesture.dir/build

CMakeFiles/gesture.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/gesture.dir/cmake_clean.cmake
.PHONY : CMakeFiles/gesture.dir/clean

CMakeFiles/gesture.dir/depend:
	cd /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment3/sumbit/demoHSV_v1改 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment3/sumbit/demoHSV_v1改 /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment3/sumbit/demoHSV_v1改 /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment3/sumbit/demoHSV_v1改 /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment3/sumbit/demoHSV_v1改 /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment3/sumbit/demoHSV_v1改/CMakeFiles/gesture.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/gesture.dir/depend

