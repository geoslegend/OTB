#####################################################
#
#   This is the root SoftwareGuide CMakeLists file.
#
#####################################################
#
# Documentation on how to build the Software Guide
# is available on OTB Wiki at:
# http://wiki.orfeo-toolbox.org/index.php/Compiling_documentation

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# TODO Check if OTB cmake is compatible with CMP0050 NEW policy
# CMP0054 : New policy introduce in CMake 3.0, keep old behaviour for now
if(POLICY CMP0050)
  cmake_policy(SET CMP0050 OLD)
endif()

PROJECT(SoftwareGuide C)



#
# File that defines the path to .cxx examples in OTB sources
#
CONFIGURE_FILE(
${SoftwareGuide_SOURCE_DIR}/SoftwareGuideConfiguration.tex.in
${SoftwareGuide_BINARY_DIR}/SoftwareGuideConfiguration.tex
IMMEDIATE
)

# Find OTB Sources
#FIND_PATH(OTB_SOURCE_DIR UseOTB.cmake.in
#   /ORFEO/thomas/ORFEO-TOOLBOX/otb/OTB
#  )

FIND_PACKAGE(OTB)
IF(OTB_FOUND)
  INCLUDE(${OTB_USE_FILE})
ELSE(OTB_FOUND)
  MESSAGE(FATAL_ERROR
          "Cannot build OTB-Documents without OTB.  Please set OTB_DIR.")
ENDIF(OTB_FOUND)
IF(DEFINED OTB_INSTALL_PREFIX)
  SET(OTB_PREFIX ${OTB_INSTALL_PREFIX})
ELSE()
  SET(OTB_PREFIX ${OTB_DIR})
ENDIF()


IF( NOT OTB_SOURCE_DIR )
  MESSAGE(FATAL_ERROR "OTB source directory is not set. Please add OTB_SOURCE_DIR to your cmake cache.")
ENDIF( NOT OTB_SOURCE_DIR )

set(OTB_VERSION_STRING_FULL "${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}.${OTB_VERSION_PATCH}")

#OPTION(GENERATE_MONTEVERDI_DOC "Generate Monteverdi user guide as well" OFF)

# Configure the default OTB_DATA_ROOT for the location of OTB Data.
FIND_PATH(OTB_DATA_ROOT README-OTB-Data PATHS $ENV{OTB_DATA_ROOT} ${OTB_SOURCE_DIR}/../OTB-Data)

FIND_PATH(OTB_DATA_LARGEINPUT_ROOT OTBData.readme PATHS $ENV{OTB_DATA_LARGEINPUT_ROOT})

IF( NOT OTB_DATA_LARGEINPUT_ROOT )
  MESSAGE(FATAL_ERROR "Large input directory is not set")
ENDIF( NOT OTB_DATA_LARGEINPUT_ROOT )

# OTB_DATA_PATHS is searched recursively.. you need not enter sub-directories
SET(OTB_DATA_PATHS "${OTB_DATA_ROOT}/Examples::${OTB_DATA_ROOT}/Input::${OTB_DATA_LARGEINPUT_ROOT}" CACHE STRING "Where the OTB data is. Enter a double colon separated list.")

#
# Rebuild the Software Guide figures or not ?
SET(BUILD_FIGURES ON CACHE BOOL "Build Software Guide figures")

IF( BUILD_FIGURES )
  SET(OTB_EXECUTABLES_DIR "${OTB_LIBRARY_DIRS}" CACHE PATH "Where the OTB examples executables are")
  SET(RUN_EXAMPLES_SCRIPT "${SoftwareGuide_SOURCE_DIR}/Examples/RunExamples.pl" CACHE FILEPATH "Where the RunExamples Script is")
ENDIF( BUILD_FIGURES )


#
# Find ImageMagick tools
# This is used to convert image formats
#
find_program(IMAGEMAGICK_CONVERT_EXECUTABLE convert)
IF(NOT IMAGEMAGICK_CONVERT_EXECUTABLE)
  MESSAGE(FATAL_ERROR "ImageMagick convert utility was not found. Please pass to advanced mode and provide its full path")
ENDIF(NOT IMAGEMAGICK_CONVERT_EXECUTABLE)


#
# Find bmeps tools
# This is used to convert image formats from png to eps
#

OPTION(OTB_USE_BMEPS "Use bmeps library to convert figures and images." OFF)
MARK_AS_ADVANCED(OTB_USE_BMEPS)
IF(OTB_USE_BMEPS)
  FIND_PROGRAM(BMEPS_EXECUTABLE bmeps)
  MARK_AS_ADVANCED(BMEPS_EXECUTABLE)
  IF(NOT BMEPS_EXECUTABLE)
    MESSAGE(FATAL_ERROR "bmeps utility was not found. Please pass to advanced mode and provide its full path. It is part of DKTools available on http://dktools.sourceforge.net/")
  ENDIF(NOT BMEPS_EXECUTABLE)
ENDIF(OTB_USE_BMEPS)

IF (BUILD_FIGURES)

  #
  #
  # ADD THE LIST OF INPUT IMAGES YOU WANT FLIPPED HERE.
  #
  # NOTE:
  # If ON, all input images in the list and all outputs that use these images
  # as inputs in cmd line args will be flipped about 'y'. The command line
  # argument itself does not run using the flipped images; instead the eps
  # images corresponding to the inputs mentioned in the list and their output
  # dependencies are flipped for inclusion in the TEX document.

  OPTION(OTB_FLIP_INPUTS_AND_THEIR_OUTPUTS "Flip the input images specified in CMakeLists and their corresponding outputs" ON)
  SET(OTB_FLIP_IMG
    BinaryImage
  )
  # END FLIP_INPUTS LIST


  ADD_SUBDIRECTORY(Examples)
  ADD_SUBDIRECTORY(Art)
  ADD_SUBDIRECTORY(Latex)
  #ADD_SUBDIRECTORY(MonteverdiGuide)
ELSE( BUILD_FIGURES )
  ADD_SUBDIRECTORY(Art)
  ADD_SUBDIRECTORY(Examples)
  ADD_SUBDIRECTORY(Latex)
  #ADD_SUBDIRECTORY(MonteverdiGuide)
ENDIF( BUILD_FIGURES )

#
# Examples Directory is where .cxx files are converted
# into .tex by a perl script.
#
# Art Directory with images and diagrams
#
# Latex Directory with tex files
#
