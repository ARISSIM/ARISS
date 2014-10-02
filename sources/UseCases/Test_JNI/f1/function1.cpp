#include "include/CBasefunction.h"

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
    myCvector = init_communication(nbarg, argument, NULL);
        
        std::string name = argv[0];
        int port = myCvector.vqueuing_port[0];
        int sock = myCvector.vqueuing_socket[0];
        std::string emetteur = myCvector.emetteur;
        
        std::string commande = "java Arinc653JniCom " + name + " " + convertInt(port) + " " + convertInt(sock) + " " + emetteur;
        system(commande.c_str());
    
}

std::string convertInt(int num){
    std::stringstream ss;
    ss << num;
    return ss.str();
}

