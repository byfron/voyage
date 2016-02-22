cmake_minimum_required(VERSION 2.6.2)

project(voyage)

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

add_subdirectory(src)
