/*
 * Use case BEI AGILE Backup
 */

#include "CBasefunction.h"
#include <iostream>

int main(int argc, char *argv[]) {
    int redemarrage = atoi(argv[7]);
    int position = atoi(argv[6]);
    //char * name_machine = argv[0];
    GUI_ARINC_partition("Backup", position, redemarrage);
    int nbarg = argc;
    char **argument = new char*[argc];
    int i = 0;
    for (i = 0; i <= nbarg; i++) {
        argument[i] = argv[i];
    }
    COMMUNICATION_VECTOR myCvector;
    myCvector = init_communication(argument, NULL);
    Type_Message rMessage;
    // Type_Message *rSMessage;

    int portIDQ0, portIDQ1, portIDS0;
    int sockQ0, sockQ1, sockS0;
    vector_get(&(myCvector.vqueuing_port), 0, &portIDQ0);
    vector_get(&(myCvector.vqueuing_socket), 0, &sockQ0);
    vector_get(&(myCvector.vqueuing_port), 1, &portIDQ1);
    vector_get(&(myCvector.vqueuing_socket), 1, &sockQ1);
    vector_get(&(myCvector.vsamp_port), 0, &portIDS0);
    vector_get(&(myCvector.vsamp_socket), 0, &sockS0);

    int ifmsg = 0;

    for (;;) {

        ifmsg = RECEIVE_QUEUING_MESSAGE(sockQ0, &rMessage);
        if (ifmsg > 0) {
            std::cout << "			" << std::endl;
            std::cout << "<<< Receiving message from: " << rMessage.m_sender << " - Length: " << rMessage.m_length << std::endl;
            std::cout << "<<< Message: " << rMessage.m_message << std::endl;
        } else {
            // std::cout << "No new message" << std::endl;
        }

        ifmsg = RECEIVE_QUEUING_MESSAGE(sockQ1, &rMessage);
        if (ifmsg > 0) {
            std::cout << "			" << std::endl;
            std::cout << "<<< QMessage from: " << rMessage.m_sender << " - Length: " << rMessage.m_length << std::endl;
            std::cout << "<<< Message: " << rMessage.m_message << std::endl;
        } else {
            // std::cout << "No new message" << std::endl;
        }

        ifmsg = READ_SAMPLING_MESSAGE(sockS0, &rMessage);
        if (ifmsg > 0) {
            std::cout << "			" << std::endl;
            // std::cout << "<<< Receiving message from: " << rMessage->m_sender << " - Length: " << rMessage->m_length << std::endl;
            std::cout << "<<< Message: " << rMessage.m_message << std::endl;
        } else {
            //            std::cout << "			" << std::endl;
            std::cout << "<<< Not new, Message: " << rMessage.m_message << std::endl;
        }
        sleep(1);
    }
    return 0;
}
