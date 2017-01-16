# gtest.

#Try first to find gtest in custom path
FIND_PATH(GTEST_INCLUDE_DIRS NAMES gtest/gtest.h PATHS ${GTEST_INCLUDE_PATH})
FIND_LIBRARY(GTEST_LIBRARY NAMES gtest gtest_main PATHS ${GTEST_LIB_PATH})

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
endif ()

if (GTEST_INCLUDE_DIRS AND GTEST_LIBRARY)
    message(STATUS "Found gtest at ${GTEST_LIBRARY}")
    add_custom_target(gtest)
else ()
    include(ExternalProject)

    set(GTEST_URL "https://github.com/google/googletest.git")
    set(GTEST_TAG "master")

    message(STATUS "Will fetch gtest ${GTEST_TAG} from ${GTEST_URL}")

    ExternalProject_Add(gtest
        GIT_REPOSITORY "${GTEST_URL}"
        GIT_TAG "${GTEST_TAG}"
        BINARY_DIR "${CMAKE_BINARY_DIR}/third-party/gtest-build"
        SOURCE_DIR "${CMAKE_BINARY_DIR}/third-party/gtest-src"
        INSTALL_COMMAND ""
        UPDATE_COMMAND ""
        CMAKE_ARGS "${gtest_cmake_args}"
        "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
        )
    set(GTEST_INCLUDE_DIRS
        "${CMAKE_BINARY_DIR}/third-party/gtest-src/googletest/include"
        )
    set(GTEST_LIBRARY "${CMAKE_BINARY_DIR}/third-party/gtest-build/googlemock/gtest/libgtest.a")
    set(GTEST_LIB_PATH "${CMAKE_BINARY_DIR}/third-party/gtest-build/googlemock/gtest")
endif ()

link_directories(${GTEST_LIB_PATH})
enable_testing()
find_package(Threads)
function(cxx_test name sources)
    add_executable(${name} ${sources})
    target_link_libraries(${name} ${ARGN} ${GTEST_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
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
