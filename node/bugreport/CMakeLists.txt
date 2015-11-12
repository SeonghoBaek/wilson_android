cmake_minimum_required(VERSION 3.3)
project(bugreport)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include_directories(${CMAKE_SOURCE_DIR}/node/bugreport)
include_directories(${CMAKE_SOURCE_DIR}/libs/nbus)

set(NODE ${CMAKE_SOURCE_DIR}/node/bugreport)

set(BUGREPORT_FILES
        ${NODE}/BugReport.cpp
        ${NODE}/ReportThread.cpp)

set(BUGREPORT_HEADER_FILES
        ${NODE}/BugReport.h
        ${NODE}/ReportThread.h)

target_link_libraries(nbus)

add_executable(rbugreport ${BUGREPORT_FILES} ${BUGREPORT_HEADER_FILES})
