#ifndef LIB_APEXARINC653_H
#define LIB_APEXARINC653_H

#include "CCommunication.h"
#include "CQueuing.h"
#include "CSampling.h"
#include "CBasefunction.h"

int SEND_QUEUING_MESSAGE(char *name, int portId, int sock, char *emetteur, char *message);
int RECEIVE_QUEUING_MESSAGE(int sock, struct Type_Message *rMessage);
int WRITE_SAMPLING_MESSAGE(char *name, int portId, int sock, char *emetteur, char *message);
int READ_SAMPLING_MESSAGE(int sock, struct Type_Message *rMessage);
int GUI_ARINC_emulator();
int GUI_ARINC_partition(char* name_partition, int position, int redemarrage);

#endif
