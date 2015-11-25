cmake_minimum_required(VERSION 2.8)
project(nodebus)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
include_directories(${CMAKE_SOURCE_DIR}/include)

set(NODEBUS ${CMAKE_SOURCE_DIR}/service)

set(CLIENT_SOURCE_FILES
        ${NODEBUS}/DefaultService.cpp
        ${NODEBUS}/INodeBusService.cpp
        ${NODEBUS}/LocalBusInit.cpp
        ${NODEBUS}/Lock.cpp
        ${NODEBUS}/Log.cpp
        ${NODEBUS}/NodeLooper.cpp
        ${NODEBUS}/NodeNetwork.cpp
        ${NODEBUS}/NodePort.cpp
        ${NODEBUS}/NodeAdapter.cpp
        ${NODEBUS}/Packet.cpp
        ${NODEBUS}/Queue.cpp
        ${NODEBUS}/XMLParser.cpp
        ${NODEBUS}/Bridge.cpp
        ${NODEBUS}/DefaultNodeCallback.cpp
        ${NODEBUS}/NBus.cpp
)

set(HEADER_FILES
        ${CMAKE_SOURCE_DIR}/include/Bridge.h
        ${CMAKE_SOURCE_DIR}/include/Configure.h
        ${CMAKE_SOURCE_DIR}/include/Config.h
        ${CMAKE_SOURCE_DIR}/include/DefaultService.h
        ${CMAKE_SOURCE_DIR}/include/IMessageService.h
        ${CMAKE_SOURCE_DIR}/include/INameService.h
        ${CMAKE_SOURCE_DIR}/include/INodeBusService.h
        ${CMAKE_SOURCE_DIR}/include/List.h
        ${CMAKE_SOURCE_DIR}/include/Lock.h
        ${CMAKE_SOURCE_DIR}/include/Log.h
        ${CMAKE_SOURCE_DIR}/include/NodeBus.h
        ${CMAKE_SOURCE_DIR}/include/NodeBusService.h
        ${CMAKE_SOURCE_DIR}/include/NodeEntry.h
        ${CMAKE_SOURCE_DIR}/include/NodeLooper.h
        ${CMAKE_SOURCE_DIR}/include/NodeNetwork.h
        ${CMAKE_SOURCE_DIR}/include/NodePort.h
        ${CMAKE_SOURCE_DIR}/include/Packet.h
        ${CMAKE_SOURCE_DIR}/include/Thread.h
        ${CMAKE_SOURCE_DIR}/include/Types.h
        ${CMAKE_SOURCE_DIR}/include/XMLParser.h
        ${CMAKE_SOURCE_DIR}/include/XMLTypes.h
        ${CMAKE_SOURCE_DIR}/include/DefaultNodeCallback.h
        ${CMAKE_SOURCE_DIR}/include/Message.h
        ${CMAKE_SOURCE_DIR}/include/NBus.h)

add_executable(nodebus_client ${CLIENT_SOURCE_FILES} ${HEADER_FILES})
add_library(nbus ${CLIENT_SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(nodebus_client xml2)