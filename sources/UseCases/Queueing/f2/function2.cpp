/*
 * Use case Queueing function2.cpp
 */
#include "CBasefunction.h"
#include <iostream>
int main(int argc, char *argv[]) {
    int redemarrage = atoi(argv[7]);
    int position = atoi(argv[6]);
	GUI_ARINC_partition("Partition2", position, redemarrage);
    int nbarg = argc;
    char **argument = new char*[argc];
    int i = 0;
    for (i = 0; i <= nbarg; i++) {
        argument[i] = argv[i];
    }
    COMMUNICATION_VECTOR myCvector;
    myCvector = init_communication(argument, NULL);
    Type_Message rMessage;
    int portID;
    int sock;
    vector_get(&(myCvector.vqueuing_port), 0, &portID);
    vector_get(&(myCvector.vqueuing_socket), 0, &sock);

    int ifmsg = 0;
    int j = 0;

    while(RECEIVE_QUEUING_MESSAGE(sock, &rMessage) <= 0) {
        ;
    }

    if(strcmp(rMessage.m_message,"P1 INIT_DONE") == 0) {
        printf("P2 Beginning of command loop... \n");
    } else {
        printf("p2 failed : Received %s from P1\n", rMessage.m_message);
        return 0;
    }
    int msgID = 0;
    int msgCnt = 0;
    for (;;) {
        if(RECEIVE_QUEUING_MESSAGE(sock, &rMessage)>0)
        {
            memcpy(&msgID, rMessage.m_message, 4);
            memcpy(&msgCnt, rMessage.m_message+4, 4);
            std::cout << "<<< recv msgID: " << msgID << " msgCnt:" << msgCnt <<std::endl;
        }

        /*
        char sMessage[256];
        sprintf(sMessage, "Message envoye depuis f2 numero %d", j);
        std::cout << "			" << std::endl;
        std::cout << ">>> Sending message: " << sMessage << std::endl;
        SEND_QUEUING_MESSAGE(argv[0], portID, sock, myCvector.emetteur, sMessage);
        j++;
        std::cout << "Queuing message sent: " << sMessage << std::endl;*/
        
       
        sleep(1);
    }
    return 0;
}

