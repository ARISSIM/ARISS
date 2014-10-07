#include "CBasefunction.h"
#include <iostream>

int main(int argc, char *argv[]) {
    
    printf("dasn f11 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

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

    char result[256];
    i = 0;
    int ret = 0;
    for (;;) {

        sprintf(result, "message envoye depuis f1 numero %d", i);
        std::cout << "TEXTE A EMETTRE: " << result << std::endl;
        WRITE_SAMPLING_MESSAGE(argv[0], samp_port, samp_socket, myCvector.emetteur, result);

        ret = READ_SAMPLING_MESSAGE(samp_socket, rMessage);
        
        std::cout << "			" << std::endl;
        std::cout << "message from : " << rMessage->m_sender << " length : " << rMessage->m_length << std::endl;
        std::cout << "receive :" << rMessage->m_message << std::endl;
        std::cout << "			" << std::endl;        std::cout << "ret" << ret << std::endl;
        
        i++;
        sleep(1);
    }

}



