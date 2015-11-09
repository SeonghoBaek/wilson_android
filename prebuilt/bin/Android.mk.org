LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := nodebus
LOCAL_MODULE := nodebus
LOCAL_MODULE_CLASS := EXECUTABLES

include $(BUILD_PREBUILT)

