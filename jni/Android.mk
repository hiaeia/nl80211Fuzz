MY_LOCAL_PATH := $(call my-dir)
MY_LOCAL_CFLAGS := -DDEBUG

include $(call all-subdir-makefiles)

LOCAL_PATH := $(MY_LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := lnetlink
LOCAL_SRC_FILES := lnetlink.c
LOCAL_C_INCLUDES    := .
LOCAL_CFLAGS    := -fPIC -fPIE $(MY_LOCAL_CFLAGS)
LOCAL_SHARED_LIBRARIES  := libc general libc
include $(BUILD_SHARED_LIBRARY)

LOCAL_PATH := $(MY_LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := nl80211
LOCAL_SRC_FILES := nl80211.c build_nl80211_args.c
LOCAL_C_INCLUDES    := .
LOCAL_CFLAGS    := -fPIC -fPIE $(MY_LOCAL_CFLAGS)
LOCAL_SHARED_LIBRARIES  := lnetlink general libc
include $(BUILD_EXECUTABLE)
