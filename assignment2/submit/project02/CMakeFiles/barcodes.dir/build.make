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
CMAKE_SOURCE_DIR = /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment2/submit/project02

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment2/submit/project02

# Include any dependencies generated for this target.
include CMakeFiles/barcodes.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/barcodes.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/barcodes.dir/flags.make

CMakeFiles/barcodes.dir/src/main.cpp.o: CMakeFiles/barcodes.dir/flags.make
CMakeFiles/barcodes.dir/src/main.cpp.o: src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment2/submit/project02/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/barcodes.dir/src/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/barcodes.dir/src/main.cpp.o -c /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment2/submit/project02/src/main.cpp

CMakeFiles/barcodes.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/barcodes.dir/src/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment2/submit/project02/src/main.cpp > CMakeFiles/barcodes.dir/src/main.cpp.i

CMakeFiles/barcodes.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/barcodes.dir/src/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment2/submit/project02/src/main.cpp -o CMakeFiles/barcodes.dir/src/main.cpp.s

CMakeFiles/barcodes.dir/src/ImgModel.cpp.o: CMakeFiles/barcodes.dir/flags.make
CMakeFiles/barcodes.dir/src/ImgModel.cpp.o: src/ImgModel.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment2/submit/project02/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/barcodes.dir/src/ImgModel.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/barcodes.dir/src/ImgModel.cpp.o -c /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment2/submit/project02/src/ImgModel.cpp

CMakeFiles/barcodes.dir/src/ImgModel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/barcodes.dir/src/ImgModel.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment2/submit/project02/src/ImgModel.cpp > CMakeFiles/barcodes.dir/src/ImgModel.cpp.i

CMakeFiles/barcodes.dir/src/ImgModel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/barcodes.dir/src/ImgModel.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment2/submit/project02/src/ImgModel.cpp -o CMakeFiles/barcodes.dir/src/ImgModel.cpp.s

# Object files for target barcodes
barcodes_OBJECTS = \
"CMakeFiles/barcodes.dir/src/main.cpp.o" \
"CMakeFiles/barcodes.dir/src/ImgModel.cpp.o"

# External object files for target barcodes
barcodes_EXTERNAL_OBJECTS =

barcodes: CMakeFiles/barcodes.dir/src/main.cpp.o
barcodes: CMakeFiles/barcodes.dir/src/ImgModel.cpp.o
barcodes: CMakeFiles/barcodes.dir/build.make
barcodes: /usr/local/lib/libopencv_gapi.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_stitching.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_aruco.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_bgsegm.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_bioinspired.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_ccalib.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_dnn_objdetect.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_dnn_superres.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_dpm.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_face.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_freetype.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_fuzzy.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_hfs.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_img_hash.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_line_descriptor.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_quality.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_reg.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_rgbd.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_saliency.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_sfm.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_stereo.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_structured_light.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_superres.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_surface_matching.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_tracking.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_videostab.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_xfeatures2d.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_xobjdetect.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_xphoto.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_highgui.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_shape.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_datasets.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_plot.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_text.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_dnn.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_ml.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_phase_unwrapping.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_optflow.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_ximgproc.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_video.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_videoio.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_imgcodecs.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_objdetect.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_calib3d.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_features2d.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_flann.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_photo.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_imgproc.4.2.0.dylib
barcodes: /usr/local/lib/libopencv_core.4.2.0.dylib
barcodes: CMakeFiles/barcodes.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment2/submit/project02/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable barcodes"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/barcodes.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/barcodes.dir/build: barcodes

.PHONY : CMakeFiles/barcodes.dir/build

CMakeFiles/barcodes.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/barcodes.dir/cmake_clean.cmake
.PHONY : CMakeFiles/barcodes.dir/clean

CMakeFiles/barcodes.dir/depend:
	cd /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment2/submit/project02 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment2/submit/project02 /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment2/submit/project02 /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment2/submit/project02 /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment2/submit/project02 /Users/Tommy/Desktop/studyInMassey/159_731ComputerVision/assignment2/submit/project02/CMakeFiles/barcodes.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/barcodes.dir/depend

