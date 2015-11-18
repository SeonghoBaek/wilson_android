LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_C_INCLUDES := \
                 $(LOCAL_PATH) \
                 external/wilson_android/include

LOCAL_SRC_FILES := USBLogcat.cpp \
                   USBThread.cpp

LOCAL_SHARED_LIBRARIES := \
            libcutils \
            libutils \
            libbinder \
            libnbus

LOCAL_MODULE := ulogcat

include $(BUILD_EXECUTABLE)
