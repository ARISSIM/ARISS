#include <jni.h>
#include "CBasefunction.h"
#include "LibApexArinc653Jni.h"
//#include "libapexarinc653jni_LibApexArinc653Jni.h"
#include "CSampling.h"
#include "CQueuing.h"

JNIEXPORT jint JNICALL Java_libapexarinc653jni_LibApexArinc653Jni_writeSamplingMessage
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

JNIEXPORT jobject JNICALL Java_libapexarinc653jni_LibApexArinc653Jni_readSamplingMessage
(JNIEnv *env, jobject obj, jint jSock) {
    Type_Message *rMessage = (Type_Message*) malloc(sizeof (Type_Message));
    int sock = jSock;

    // Récupération de la classe ReturnObject.java
    jclass cReturnObject = env->FindClass("libapexarinc653jni/ReturnObject");
    // Récupération du constructeur paramétré de la classe avec les paramètres (jint, jstring, jint, jstring)
    jmethodID constructorReturnObject =  env->GetMethodID(cReturnObject, "<init>", "(ILjava/lang/String;ILjava/lang/String;)V");
    
    // Réception en C du message en queuing
    int cToReturn = 0;
    cToReturn = READ_SAMPLING_MESSAGE(sock, rMessage);
    
    // Création des jobjects pour les paramètres du constructeur de ReturnObject
    jint jReturnCode = cToReturn;
    jstring jSender = env->NewStringUTF(rMessage->m_sender);
    jint jLength = rMessage->m_length;
    jstring jMessage = env->NewStringUTF(rMessage->m_message);
    
    // Création de l'instance de ReturnObject pour renvoyer à l'application java
    jobject jToReturn = env->NewObject(cReturnObject,constructorReturnObject, jReturnCode, jSender, jLength, jMessage);

    return jToReturn;
}


JNIEXPORT jint JNICALL Java_libapexarinc653jni_LibApexArinc653Jni_sendQueuingMessage
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



JNIEXPORT jobject JNICALL Java_libapexarinc653jni_LibApexArinc653Jni_receiveQueuingMessage
(JNIEnv *env, jobject obj, jint jSock) {
    Type_Message *rMessage = (Type_Message*) malloc(sizeof (Type_Message));
    int sock = jSock;

    // Récupération de la classe ReturnObject.java
    jclass cReturnObject = env->FindClass("libapexarinc653jni/ReturnObject");
    // Récupération du constructeur paramétré de la classe avec les paramètres (jint, jstring, jint, jstring)
    jmethodID constructorReturnObject =  env->GetMethodID(cReturnObject, "<init>", "(ILjava/lang/String;ILjava/lang/String;)V");
    
    // Réception en C du message en queuing
    int cToReturn = 0;
    cToReturn = RECEIVE_QUEUING_MESSAGE(sock, rMessage);
    
    // Création des jobjects pour les paramètres du constructeur de ReturnObject
    jint jReturnCode = cToReturn;
    jstring jSender = env->NewStringUTF(rMessage->m_sender);
    jint jLength = rMessage->m_length;
    jstring jMessage = env->NewStringUTF(rMessage->m_message);
    
    // Création de l'instance de ReturnObject pour renvoyer à l'application java
    jobject jToReturn = env->NewObject(cReturnObject,constructorReturnObject, jReturnCode, jSender, jLength, jMessage);

    return jToReturn;
}



/*
 *  Fonctions hors communication 
 */

/*
JNIEXPORT jobject JNICALL Java_LibApexArinc653Jni_initCommunication
  (JNIEnv *env, jobject obj, jstring jArg, jstring jMode) {
    COMMUNICATION_VECTOR cVector;
    char* arg = strdup(env->GetStringUTFChars(jArg, 0));
    char* mode = strdup(env->GetStringUTFChars(jMode, 0));
    cVector = init_communication(&arg,&mode);
    
    // Getting CommunicationVector class and methods
    jclass cCommunicationVector = env->FindClass("CommunicationVector");
    jmethodID CommunicationVector =  env->GetMethodID(cCommunicationVector, "CommunicationVector", "(Ljava/lang/String;)V");
    jmethodID setSender =  env->GetMethodID(cCommunicationVector, "setSender", "(Ljava/lang/String;)V");
    jmethodID addSamplingSocket =  env->GetMethodID(cCommunicationVector, "addSamplingSocket", "(I)V");
    jmethodID addQueuingSocket =  env->GetMethodID(cCommunicationVector, "addQueuingSocket", "(I)V");
    jmethodID addSamplingPort =  env->GetMethodID(cCommunicationVector, "addSamplingPort", "");
    jmethodID addQueuingPort =  env->GetMethodID(cCommunicationVector, "addQueuingPort", "(I)V");
    
    // Instanciaton of a new jobject of CommunicationVector type
    jstring sender = env->NewStringUTF(cVector.emetteur);
    jobject comVector = env->NewObject(cCommunicationVector, CommunicationVector, sender);
    
    int i = 0;
    int tmp = 0;
    
    // Filling comVector samplingSocket vector
    for(i = 0; i < cVector.vsamp_socket.size; i++) {
        vector_get(&(cVector.vsamp_socket), i, &tmp);
        env->CallVoidMethod(comVector, addSamplingSocket, tmp);
    }
    
    // Filling comVector queuingSocket vector
    for(i = 0; i < cVector.vqueuing_socket.size; i++) {
        vector_get(&(cVector.vqueuing_socket), i, &tmp);
        env->CallVoidMethod(comVector, addQueuingSocket, tmp);
    }
    
    // Filling comVector samplingPort vector
    for(i = 0; i < cVector.vsamp_port.size; i++) {
        vector_get(&(cVector.vsamp_port), i, &tmp);
        env->CallVoidMethod(comVector, addSamplingPort, tmp);
    }
    
    // Filling comVector queuingPort vector
    for(i = 0; i < cVector.vqueuing_port.size; i++) {
        vector_get(&(cVector.vqueuing_port), i, &tmp);
        env->CallVoidMethod(comVector, addQueuingPort, tmp);
    }   
    
    return comVector;
}

JNIEXPORT jint JNICALL Java_LibApexArinc653Jni_guiArincPartition
  (JNIEnv *env, jobject obj, jstring jPartitionName, jint jPosition, jint jRestart) {
    int toReturn;
    char* partitionName = strdup(env->GetStringUTFChars(jPartitionName, 0));
    int position = jPosition;
    int restart = jRestart;
    toReturn = GUI_ARINC_partition(partitionName, position, restart);
    return toReturn;
}*/