/*
 * Use case JNI_Template function2.cpp
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
    vector_get(&(myCvector.vsamp_port), 0, &portID);
    std::cout << "SamplingPort : " << portID << std::endl;
    vector_get(&(myCvector.vsamp_socket), 0, &sock);
    std::cout << "Sampling socket : " << sock << std::endl;
    //    CQueuing Qservice;

    int ifmessage = 0;
    int cptMessage = 0;
    for (ifmessage=0; ifmessage < 10 ; ifmessage++){
            char sMessage[256];
            std::cout << "Sending sampling message numero " << ifmessage << std::endl;
            sprintf(sMessage, "Message envoye depuis f2 numero %d", ifmessage);
            WRITE_SAMPLING_MESSAGE(argv[0], portID, sock, myCvector.emetteur, sMessage);
    }
    for (;;) {
       // std::cout << "debur boucle for, vqueuing_socket[0] : " << sock << std::endl;
        ifmessage = READ_SAMPLING_MESSAGE(sock, &rMessage);
        if (ifmessage > 0) {
            //            Qservice.Display_Message();
            std::cout << "			" << std::endl;
            std::cout << "Display message : " << rMessage.m_message << std::endl;
            std::cout << "length " << rMessage.m_length << std::endl;
            std::cout << "total length " << sizeof (rMessage) << std::endl;
            std::cout << "receive :" << rMessage.m_message << std::endl;
            std::cout << "			" << std::endl;
        }else{
            std::cout << "No new sampling message" << std::endl;
        }
        sleep(1);

    }
    return 0;
}