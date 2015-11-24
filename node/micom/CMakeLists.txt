cmake_minimum_required(VERSION 3.3)
project(micomd)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include_directories(${CMAKE_SOURCE_DIR}/node/rlogcat)
include_directories(${CMAKE_SOURCE_DIR}/libs/nbus)

set(NODE ${CMAKE_SOURCE_DIR}/node)

set(MICOM_FILES
        ${NODE}/micom/MicomDaemon.cpp
        ${NODE}/micom/MicomThread.cpp)

set(MICOM_HEADER_FILES
        ${NODE}/micom/MicomThread.h
        ${NODE}/micom/MicomDaemon.h)

target_link_libraries(nbus xml2)

add_executable(micomd ${MICOM_FILES} ${MICOM_HEADER_FILES})
