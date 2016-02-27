cmake_minimum_required(VERSION 2.6.2)

project(voyage)

option(BUILD_VOYAGE3D "build 3d interface with OGRE library" OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC -march=native")

# General dependencies
set(RAKNET_ROOT "${CMAKE_SOURCE_DIR}/third-party/RakNet")
find_library(RAKNET_LIB RakNetLibStatic PATHS ${RAKNET_ROOT}/Lib/RakNetLibStatic)
include_directories(${RAKNET_ROOT}/include/raknet)
find_package(Threads REQUIRED)

if (BUILD_VOYAGE3D)  
  set(CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/CMake"
    "${CMAKE_SOURCE_DIR}/CMake/Utils"
    "${CMAKE_SOURCE_DIR}/CMake/Packages"
    )

  include(OgreAddTargets)
  include(CMakeDependentOption)
  include(CheckCXXCompilerFlag)
  include(MacroLogFeature)
  include(OgreAddTargets)
  include(OgreConfigTargets)
  include(PreprocessorUtils)

  find_package(OGRE)
  find_package(SDL2)

  include_directories(${OGRE_INCLUDE_DIR})
  include_directories(${SDL2_INCLUDE_DIR})

  ogre_add_component_include_dir(Hlms/Pbs)
  ogre_add_component_include_dir(Hlms/Unlit)
  ogre_add_component_include_dir(Hlms/Common)

  include_directories(${OGRE_INCLUDE_DIR}/Overlay)
  include_directories(${OGRE_INCLUDE_DIR}/Hlms/Unlit)
  include_directories(${OGRE_INCLUDE_DIR}/Hlms/Pbs)
  
  
  set(OGRE_LIBRARIES ${OGRE_LIBRARIES} OgreOverlay OgreHlmsPbs OgreHlmsUnlit)
endif()

add_subdirectory(src)
