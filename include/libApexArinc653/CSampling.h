#ifndef CSAMPLING_H
#define CSAMPLING_H 
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include "CCommunication.h"

int WRITE_SAMPLING_MESSAGE(char *name, int portId, int sock, char *emetteur, char *message);
int READ_SAMPLING_MESSAGE(int sock, Type_Message *rMessage);

#endif
