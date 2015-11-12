cmake_minimum_required(VERSION 3.3)
project(rlogcat)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include_directories(${CMAKE_SOURCE_DIR}/node/rlogcat)
include_directories(${CMAKE_SOURCE_DIR}/libs/nbus)

set(NODE ${CMAKE_SOURCE_DIR}/node)

set(RLOGCAT_FILES
        ${NODE}/rlogcat/RedirectLogCat.cpp
        ${NODE}/rlogcat/LogCatThread.cpp)

set(RLOGCAT_HEADER_FILES
        ${NODE}/rlogcat/LogCatThread.h
        ${NODE}/rlogcat/RedirectLogCat.h)

target_link_libraries(nbus xml2)

add_executable(rlogcat ${RLOGCAT_FILES} ${RLOGCAT_HEADER_FILES})
