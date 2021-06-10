cmake_minimum_required(VERSION 3.19)

project(stubber VERSION 1.0.0 LANGUAGES CXX)

set(
        SOURCE_FILES
        src/stubber.cpp
)

set(
        HEADER_FILES
        include/stubber/any.h
        include/stubber/any_abstract.h
        include/stubber/stubber.h
)

# Prevent in-source builds. This clutters the source directory.
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR “In-source builds not allowed! Create a build directory and run CMake from there. ”)
endif ()

add_library(${PROJECT_NAME} STATIC EXCLUDE_FROM_ALL ${HEADER_FILES} ${SOURCE_FILES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        )

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported)
if (ipo_supported)
    set(INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()
