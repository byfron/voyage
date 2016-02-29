cmake_minimum_required(VERSION 2.6.2)

project(voyage)

option(BUILD_VOYAGE3D "build 3d interface with OGRE library" OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC -march=native")

# General dependencies
set(RAKNET_ROOT "${CMAKE_SOURCE_DIR}/third-party/RakNet")
find_library(RAKNET_LIB RakNetLibStatic PATHS ${RAKNET_ROOT}/Lib/RakNetLibStatic)
include_directories(${RAKNET_ROOT}/include/raknet)
find_package(Threads REQUIRED)

set(DB_NAME "test.db")
set(DB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources/datasets/")
set(SCRIPTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources/scripts/")

add_definitions(-DDB_DIR="${DB_DIR}")
add_definitions(-DDB_NAME="${DB_NAME}")
add_definitions(-DSCRIPTS_DIR="${SCRIPTS_DIR}")

set(CMAKE_MODULE_PATH
  "${CMAKE_SOURCE_DIR}/CMake"
  "${CMAKE_SOURCE_DIR}/CMake/Utils"
  "${CMAKE_SOURCE_DIR}/CMake/Packages"
  )

SET(PYTHON_INCLUDE_PATH /usr/include/python2.7)
SET(PYTHON_LIBRARIES /usr/lib/python2.7/config-x86_64-linux-gnu/libpython2.7.so)
include(CMake/GTest.cmake)

FIND_PACKAGE(Boost 1.45.0)
IF(Boost_FOUND)
  INCLUDE_DIRECTORIES("${Boost_INCLUDE_DIRS}" "/usr/include/python2.6")
  SET(Boost_USE_STATIC_LIBS OFF)
  SET(Boost_USE_MULTITHREADED ON)
  SET(Boost_USE_STATIC_RUNTIME OFF)
  FIND_PACKAGE(Boost 1.45.0 COMPONENTS python)
ENDIF()

#find_package(SQLite3)
find_library(SQLITE3_LIBRARY sqlite3)

if (BUILD_VOYAGE3D)  

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
