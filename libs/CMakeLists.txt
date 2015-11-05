cmake_minimum_required(VERSION 2.8)
project(libs)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
include_directories(${CMAKE_SOURCE_DIR}/service/include)

set(NBUS ${CMAKE_SOURCE_DIR}/libs/nbus)

set(SOURCE_FILES
        ${NBUS}/Lock.cpp
        ${NBUS}/Log.cpp
        ${NBUS}/NodeLooper.cpp
        ${NBUS}/Bridge.cpp
        ${NBUS}/NBus.cpp
        ${NBUS}/Packet.cpp
        ${NBUS}/XMLParser.cpp
        ${NBUS}/Queue.cpp
)

set(HEADER_FILES
        ${NBUS}/Log.h
        ${NBUS}/NodeLooper.h
        ${NBUS}/Bridge.h
        ${NBUS}/NBus.h
        ${NBUS}/Config.h
        ${NBUS}/Lock.h
        ${NBUS}/Types.h
        ${NBUS}/List.h
        ${NBUS}/XMLTypes.h
        ${NBUS}/XMLParser.h
        ${NBUS}/Packet.h
        ${NBUS}/Configure.h
)

add_library(nbus ${SOURCE_FILES} ${HEADER_FILES})
