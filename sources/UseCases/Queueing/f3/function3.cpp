#include "include/CBasefunction.h"

int main(int argc, char *argv[]) {
    int redemarrage = atoi(argv[7]);
    int position = atoi(argv[6]);
    GUI_ARINC_partition("Partition3", position, redemarrage);
    int nbarg = argc;
    char **argument = new char*[argc];
    int i = 0;
    for (i = 0; i <= nbarg; i++) {
        argument[i] = argv[i];
    }
    COMMUNICATION_VECTOR myCvector;
    myCvector = init_communication(nbarg, argument, NULL);

    int ifmessage = 0;
    CQueuing Qservice;
    for (;;) {
        std::cout << "Dans le boucle for, vqueuing_socket[0] : " << myCvector.vqueuing_socket[0] << std::endl;
        Qservice.READ_QUEUING_MESSAGE(myCvector.vqueuing_socket[0]);
        if (ifmessage > 0)
            Qservice.Display_Message();
    }
    return 0;
}
