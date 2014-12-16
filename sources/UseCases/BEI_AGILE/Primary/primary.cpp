/*
 * Use case BEI AGILE Primary
 */
#include "CBasefunction.h"
#include <iostream>

int main(int argc, char *argv[]) {
    int redemarrage = atoi(argv[7]);
    int position = atoi(argv[6]);
    char * name_machine = argv[0];
    GUI_ARINC_partition("Primary", position, redemarrage);
    int nbarg = argc;
    char **argument = new char*[argc];
    int i = 0;
    for (i = 0; i <= nbarg; i++) {
        argument[i] = argv[i];
    }
    COMMUNICATION_VECTOR myCvector;
    myCvector = init_communication(argument, NULL);
    Type_Message rMessage;
    int portIDQ0, portIDQ1, portIDS0;
    int sockQ0, sockQ1, sockS0;
    vector_get(&(myCvector.vqueuing_port), 0, &portIDQ0);
    vector_get(&(myCvector.vqueuing_socket), 0, &sockQ0);
    vector_get(&(myCvector.vqueuing_port), 1, &portIDQ1); //port à utiliser pour l'envoi des maessages
    vector_get(&(myCvector.vqueuing_socket), 1, &sockQ1); // socket à utiliser pour l'envoi des messages

    vector_get(&(myCvector.vsamp_port), 0, &portIDS0);
    vector_get(&(myCvector.vsamp_socket), 0, &sockS0);

    int ifmsg = 0;
    int j = 0;
    int ret = 0;
    for (;;) {
        char sMessage[256];


        sprintf(sMessage, "QMessage envoye depuis Primary numero %d", j);
        std::cout << "			" << std::endl;
        std::cout << ">>> Sending message: " << sMessage << std::endl;
        SEND_QUEUING_MESSAGE(name_machine, portIDQ1, sockQ1, myCvector.emetteur, sMessage);
        j++;
        std::cout << "Queuing message sent: " << sMessage << std::endl;

        sprintf(sMessage, "SMessage from primary #%d", j);
        std::cout << "			" << std::endl;
        std::cout << ">>> Sending message: " << sMessage << std::endl;
        ret = WRITE_SAMPLING_MESSAGE(name_machine, portIDS0, sockS0, myCvector.emetteur, sMessage);
        if (ret == -1) {
            perror("WRITE_SAMPLING_MESSAGE : ");
        }

        ifmsg = RECEIVE_QUEUING_MESSAGE(sockQ0, &rMessage);
        if (ifmsg > 0) {
            std::cout << "			" << std::endl;
            std::cout << "<<< Receiving message from: " << rMessage.m_sender << " - Length: " << rMessage.m_length << std::endl;
            std::cout << "<<< Message: " << rMessage.m_message << std::endl;
        } else {
            std::cout << "No new message" << std::endl;
        }

        sleep(1);
    }
    return 0;
}

