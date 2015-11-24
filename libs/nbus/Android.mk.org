LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_C_INCLUDES := \
                 $(LOCAL_PATH) \
                 external/icu4c/common \
                 external/libxml2/include
                 
LOCAL_SRC_FILES := \
                    Lock.cpp \
                    Log.cpp \
                    NodeLooper.cpp \
                    Bridge.cpp \
                    NBus.cpp \
                    Packet.cpp \
                    XMLParser.cpp \
                    Queue.cpp

LOCAL_SHARED_LIBRARIES := \
             liblog \
             libcutils \
			 libicuuc \
             libutils

LOCAL_STATIC_LIBRARIES := libxml2

LOCAL_MODULE := libnbus

include $(BUILD_SHARED_LIBRARY)
