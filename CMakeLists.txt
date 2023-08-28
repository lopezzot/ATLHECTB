##**************************************************
## \file CMakeLists.txt
## \brief: CMakeLists of G4ATLHECTB project
## \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
## \start date: 11 May 2021
##**************************************************

#Setup project
#
cmake_minimum_required(VERSION 3.8...3.18)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()
project(G4ATLHECTB)

#Find Geant4 package, activate all available UI and Vis drivers by default
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
else()
    find_package(Geant4 REQUIRED)
endif()

#Option to enable leakage particle spectrum analysis
#
option(WITH_LEAKAGEANALYSIS "enable SpectrumAnalysis on leakage" OFF)
if(WITH_LEAKAGEANALYSIS)
    add_compile_definitions(ATLHECTB_LEAKANALYSIS)
    message(STATUS "WITH_LEAKAGEANALYSIS = ON : building with ATLHECTB_LEAKANALYSIS compiler definition.")
else()
    message(STATUS "WITH_LEAKAGEANALYSIS = OFF : building without ATLHECTB_LEAKANALYSIS compiler definition.")
endif()

#Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#Find FLUKAInterface, this part is taken from /examples/extenedd/hadronic/FLUKACern/FlukaInterface/README.md
#as in geant4-11.1.ref05 (May 2023)
# FindFLUKAInterface.cmake is located at your_path_to_geant4/cmake/Modules/FindFLUKAInterface.cmake
# Check that FindFLUKAInterface.cmake can be found from $CMAKE_MODULE_PATH
message(STATUS "CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")
# Otherwise, you can always prepend it to the cmake module search path with:
# set(CMAKE_MODULE_PATH my_path_to_find_fluka ${CMAKE_MODULE_PATH})

# Check whether FLUKA should be used or not
set(G4_USE_FLUKA OFF CACHE BOOL "Using FLUKA")
if(G4_USE_FLUKA)
  message(STATUS "G4_USE_FLUKA=ON : Using FLUKA interface for building ${PROJECT_SOURCE_DIR}")
  add_definitions(-DG4_USE_FLUKA)
  find_package(FLUKAInterface REQUIRED)
  if(FLUKAInterface_FOUND)
    message(STATUS "FLUKA cmake module was found : ${CMAKE_MODULE_PATH}")
  else()
    message(FATAL_ERROR "FLUKA cmake module was NOT found! Please add one.")
  endif()
else()
  message(STATUS "G4_USE_FLUKA=OFF : NOT using FLUKA interface for building ${PROJECT_SOURCE_DIR}. \n \
  If ever you want to use the FLUKA interface, please repeat cmake command with -DG4_USE_FLUKA=1")
endif()
#End of find FLUKAInterface

#Setup project include directories
#
include_directories(${PROJECT_SOURCE_DIR}/include ${FLUKAInterface_INCLUDE_DIR})

#Locate project source and headers
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#Add executable and link it to Geant4 libraries
#
add_executable(ATLHECTB ATLHECTB.cc ${sources} ${headers})
target_link_libraries(ATLHECTB ${Geant4_LIBRARIES} ${FLUKAInterface_LIBRARIES})

#Copy all scripts to the build directory
#
set(ATLHECTB_SCRIPTS
    ATLHECTB_init_vis.mac
    ATLHECTB_gui.mac
    ATLHECTB_run.mac
    )

foreach(_script ${ATLHECTB_SCRIPTS})
    configure_file(
        ${PROJECT_SOURCE_DIR}/${_script}
        ${PROJECT_BINARY_DIR}/${_script}
        COPYONLY
       )
endforeach()

#For internal Geant4 use 
#
add_custom_target(G4ATLHECTB DEPENDS ATLHECTB)

#Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS ATLHECTB DESTINATION bin)

##**************************************************
