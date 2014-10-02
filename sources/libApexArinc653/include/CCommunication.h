#ifndef _communication_
#define _communication_

#define MSG_LENGTH 1000
#include "Vector.h"
#include "stdbool.h"

typedef struct Type_Message {
    char m_sender[MSG_LENGTH];
    int m_length;
    char m_message[MSG_LENGTH];
} Type_Message;

typedef struct SAMPLING_PORT_STATUS_TYPE {
    int MAX_MESSAGE_SIZE;
    bool PORT_DIRECTION;
    int REFRESH_PERIOD;
    int LAST_MSG_VALIDITY;
} SAMPLING_PORT_STATUS_TYPE;

typedef struct COMMUNICATION_VECTOR {
    char* emetteur;
    vector vsamp_socket; //int
    vector vqueuing_socket; //int
    vector vsamp_port; //int
    vector vqueuing_port; //int
} COMMUNICATION_VECTOR;

#endif
