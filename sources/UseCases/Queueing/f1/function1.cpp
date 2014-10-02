#include "CBasefunction.h"

#include <iostream>

int main(int argc, char *argv[]) {
    int redemarrage = atoi(argv[7]);
    int position = atoi(argv[6]);
    GUI_ARINC_partition("Partition1", position, redemarrage);
    Type_Message rMessage;
    int nbarg = argc;
    char **argument = new char*[argc];
    int i = 0;
    for (i = 0; i <= nbarg; i++) {
        argument[i] = argv[i];
    }
    COMMUNICATION_VECTOR myCvector;
    myCvector = init_communication(nbarg, argument, NULL);

    //////////////////////
    //INIT comm Queuing //
    //////////////////////
    //    CQueuing Qservice;
    int ifmsg = 0;
    std::cout << "before loop" << std::endl;

int portID;
        int sock;
        vector_get(&(myCvector.vqueuing_port), 0, &portID);
        vector_get(&(myCvector.vqueuing_socket), 0, &sock);
    for (int i = 0; i < 20; i++) {

        std::cout << "into the loop" << std::endl;

        char result[256];
        sprintf(result, "message envoye depuis f1 numero %d", i);
        
        SEND_QUEUING_MESSAGE(argv[0], portID, sock, myCvector.emetteur, result);
        //     Qservice.WRITE_QUEUING_MESSAGE(argv[0], myCvector.vqueuing_port[0], myCvector.vqueuing_socket[0], myCvector.emetteur, result);
        std::cout << "queuing message wrote to the port :" << portID << std::endl;
        sleep(1);
    }


    for (;;) {

        std::cout << "Dans le boucle for, vqueuing_socket[0] : " << sock << std::endl;
        ifmsg = RECEIVE_QUEUING_MESSAGE(sock, &rMessage);
        //Qservice.READ_QUEUING_MESSAGE(sock);
        if (ifmsg > 0) {
            std::cout << "			" << std::endl;
            std::cout << "Display message : " << rMessage.m_message << std::endl;
            std::cout << "length " << rMessage.m_length << std::endl;
            std::cout << "total length " << sizeof (rMessage) << std::endl;
            std::cout << "receive :" << rMessage.m_message << std::endl;
            std::cout << "			" << std::endl;
        }
    }
}

