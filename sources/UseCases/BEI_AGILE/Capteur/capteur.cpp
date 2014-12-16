
/*
 * Use case BEI AGILE capteur
 */
#include "CBasefunction.h"

#include <iostream>

int main(int argc, char *argv[]) {
    int redemarrage = atoi(argv[7]);
    int position = atoi(argv[6]);
    char * name_machine = argv[0];
    GUI_ARINC_partition("Capteur", position, redemarrage);

    Type_Message rMessage; // Objet message que vous allez utiliser

    int nbarg = argc;
    char **argument = new char*[argc];
    int i = 0;
    for (i = 0; i <= nbarg; i++) {
        argument[i] = argv[i];
    }
    COMMUNICATION_VECTOR myCvector;
    myCvector = init_communication(argument, NULL);

    //////////////////////
    //INIT comm Queuing //
    //////////////////////
    int ifmsg = 0;
    int j = 0;

    int portID0, portID1;
    int sock0, sock1;

    vector_get(&(myCvector.vqueuing_port), 0, &portID0); //port à utiliser pour l'envoi des maessages
    vector_get(&(myCvector.vqueuing_socket), 0, &sock0); // socket à utiliser pour l'envoi des messages

    vector_get(&(myCvector.vqueuing_port), 1, &portID1); //port à utiliser pour l'envoi des maessages
    vector_get(&(myCvector.vqueuing_socket), 1, &sock1); // socket à utiliser pour l'envoi des messages

    for (;;) {
        char sMessage[256];
        sprintf(sMessage, "message capteur numero %d", j); // message à evoyer
        std::cout << "			" << std::endl;
        std::cout << ">>> Sending message: " << sMessage << std::endl;
        SEND_QUEUING_MESSAGE(name_machine, portID0, sock0, myCvector.emetteur, sMessage);
        j++;

        sprintf(sMessage, "message capteur numero %d", j); // message à evoyer
        std::cout << "			" << std::endl;
        std::cout << ">>> Sending message: " << sMessage << std::endl;
        SEND_QUEUING_MESSAGE(name_machine, portID1, sock1, myCvector.emetteur, sMessage);
        j++;


        sleep(1);
    }
    return 0;
}

