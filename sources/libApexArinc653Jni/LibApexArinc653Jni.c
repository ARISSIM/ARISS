#include <jni.h>
#include "CBasefunction.h"
#include "LibApexArinc653Jni.h"
#include "CSampling.h"
#include "CQueuing.h"

JNIEXPORT jint JNICALL Java_LibApexArinc653Jni_writeSamplingMessage
(JNIEnv *env, jobject obj, jstring jName, jint jPortId, jint jSock, jstring jEmetteur, jstring jMessage) {
    char *name = strdup(env->GetStringUTFChars(jName, 0));
    int portId = jPortId;
    int sock = jSock;
    char *emetteur = strdup(env->GetStringUTFChars(jEmetteur, 0));
    char *message = strdup(env->GetStringUTFChars(jMessage, 0));
    int toReturn;
    toReturn = WRITE_SAMPLING_MESSAGE(name, portId, sock, emetteur, message);
    return toReturn;
}

JNIEXPORT jint JNICALL Java_LibApexArinc653Jni_sendQueuingMessage
(JNIEnv *env, jobject obj, jstring jName, jint jPortId, jint jSock, jstring jEmetteur, jstring jMessage) {
    char *name = strdup(env->GetStringUTFChars(jName, 0));
    int portId = jPortId;
    int sock = jSock;
    char *emetteur = strdup(env->GetStringUTFChars(jEmetteur, 0));
    char *message = strdup(env->GetStringUTFChars(jMessage, 0));
    int toReturn;
    toReturn = SEND_QUEUING_MESSAGE(name, portId, sock, emetteur, message);
    return toReturn;
}