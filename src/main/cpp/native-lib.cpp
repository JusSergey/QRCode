#include <jni.h>
#include <string>
#include "AIVision.h"

UID_t getUID(RGBA *rgba, int width, int height) {
    AIVision *vision = new AIVision(rgba, width*height, width, height);
    //AIVision *vision = create("/mnt/sdcard/dump.log", 160, 160);
    auto uid = vision->getUID();
    delete vision;
    return uid;
    //AIVision vision("qe", 12, 34);
//    AIVision vision(rgba, (u_int32_t)len, width, height);
    //return vision.getUID();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_heart_qrcodeex_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_heart_qrcodeex_MainActivity_getGUIDUser(
        JNIEnv *env,
        jobject,
        jintArray pixels,
        jint width, jint height)
{
    jint *arrayPixels = env->GetIntArrayElements(pixels, nullptr);
    auto uid = getUID(reinterpret_cast<RGBA *>(arrayPixels), width, height);
    env->ReleaseIntArrayElements(pixels, arrayPixels, 0);
    return uid;
}
