#include "include/CBasefunction.h"

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
    myCvector = init_communication(nbarg, argument, NULL);

    //////////////////////
    //INIT comm Queuing //
    //////////////////////
    CQueuing Qservice;
    int ifmsg = 0;
    std::cout << "before loop" << std::endl;


    for (int i = 0; i < 20; i++) {

        std::cout << "into the loop" << std::endl;

        char result[256];
        sprintf(result, "message envoye depuis f1 numero %d", i);
        Qservice.WRITE_QUEUING_MESSAGE(argv[0], myCvector.vqueuing_port[0], myCvector.vqueuing_socket[0], myCvector.emetteur, result);
        std::cout << "queuing message wrote to the port :" << myCvector.vqueuing_port[0] << std::endl;
        sleep(1);
    }


    for (;;) {

        std::cout << "Dans le boucle for, vqueuing_socket[0] : " << myCvector.vqueuing_socket[0] << std::endl;
        ifmsg = Qservice.READ_QUEUING_MESSAGE(myCvector.vqueuing_socket[0]);
        if (ifmsg > 0) {
            Qservice.Display_Message();
        }
    }
}

