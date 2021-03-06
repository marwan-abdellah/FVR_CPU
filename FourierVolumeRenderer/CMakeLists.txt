# *********************************************************************
# * Copyrights (c) Marwan Abdellah. All rights reserved.
# * This code is part of my Master's Thesis Project entitled "High
# * Performance Fourier Volume Rendering on Graphics Processing Units
# * (GPUs)" and submitted to the Systems & Biomedical Engineering
# * Department, Faculty of Engineering, Cairo University.
# * Please, don't use or distribute without authors' permission.

# * File        : Volume
# * Author(s)	: Marwan Abdellah <abdellah.marwan@gmail.com>
# * Created	: April 2011
# * Description	:
# * Note(s)	:
# *********************************************************************

# Minimum required CMake version 
cmake_minimum_required(VERSION 2.6)

# Fourier Volume Rendeing
PROJECT(FourierVolumeRenderer)

# Add CMake modules 
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)

SET(FFTW_ROOT ${PROJECT_SOURCE_DIR}/../install) 
FIND_PACKAGE(FFTW REQUIRED)
IF(FFTW_FOUND)
  MESSAGE(STATUS "FFTW found...")
  MESSAGE("-- FFTW include directory : ${FFTW_INCLUDE_DIR}")
  MESSAGE("-- FFTW library directory : ${FFTW_LIBRARIES}")
  INCLUDE_DIRECTORIES(${FFTW_INCLUDE_DIR})
  LINK_LIBRARIES(${FFTW_LIBRARIES})
ELSE(FFTW_FOUND)
  MESSAGE(FATAL_ERROR "FFTW not found")
ENDIF(FFTW_FOUND)

# OpenGL & GLU & GLUT  
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLUT REQUIRED)

IF(OPENGL_FOUND)
  MESSAGE(STATUS "OpenGL found...")
  MESSAGE("-- OpenGL include directory : ${OPENGL_INCLUDE_DIR}")
  MESSAGE("-- OpenGL library directory : ${OPENGL_LIBRARY}")
  INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
  LINK_LIBRARIES(${OPENGL_LIBRARY})
ELSE(OPENGL_FOUND)
  MESSAGE(FATAL_ERROR "OpenGL not found")
ENDIF(OPENGL_FOUND)

IF(GLUT_FOUND)
  MESSAGE(STATUS  "GLUT found...")
  MESSAGE("GLUT include directory : ${GLUT_INCLUDE_DIR}")
  MESSAGE("GLUT library directory : ${GLUT_LIBRARY}")
  INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR}/GL)
  LINK_LIBRARIES(${GLUT_LIBRARY})
ELSE(GLUT_FOUND)
  MESSAGE(FATAL_ERROR "GLUT not found")
ENDIF(GLUT_FOUND)

# Add the heade files to the include directories
INCLUDE_DIRECTORIES("${OPENGL_INCLUDE_DIR}")

# Include FindCUDA.cmake 
INCLUDE(FindCUDA_Local)

INCLUDE(FindGLEW)


message(--> ${GLEW_LIB})
LINK_LIBRARIES(${GLEW_LIB})
INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
SET(SOURCES src/FourierVolumeRenderer.cpp)

# Loader
LIST(APPEND SOURCES src/Loader/Loader.cpp)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/Loader)

# OpenGL
LIST(APPEND SOURCES src/OpenGL/DisplayList.cpp)
LIST(APPEND SOURCES src/OpenGL/cOpenGL.cpp)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/OpenGL)

# Volume Processing
LIST(APPEND SOURCES src/VolumeProcessing/volume.cpp)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/VolumeProcessing/)

# FFT Shift
LIST(APPEND SOURCES src/FFTShift/FFTShift.cpp)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/FFTShift/)

# Spectrum
LIST(APPEND SOURCES src/SpectrumProcessing/Spectrum.cpp)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/SpectrumProcessing/)

# Slice Processing
LIST(APPEND SOURCES src/SliceProcessing/Slice.cpp)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/SliceProcessing/)

# Wrapping around
LIST(APPEND SOURCES src/WrappingAround/WrappingAround.cpp)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/WrappingAround/)

# Rendering Loop
LIST(APPEND SOURCES src/RenderingLoop/RenderingLoop.cpp)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/RenderingLoop/)


# Include directories 
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# CUDA kernels directory 
SET(CUDA_KERNELS_DIR ${CMAKE_SOURCE_DIR}/CUDA/FFT)
INCLUDE_DIRECTORIES(${CUDA_KERNELS_DIR})

# --------------------------------------------------------
# NOTE: You have to append all the ".cu" files in this file 
#       to have them compiled
# -------------------------------------------------------- 
# SET(CUDA_SOURCES)
    
# Generate the executable considering CUDA stuff :)
CUDA_ADD_EXECUTABLE(FVR ${CUDA_SOURCES} ${SOURCES})
