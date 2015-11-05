LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_STATIC_JAVA_LIBRARIES := android-common

LOCAL_SRC_FILES := $(call all-java-files-under, src) $(call all-renderscript-files-under, src)

LOCAL_SRC_FILES += src/com/node/micom/service/IMicomListener.aidl \
                   src/com/node/micom/service/IMicomService.aidl \
                   src/com/node/wilson/IWilsonRemoteListener.aidl \
                   src/com/node/wilson/IWilsonService.aidl

LOCAL_RESOURCE_DIR += $(LOCAL_PATH)/res

LOCAL_PACKAGE_NAME := Wilson

include $(BUILD_PACKAGE)
