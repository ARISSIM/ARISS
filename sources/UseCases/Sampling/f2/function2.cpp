#include "include/CBasefunction.h"

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
    myCvector = init_communication(nbarg, argument, NULL);

    int ret;

    CSampling Sservice;
    for (;;) {
        ret = Sservice.READ_SAMPLING_MESSAGE(myCvector.vsamp_socket[0]);
        std::cout << "ret" << ret << std::endl;
        Sservice.Display_Message();

        Sservice.WRITE_SAMPLING_MESSAGE(argv[0], myCvector.vsamp_port[0], myCvector.vsamp_socket[0], myCvector.emetteur, " Hello");
        sleep(1);
    }

}

