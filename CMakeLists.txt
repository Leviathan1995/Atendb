cmake_minimum_required(VERSION 2.6)

project(atendb CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CXX_FLAGS
    -g
    -Wall
    -march=native
    -rdynamic
    -std=c++0x
)

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++11 -g -O0")

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

find_package(GTest REQUIRED)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) 

file(GLOB_RECURSE SOURCE 
     ${CMAKE_CURRENT_SOURCE_DIR} 
     ${CMAKE_CURRENT_SOURCE_DIR}/db/*.cc
     ${CMAKE_CURRENT_SOURCE_DIR}/port/*.cc
     ${CMAKE_CURRENT_SOURCE_DIR}/util/*.cc
     ${CMAKE_CURRENT_SOURCE_DIR}/file/*.cc
)

add_library(atendb ${SOURCE})
target_link_libraries(atendb pthread rt)
set_target_properties(atendb PROPERTIES OUTPUT_NAME atendb)
install(TARGETS atendb DESTINATION lib)

add_subdirectory(test)
