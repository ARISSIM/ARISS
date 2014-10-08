/*
 * Use case JNI_Template function1.cpp
 */
#include "CBasefunction.h"
#include <iostream>
#include <sstream>
std::string convertInt(int num);

int main(int argc, char *argv[])
{
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
        
        std::string name = argv[0];
        int portID;
        int sock;
        vector_get(&(myCvector.vqueuing_port), 0, &portID);
        std::cout << "QueingPort : " << portID << std::endl;
        vector_get(&(myCvector.vqueuing_socket), 0, &sock);
        std::cout << "Queuing socket : " << sock << std::endl;
        std::string emetteur = myCvector.emetteur;
        
        std::string commande = "java TestJNI " + name + " " + convertInt(portID) + " " + convertInt(sock) + " " + emetteur;
        system(commande.c_str());
    
}

std::string convertInt(int num){
    std::stringstream ss;
    ss << num;
    return ss.str();
}

