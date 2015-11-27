LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_C_INCLUDES := \
                 $(LOCAL_PATH) \
                 external/icu4c/common \
                 external/libxml2/include \
                 external/wilson_android/include

LOCAL_SRC_FILES := MicomThread.cpp \
                   MicomDaemon.cpp \
                   MicomBuffer.cpp

LOCAL_SHARED_LIBRARIES := \
            libcutils \
            libutils \
            libbinder \
            libnbus

LOCAL_MODULE := micomd

include $(BUILD_EXECUTABLE)
