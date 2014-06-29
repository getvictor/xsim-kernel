cmake_minimum_required (VERSION 2.8)
project (xsim_core)
# The version number
set (xsim_core_VERSION_MAJOR 1)
set (xsim_core_VERSION_MINOR 0)

set (CMAKE_CXX_FLAGS "-std=c++11")

set(XSIM_INCLUDE src/main)
file(GLOB XSIM_SRC ${XSIM_INCLUDE}/*.cpp)
add_library(XsimCore SHARED ${XSIM_SRC})

# Google Test
set(GTEST_REVISION "690")
find_package(Threads REQUIRED)
include(ExternalProject)

set(GTEST_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/gtest")
ExternalProject_Add(GTestExternal
    SVN_REPOSITORY http://googletest.googlecode.com/svn/trunk
    SVN_REVISION -r "${GTEST_REVISION}"
    TIMEOUT 10
    PREFIX "${GTEST_PREFIX}"
    INSTALL_COMMAND "")

set(LIBPREFIX "${CMAKE_STATIC_LIBRARY_PREFIX}")
set(LIBSUFFIX "${CMAKE_STATIC_LIBRARY_SUFFIX}")
set(GTEST_LOCATION "${GTEST_PREFIX}/src/GTestExternal-build")
set(GTEST_INCLUDE "${GTEST_PREFIX}/src/GTestExternal/include")
set(GTEST_LIBRARY  "${GTEST_LOCATION}/${LIBPREFIX}gtest${LIBSUFFIX}")
set(GTEST_MAINLIB  "${GTEST_LOCATION}/${LIBPREFIX}gtest_main${LIBSUFFIX}")

add_library(GTest IMPORTED STATIC GLOBAL)
set_target_properties(GTest PROPERTIES
    "IMPORTED_LOCATION" "${GTEST_LIBRARY}"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}")

add_library(GTestMain IMPORTED STATIC GLOBAL)
set_target_properties(GTestMain PROPERTIES
    "IMPORTED_LOCATION" "${GTEST_MAINLIB}"
    "IMPORTED_LINK_INTERFACE_LIBRARIES"
        "${GTEST_LIBRARY};${CMAKE_THREAD_LIBS_INIT}")

add_dependencies(GTest GTestExternal)

include_directories("${GTEST_INCLUDE}" ${XSIM_INCLUDE})
file(GLOB TEST_SRC src/test/*.cpp)
add_executable(test ${TEST_SRC})
target_link_libraries(test GTestMain XsimCore)
