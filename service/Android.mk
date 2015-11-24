LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_C_INCLUDES := \
                 $(LOCAL_PATH) \
                 $(LOCAL_PATH)/include \
                 external/icu4c/common \
                 external/libxml2/include 
                 
LOCAL_SRC_FILES := \
                    src/Bridge.cpp \
                    src/DefaultService.cpp \
                    src/LocalBusInit.cpp \
                    src/INodeBusService.cpp \
                    src/Lock.cpp \
                    src/Log.cpp \
                    src/NodeLooper.cpp \
                    src/NodeNetwork.cpp \
                    src/NodeAdapter.cpp \
                    src/NodePort.cpp \
                    src/Packet.cpp \
                    src/Queue.cpp \
                    src/XMLParser.cpp \
                    src/DefaultNodeCallback.cpp \
                    src/NBus.cpp

LOCAL_STATIC_LIBRARIES := libxml2

LOCAL_SHARED_LIBRARIES := \
            liblog \
            libcutils \
			 libicuuc \
            libutils


LOCAL_MODULE := nodebus

include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_C_INCLUDES := \
                 $(LOCAL_PATH) \
                 $(LOCAL_PATH)/include \
                 external/icu4c/common \
                 external/libxml2/include

LOCAL_SRC_FILES := \
                    src/Bridge.cpp \
                    src/INodeBusService.cpp \
                    src/Lock.cpp \
                    src/Log.cpp \
                    src/NodeLooper.cpp \
                    src/NodeNetwork.cpp \
                    src/NodePort.cpp \
                    src/NodeAdapter.cpp \
                    src/Packet.cpp \
                    src/Queue.cpp \
                    src/XMLParser.cpp \
                    src/NBus.cpp

LOCAL_STATIC_LIBRARIES := libxml2

LOCAL_SHARED_LIBRARIES := \
            liblog \
            libcutils \
			 libicuuc \
            libutils
            
LOCAL_MODULE := libnbus

include $(BUILD_SHARED_LIBRARY)