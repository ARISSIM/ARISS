/*
 * Use case petshop function2.cpp
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
    
    char *name = argv[0];
    char *emetteur = myCvector.emetteur;
    
    int queuingPortID;
    int queuingSock;
    vector_get(&(myCvector.vqueuing_port), 0, &queuingPortID);
    std::cout << "QueingPort : " << queuingPortID << std::endl;
    vector_get(&(myCvector.vqueuing_socket), 0, &queuingSock);
    std::cout << "Queuing socket : " << queuingSock << std::endl;
    
    int samplingPortID;
    int samplingSock;
    vector_get(&(myCvector.vsamp_port), 0, &samplingPortID);
    std::cout << "Sampling port : " << samplingPortID << std::endl;
    vector_get(&(myCvector.vsamp_socket), 0, &samplingSock);
    std::cout << "Sampling socket : " << samplingSock << std::endl;

    int ifmessage = 0;
    char sMessage[256];
    int j = 0;

    for (;;) {
        ifmessage = RECEIVE_QUEUING_MESSAGE(queuingSock, &rMessage);
        if (ifmessage > 0) {
            std::cout << "			" << std::endl;
            std::cout << "<<< Receiving queuing message from: " << rMessage.m_sender << " - Length: " << rMessage.m_length << std::endl;
            std::cout << "<<< Message (queuing): " << rMessage.m_message << std::endl;
        }else{
            std::cout << "<<< No new queuing message. " << std::endl;
        }
        
        ifmessage = READ_SAMPLING_MESSAGE(samplingSock, &rMessage);
        if (ifmessage > 0) {
            std::cout << "			" << std::endl;
            std::cout << "<<< Receiving sampling message from: " << rMessage.m_sender << " - Length: " << rMessage.m_length << std::endl;
            std::cout << "<<< Message (sampling): " << rMessage.m_message << std::endl;
        }else{
            std::cout << "<<< No sampling message. " << std::endl;
        }
        
        sprintf(sMessage, "Message sent from Partition2 number: %d", j);
        std::cout << "			" << std::endl;
        std::cout << ">>> Sending message: " << sMessage << std::endl;
        SEND_QUEUING_MESSAGE(name, queuingPortID, queuingSock, emetteur, rMessage.m_message);
        WRITE_SAMPLING_MESSAGE(name, samplingPortID, samplingSock, emetteur, rMessage.m_message);
        j++;
        
        sleep(4);
    }
    return 0;
}

