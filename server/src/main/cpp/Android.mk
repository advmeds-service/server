LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := advmedsServer
LOCAL_SRC_FILES := server/Jni.cpp \
                   server/Define.h \
                   server/RegisterFunctions.cpp \
                   server/ServerManager.cpp

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)