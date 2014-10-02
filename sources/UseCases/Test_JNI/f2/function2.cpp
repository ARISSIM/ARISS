#include "include/CBasefunction.h"

int main(int argc, char *argv[])
{
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
    myCvector = init_communication(nbarg, argument, NULL);


    CQueuing Qservice;

    int ifmessage = 0;

    for (;;) {
        std::cout << "debur boucle for, vqueuing_socket[0] : " << myCvector.vqueuing_socket[0] << std::endl;

        ifmessage = Qservice.READ_QUEUING_MESSAGE(myCvector.vqueuing_socket[0]);
        if (ifmessage > 0)
            Qservice.Display_Message();
    }

}

