cmake_minimum_required(VERSION 3.3)
project(usblogcat)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include_directories(${CMAKE_SOURCE_DIR}/node/ulogcat)
include_directories(${CMAKE_SOURCE_DIR}/libs/nbus)

set(NODE ${CMAKE_SOURCE_DIR}/node/ulogcat)

set(BUGREPORT_FILES
	${NODE}/USBLogcat.cpp
	${NODE}/USBThread.cpp)

set(BUGREPORT_HEADER_FILES
	${NODE}/USBLogcat.h
	${NODE}/USBThread.h)

target_link_libraries(nbus)

add_executable(ulogcat ${BUGREPORT_FILES} ${BUGREPORT_HEADER_FILES})
