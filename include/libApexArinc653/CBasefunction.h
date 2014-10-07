#ifndef CBASE_FUNCTIONS_H
#define  CBASE_FUNCTIONS_H 

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>

#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include "stdbool.h"

#include "Vector.h"
#include "CArgument.h"
#include "CSampling.h"
#include "CQueuing.h"
#include "CCommunication.h"

/************		Variables				**********/

/************		methods				**********/
int GUI_ARINC_partition(char* name_partition, int position, int redemarrage);
int GUI_ARINC_emulator();
void GUI_ARINC_partition_kill(int pid_to_kill);

COMMUNICATION_VECTOR init_communication(char* argument[], char** mode);

//static int open_pipe(const char* Output_pipe, const char* Input_pipe, const char* name_partition);
//static int create_pipe(const char* Output_pipe, const char* Input_pipe);
//static int read_in_pipe(char* input_chain);



#endif
