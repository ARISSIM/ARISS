#include "CBasefunction.h"
#include <iostream>

int main(int argc, char *argv[]) {
    int redemarrage = atoi(argv[7]);
    int position = atoi(argv[6]);
    GUI_ARINC_partition("Partition1", position, redemarrage);
    int nbarg = argc;
    char **argument = new char*[argc];
    int i = 0;
    for (i = 0; i <= nbarg; i++) {
        argument[i] = argv[i];
    }
    COMMUNICATION_VECTOR myCvector;
    myCvector = init_communication(argument, NULL);

    int samp_port;
    vector_get(&(myCvector.vsamp_port), 0, &samp_port);

    int samp_socket;
    vector_get(&(myCvector.vsamp_socket), 0, &samp_socket);

    Type_Message *rMessage;

    char sMessage[256];
    i = 0;
    int ret = 0;
    for (;;) {
        
        sprintf(sMessage, "message envoye depuis f1 numero %d", i);
        std::cout << "			" << std::endl;
        std::cout << ">>> Sending message: " << sMessage << std::endl;
        WRITE_SAMPLING_MESSAGE(argv[0], samp_port, samp_socket, myCvector.emetteur, sMessage);
std::cout << "TOTO" << std::endl;
        ret = READ_SAMPLING_MESSAGE(samp_socket, rMessage);
std::cout << "TITI" << std::endl;
        if (ret > 0) {
            std::cout << "			" << std::endl;
            std::cout << "<<< Receiving message from: " << rMessage->m_sender << " - Length: " << rMessage->m_length << std::endl;
            std::cout << "<<< Message: " << rMessage->m_message << std::endl;
        }else{
            std::cout << "			" << std::endl;
            std::cout << "<<< No new message" << std::endl;
        }
        
        i++;
        sleep(1);
    }

}



