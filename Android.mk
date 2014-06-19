LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := gpgoap
LOCAL_SRC_FILES := astar.c goap.c

#LOCAL_C_INCLUDES := 

LOCAL_CFLAGS += -std=c99

include $(BUILD_STATIC_LIBRARY)

