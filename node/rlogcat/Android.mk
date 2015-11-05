LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_C_INCLUDES := \
                 $(LOCAL_PATH) \
                 external/icu4c/common \
                 external/libxml2/include \
                 vendor/wilson/include

LOCAL_SRC_FILES := LogCatThread.cpp \
                   RedirectLogCat.cpp

LOCAL_SHARED_LIBRARIES := \
            libcutils \
            libutils \
            libbinder \
            libnbus

LOCAL_MODULE := rlogcat

include $(BUILD_EXECUTABLE)
