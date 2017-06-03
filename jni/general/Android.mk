LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := general
LOCAL_SRC_FILES := generalLib.c generalLib.h
LOCAL_C_INCLUDES    := .
LOCAL_CFLAGS    := -fPIC -fPIE $(MY_LOCAL_CFLAGS)
LOCAL_SHARED_LIBRARIES  := libc
include $(BUILD_SHARED_LIBRARY)


