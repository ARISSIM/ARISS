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

    for (;;) {
        char sMessage[256];
        sprintf(sMessage, "Message envoye depuis f2 numero %d", j);
        std::cout << "			" << std::endl;
        std::cout << ">>> Sending message: " << sMessage << std::endl;
        SEND_QUEUING_MESSAGE(argv[0], portID, sock, myCvector.emetteur, sMessage);
        j++;
        std::cout << "Queuing message sent: " << sMessage << std::endl;
        
        ifmsg = RECEIVE_QUEUING_MESSAGE(sock, &rMessage);
        if (ifmsg > 0) {
            std::cout << "			" << std::endl;
            std::cout << "<<< Receiving message from: " << rMessage.m_sender << " - Length: " << rMessage.m_length << std::endl;
            std::cout << "<<< Message: " << rMessage.m_message << std::endl;
        }else{
            std::cout << "No new message" << std::endl;
        }
        
        sleep(1);
    }
    return 0;
}

