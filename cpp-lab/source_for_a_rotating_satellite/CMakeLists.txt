#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(cusp_gdml) 

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
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
link_directories(/usr/local/lib)
include_directories(/usr/local/lib)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(rotating_source_model_g4 rotating_source_model_g4.cc ${sources} ${headers})
target_link_libraries(rotating_source_model_g4 ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
# Enable the DEBUG directive for this target
#
# target_compile_definitions(cusp_gdml PRIVATE DEBUG)

#----------------------------------------------------------------------------
# Install the executable to 'bin/Darwin-g++' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS rotating_source_model_g4 DESTINATION bin/Darwin-g++)
