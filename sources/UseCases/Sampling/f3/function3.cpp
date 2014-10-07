#include "CBasefunction.h"
#include <iostream>

int main(int argc, char *argv[]) {

    int redemarrage = atoi(argv[7]);
    int position = atoi(argv[6]);
    GUI_ARINC_partition("Partition3", position, redemarrage);
    if (argc > 1) // if there is any communication, the configuration will be given in argument
    {
        int nbarg = argc;
        char **argument = new char*[argc];
        int i = 0;
        for (i = 0; i <= nbarg; i++) {
            argument[i] = argv[i];
        }
        COMMUNICATION_VECTOR myCvector;
        myCvector = init_communication(argument, NULL);
    }

    int t = 0;
    while (1) {
        std::cout << "je bosse :" << std::endl;
        for (t = 1; t < 100; t++) {
            std::cout << "time = " << t << std::endl;
            sleep(1);
        }

    }

}
