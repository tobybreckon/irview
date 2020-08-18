# irview cmake configuration file

# Author : Toby Breckon, toby.breckon@durham.ac.uk

# Copyright (c) 2008 School of Engineering, Cranfield University
# Copyright (c) 2017 School of Engineering and Computing Sciences, Durham University
# Copyright (c) 2019 Dept. Computer Science, Durham University

# License : GPL - http://www.gnu.org/licenses/gpl.html

cmake_minimum_required (VERSION 2.6)

# linux specific stuff

IF ( UNIX )
   set( CMAKE_CXX_FLAGS "-O3 -Wall -march=native ${CMAKE_CXX_FLAGS}" )
   set( CMAKE_PREFIX_PATH "/opt/opencv/lib64/cmake/opencv4/" )
   set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
   MESSAGE( "LINUX/UNIX SPECIFIC CONFIG" )
ENDIF ( UNIX )

# windows (also for 64-bit as WIN32 defined on MS WIN 64-bit also - ?!)

IF ( WIN32 )
   set( CMAKE_PREFIX_PATH "C:/OpenCV/build" )
   set( OpenCV_DIR "C:/OpenCV/build" )
   MESSAGE( "WINDOWS CONFIG - assuming OpenCV in C:/OpenCV/build" )
ENDIF ( WIN32 )

project(irview)

find_package( OpenCV REQUIRED )
if (NOT (OpenCV_VERSION VERSION_GREATER 3.0))
         message(FATAL_ERROR "${PROJECT_NAME} requires OpenCV 3.0 or greater.")
endif ()

# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them

message(STATUS "OpenCV library status:")

message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
# message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

# set (OpenCV_INCLUDE_DIRS "/opt/opencv/include")
# message(STATUS "    include path (reset): ${OpenCV_INCLUDE_DIRS}")

include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIR})

add_executable(irview irview.cpp)
target_link_libraries( irview ${OpenCV_LIBS} )
