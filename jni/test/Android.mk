#LOCAL_PATH := $(call my-dir)
#include $(CLEAR_VARS)
#LOCAL_MODULE    := testGeneral
#LOCAL_SRC_FILES := testGeneral.c
#LOCAL_C_INCLUDES    := .
#LOCAL_CFLAGS    := -fPIC -fPIE $(MY_LOCAL_CFLAGS)
#LOCAL_SHARED_LIBRARIES  := libc general
#include $(BUILD_EXECUTABLE)


#LOCAL_PATH := $(call my-dir)
#include $(CLEAR_VARS)
#LOCAL_MODULE    := testNl80211
#LOCAL_SRC_FILES := testNl80211.c
#LOCAL_C_INCLUDES    := .
#LOCAL_CFLAGS    := -fPIC -fPIE $(MY_LOCAL_CFLAGS)
#LOCAL_SHARED_LIBRARIES  := libc general lnetlink
#include $(BUILD_EXECUTABLE)

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := testGetFamliy
LOCAL_SRC_FILES := testGetFamliy.c
LOCAL_C_INCLUDES    := .
LOCAL_CFLAGS    := -fPIC -fPIE $(MY_LOCAL_CFLAGS)
LOCAL_SHARED_LIBRARIES  := libc general lnetlink
include $(BUILD_EXECUTABLE)
