//
// Created by shrek on 2021/12/29.
//
#include <jni.h>
#include "Define.h"

jint registerNativeServerFunction(JNIEnv *env);

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("JNI_OnLoad");
    JNIEnv *env = nullptr;
    vm->GetEnv((void**) &env, JNI_VERSION_1_6);
    if (!registerNativeServerFunction(env)) {
        return -1;
    }

    return JNI_VERSION_1_6;
}

JNIEXPORT void JNI_OnUnload(JavaVM* vm, void* reserved) {
    LOGI("JNI_OnUnload");
}





