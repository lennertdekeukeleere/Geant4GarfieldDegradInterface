#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(InterfaceExample)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
#----------------------------------------------------------------------------
# Find ROOT (required package)
#
find_package(ROOT QUIET REQUIRED COMPONENTS RIO Net)
if(NOT ROOT_FOUND)
  message(STATUS "InterfaceExample: ROOT package not found. --> InterfaceExample disabled")  
  return()
endif()
include(${ROOT_USE_FILE})
# Setup Geant4 include directories and compile definitions
include_directories(${PROJECT_SOURCE_DIR}/include)

# Setup GARFIELD++ include directories
include_directories($ENV{GARFIELD_HOME}/Include)
include_directories($ENV{GARFIELD_HOME}/Heed)
# Add additional libraries needed for GARFIELD++
link_directories($ENV{GARFIELD_LIB})
#---------------------------------------------------------------------------
#include the macro for generating dictionaries


#--------------------------------------------------------------------------
#Create a shared library with the generated dictionary




#---------------------------------------------------------------------------
# Find CADMESH (required package)
#
find_package(cadmesh)
if(NOT cadmesh_FOUND)
  message(STATUS "InterfaceExample: CADMesh library not found. --> InterfaceExample disabled")
  return()
endif()
include_directories(${CADMESH_INCLUDE_DIRS})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
#file(GLOB headers $ENV{GARFIELD_HOME}/include/*.hh)
#file(GLOB headers $ENV{GARFIELD_HOME}/heed/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 and GARFIELD++ libraries
#
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -g")
add_executable(InterfaceExample InterfaceExample.cc ${sources} ${headers})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
foreach(_script ${InterfaceExample_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS InterfaceExample DESTINATION bin)
