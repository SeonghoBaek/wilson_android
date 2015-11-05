cmake_minimum_required(VERSION 2.8)
project(nodebus)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
include_directories(${CMAKE_SOURCE_DIR}/service/include)

set(NODEBUS ${CMAKE_SOURCE_DIR}/service)

set(CLIENT_SOURCE_FILES
        ${NODEBUS}/src/DefaultService.cpp
        ${NODEBUS}/src/INodeBusService.cpp
        ${NODEBUS}/src/LocalBusInit.cpp
        ${NODEBUS}/src/Lock.cpp
        ${NODEBUS}/src/Log.cpp
        ${NODEBUS}/src/NodeLooper.cpp
        ${NODEBUS}/src/NodeNetwork.cpp
        ${NODEBUS}/src/NodePort.cpp
        ${NODEBUS}/src/Packet.cpp
        ${NODEBUS}/src/Queue.cpp
        ${NODEBUS}/src/XMLParser.cpp
        ${NODEBUS}/src/Bridge.cpp
        ${NODEBUS}/src/DefaultNodeCallback.cpp
)

set(HEADER_FILES
        ${NODEBUS}/include/Bridge.h
        ${NODEBUS}/include/Configure.h
        ${NODEBUS}/include/DefaultService.h
        ${NODEBUS}/include/IMessageService.h
        ${NODEBUS}/include/INameService.h
        ${NODEBUS}/include/INodeBusService.h
        ${NODEBUS}/include/List.h
        ${NODEBUS}/include/Lock.h
        ${NODEBUS}/include/Log.h
        ${NODEBUS}/include/NodeBus.h
        ${NODEBUS}/include/NodeBusService.h
        ${NODEBUS}/include/NodeEntry.h
        ${NODEBUS}/include/NodeLooper.h
        ${NODEBUS}/include/NodeNetwork.h
        ${NODEBUS}/include/NodePort.h
        ${NODEBUS}/include/Packet.h
        ${NODEBUS}/include/Thread.h
        ${NODEBUS}/include/Types.h
        ${NODEBUS}/include/XMLParser.h
        ${NODEBUS}/include/XMLTypes.h
        ${NODEBUS}/include/DefaultNodeCallback.h)

add_executable(nodebus_client ${CLIENT_SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(nodebus_client xml2)