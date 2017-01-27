cmake_minimum_required(VERSION 2.6.2)

project(voyage)

option(BUILD_VOYAGE3D "build 3d interface with OGRE library" OFF)
option(BUILD_VOYAGE_SDL "build 2d interface with SDL library" ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fPIC -pthread -march=native")

# General dependencies
set(RAKNET_ROOT "${CMAKE_SOURCE_DIR}/RakNet")
find_library(RAKNET_LIB RakNetLibStatic PATHS ${RAKNET_ROOT}/Lib/LibStatic)
include_directories(${RAKNET_ROOT}/include/raknet)
find_package(Threads REQUIRED)
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

set(DB_NAME "test.db")
set(DB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources/datasets/")
set(SCRIPTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources/scripts/")
set(GRAPHICS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources/graphics/")

add_definitions(-DCONFIG_FILE_PATH="${CMAKE_BINARY_DIR}/pumpkin/data/config/")
add_definitions(-DTEXTURE_FILE_PATH="${CMAKE_BINARY_DIR}/pumpkin/data/textures/")
add_definitions(-DSHADER_FILE_PATH="${CMAKE_BINARY_DIR}/pumpkin/data/shaders/")

add_definitions(-DDB_DIR="${DB_DIR}")
add_definitions(-DDB_NAME="${DB_NAME}")
add_definitions(-DSCRIPTS_DIR="${SCRIPTS_DIR}")
add_definitions(-DGRAPHICS_DIR="${GRAPHICS_DIR}")

set(CMAKE_MODULE_PATH
  "${CMAKE_SOURCE_DIR}/CMake"
  "${CMAKE_SOURCE_DIR}/CMake/Utils"
  "${CMAKE_SOURCE_DIR}/CMake/Packages"
  )

SET(PYTHON_INCLUDE_PATH /usr/include/python2.7)
SET(PYTHON_LIBRARIES /usr/lib/python2.7/config-x86_64-linux-gnu/libpython2.7.so)


######## GTest #########
if (BUILD_TESTS)
include(CMake/DownloadProject.cmake)
download_project(PROJ                googletest
                 GIT_REPOSITORY      https://github.com/google/googletest.git
                 GIT_TAG             master
		 )


# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

function(cxx_test name sources)
    add_executable(${name} ${sources})
    target_link_libraries(${name} ${ARGN} gtest gmock_main ${CMAKE_THREAD_LIBS_INIT})
    set_property(TARGET ${name} APPEND PROPERTY INCLUDE_DIRECTORIES
        ${GTEST_INCLUDE_DIRS}
        )

    # add gtest as a dependency if we are building it ourselves
    if(TARGET gtest)
        add_dependencies(${name} gtest)
    endif()
    # Working directory: where the dlls are installed.
    add_test(NAME ${name}
        COMMAND ${name})
    # For debugging, one can manually run the tests with the option
    # --gtest_break_on_failure which will raise a segfault on the first error
endfunction()
endif()

#######################
include_directories(src)
include_directories(pumpkin/src)
include_directories(pumpkin/bx/include)
include_directories(pumpkin/bgfx/include)
include_directories(pumpkin/bgfx/src)
include_directories(pumpkin/bgfx/3rdparty)
include_directories(pumpkin/bgfx/examples/)

#include the created files in the build sub-directory as a include
set(PROTOBUF_GENEREATED_HEADERS ${CMAKE_BINARY_DIR}/protobuf)
include_directories(${PROTOBUF_GENEREATED_HEADERS})
message(STATUS ${PROTOBUF_GENEREATED_HEADERS})


FIND_PACKAGE(Boost 1.45.0)
IF(Boost_FOUND)
  INCLUDE_DIRECTORIES("${Boost_INCLUDE_DIRS}" "/usr/include/python2.6")
  SET(Boost_USE_STATIC_LIBS OFF)
  SET(Boost_USE_MULTITHREADED ON)
  SET(Boost_USE_STATIC_RUNTIME OFF)
  FIND_PACKAGE(Boost 1.45.0 COMPONENTS python)
ENDIF()

find_package(SDL2)
find_package(SDL2_image)
find_package(OpenGL REQUIRED)
FIND_PACKAGE(Protobuf REQUIRED)

find_library(SQLITE3_LIBRARY sqlite3)
add_subdirectory(protobuf)
add_subdirectory(src)
add_subdirectory(pumpkin)
#add_subdirectory(test)
