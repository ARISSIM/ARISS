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

    int samp_port;
    vector_get(&(myCvector.vsamp_port), 0, &samp_port);

    int samp_socket;
    vector_get(&(myCvector.vsamp_socket), 0, &samp_socket);

    Type_Message *rMessage;
    char sMessage[256];

    int ret;

    //int WRITE_SAMPLING_MESSAGE(char *name, int portId, int sock,char *emetteur, char *message);
    //int READ_SAMPLING_MESSAGE(int sock, Type_Message *rMessage);
    //        int GUI_ARINC_partition(char* name_partition, int position, int redemarrage);
    //void GUI_ARINC_partition_kill(int pid_to_kill);
    //
    //COMMUNICATION_VECTOR init_communication(int nbarg, char* argument[], char** mode);    
    for (;;) {
        std::cout << "In boucle for" << std::endl;
        
        ret = READ_SAMPLING_MESSAGE(samp_socket, rMessage);
        std::cout << "RET: " << ret << std::endl;
        if (ret > 0) {
            std::cout << "			" << std::endl;
            std::cout << "Message from: " << rMessage->m_sender << " Length: " << rMessage->m_length << std::endl;
            std::cout << "Message: " << rMessage->m_message << std::endl;
        }else{
            std::cout << "			" << std::endl;
            std::cout << "No new message" << std::endl;
        }
        
        sprintf(sMessage, "Hello");
        WRITE_SAMPLING_MESSAGE(argv[0], samp_port, samp_socket, myCvector.emetteur, sMessage);

        sleep(1);
    }


}
