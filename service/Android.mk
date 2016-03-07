LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_CFLAGS += -Wno-reorder

LOCAL_C_INCLUDES := \
                 $(LOCAL_PATH) \
                 external/wilson_android/include \
                 external/icu4c/common \
                 external/libxml2/include 
                 
LOCAL_SRC_FILES := \
                    Bridge.cpp \
                    DefaultService.cpp \
                    LocalBusInit.cpp \
                    INodeBusService.cpp \
                    Lock.cpp \
                    Log.cpp \
                    NodeLooper.cpp \
                    NodeNetwork.cpp \
                    NodeAdapter.cpp \
                    NodePort.cpp \
                    Packet.cpp \
                    Queue.cpp \
                    Stack.cpp \
                    XMLParser.cpp \
                    DefaultNodeCallback.cpp \
                    NBus.cpp

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
                 external/wilson_android/include \
                 external/icu4c/common \
                 external/libxml2/include

LOCAL_SRC_FILES := \
                    Bridge.cpp \
                    INodeBusService.cpp \
                    Lock.cpp \
                    Log.cpp \
                    NodeLooper.cpp \
                    NodeNetwork.cpp \
                    NodePort.cpp \
                    NodeAdapter.cpp \
                    Packet.cpp \
                    Queue.cpp \
                    Stack.cpp \
                    XMLParser.cpp \
                    NBus.cpp

LOCAL_STATIC_LIBRARIES := libxml2

LOCAL_SHARED_LIBRARIES := \
            liblog \
            libcutils \
			 libicuuc \
            libutils
            
LOCAL_MODULE := libnbus

include $(BUILD_SHARED_LIBRARY)
