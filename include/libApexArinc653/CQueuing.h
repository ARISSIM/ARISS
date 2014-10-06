#ifndef CQUEUING_H
#define CQUEUING_H 
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include "CCommunication.h"

#define MSG_LENGTH 1000

int SEND_QUEUING_MESSAGE(char *name, int portId, int sock,char *emetteur, char *message);
int RECEIVE_QUEUING_MESSAGE(int sock, Type_Message *rMessage);

#endif
