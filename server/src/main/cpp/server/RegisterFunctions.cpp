//
// Created by shrek on 2021/12/29.
//

#include <jni.h>
#include "Define.h"

void startServer(JNIEnv *env, jobject server, jint port) {
    LOGI("start server port %d", port);
}

void stopServer(JNIEnv *env, jobject server) {
    LOGI("stop server");
}

static const char *nativeServerClassName = "com/advmeds/server/Server";
static JNINativeMethod methods[] = {
        {"nativeStartServer", "(I)V", (void *) startServer},
        {"nativeStopServer", "()V", (void *) stopServer}
};

jint registerNativeServerFunction(JNIEnv *env) {
    LOGI("start registerNativeServerFunction");
    if (env == nullptr) {
        LOGI("env is null");
        return JNI_FALSE;
    }
    LOGI("env address %p", env);
    jclass clazz = env->FindClass(nativeServerClassName);
    if (clazz == nullptr) {
        LOGI("not found class %s",nativeServerClassName);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) < 0) {
        LOGI("register error");
        return JNI_FALSE;
    }

    LOGI("register server function success");
    return JNI_TRUE;
}



