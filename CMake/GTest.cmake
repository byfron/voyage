# gtest.

#Try first to find gtest in custom path
if (USE_GMOCK)
    FIND_PATH(GTEST_INCLUDE_DIRS NAMES gtest/gtest.h PATHS ${GTEST_INCLUDE_PATH})
    FIND_PATH(GMOCK_INCLUDE_DIRS NAMES gmock/gmock.h PATHS ${GTEST_INCLUDE_PATH})
    FIND_LIBRARY(GTEST_LIBRARY NAMES gtest PATHS ${GTEST_LIB_PATH})
    FIND_LIBRARY(GMOCK_LIBRARY NAMES gmock PATHS ${GTEST_LIB_PATH})
else()
    FIND_PATH(GTEST_INCLUDE_DIRS NAMES gtest/gtest.h PATHS ${GTEST_INCLUDE_PATH})
    FIND_LIBRARY(GTEST_LIBRARY NAMES gtest PATHS ${GTEST_LIB_PATH})
endif()

if (MSVC)
function(cxx_test name sources)
    add_executable(${name} ${sources})
    target_link_libraries(${name} ${ARGN} gtest ${CMAKE_THREAD_LIBS_INIT})
    set_property(TARGET ${name} APPEND PROPERTY INCLUDE_DIRECTORIES
                 ${GTEST_INCLUDE_DIRS}
                 )
    # Working directory: where the dlls are installed.
    add_test(NAME ${name}
             COMMAND ${name} "--gtest_break_on_failure")
endfunction()
endif()

if (GTEST_INCLUDE_DIRS AND GTEST_LIBRARY)
   link_directories(${GTEST_LIB_PATH})
   enable_testing()
   find_package(Threads)
else()
    include(ExternalProject)

    option(USE_GMOCK "If ON, not only gtest, but also gmock will be installed." ON)

    if (USE_GMOCK)
      ExternalProject_Add(gmock
            URL "https://googlemock.googlecode.com/files/gmock-1.7.0.zip"
            BINARY_DIR "${CMAKE_BINARY_DIR}/third-party/gmock-build"
	    SOURCE_DIR "${CMAKE_BINARY_DIR}/third-party/gmock-src"
	    INSTALL_DIR "${CMAKE_BINARY_DIR}/third-party/gtest-build"
	    CMAKE_ARGS "${gtest_cmake_args}"
	    "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
	    INSTALL_COMMAND ""
	    )
    set(GTEST_INCLUDE_DIRS
                   "${CMAKE_BINARY_DIR}/third-party/gmock-src/gtest/include"
                   "${CMAKE_BINARY_DIR}/third-party/gmock-src/include"
     )
  link_directories(
      "${CMAKE_BINARY_DIR}/third-party/gmock-build"
      "${CMAKE_BINARY_DIR}/third-party/gmock-build/gtest"
      )
else (USE_GMOCK)
  ExternalProject_Add(gtest
	  SVN_REPOSITORY "http://googletest.googlecode.com/svn/tags/release-1.7.0"
          BINARY_DIR "${CMAKE_BINARY_DIR}/third-party/gtest-build"
          SOURCE_DIR "${CMAKE_BINARY_DIR}/third-party/gtest-src"
          INSTALL_COMMAND ""
          CMAKE_ARGS "${gtest_cmake_args}"
            "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
          )
  set(GTEST_INCLUDE_DIRS
          "${CMAKE_BINARY_DIR}/third-party/gtest-src/include"
     )
endif (USE_GMOCK)

enable_testing()
find_package(Threads)
function(cxx_test name sources)
    add_executable(${name} ${sources})
    target_link_libraries(${name} ${ARGN} gtest ${CMAKE_THREAD_LIBS_INIT})
    set_property(TARGET ${name} APPEND PROPERTY INCLUDE_DIRECTORIES
                 ${GTEST_INCLUDE_DIRS}
                 )
    add_dependencies(${name} gmock)
    # Working directory: where the dlls are installed.
    add_test(NAME ${name}
             COMMAND ${name} "--gtest_break_on_failure")
endfunction()
endif()
