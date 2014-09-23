#include "include/CBasefunction.h"

int main(int argc, char *argv[]) {
    int redemarrage = atoi(argv[7]);
    int position = atoi(argv[6]);
    GUI_ARINC_partition("Partition4", position, redemarrage);

    int nbarg = argc;
    char **argument = new char*[argc];
    int i = 0;
    for (i = 0; i <= nbarg; i++) {
        argument[i] = argv[i];
    }
    COMMUNICATION_VECTOR myCvector;
    myCvector = init_communication(nbarg, argument, NULL);


    //////////////////////////////////
    //INIT comm Sampling //
    /////////////////////////////////
    CSampling Sservice;
    char result[256];
    std::string targetHostIP;
    targetHostIP = "casals.laas.fr"; //beware this IP adress is harcoded !!!!
    size_t size = targetHostIP.size() + 1;
    char * machine_name = new char[ size ];
    // copier la chaîne
    strncpy(machine_name, targetHostIP.c_str(), size);
    i = 0;
    sleep(4);
    for (;;) {
        i++;
        sprintf(result, "marthym f4 : sampling message numero %d", i);
        Sservice.WRITE_SAMPLING_MESSAGE(machine_name, myCvector.vsamp_port[0], myCvector.vsamp_socket[0], myCvector.emetteur, result);

        Sservice.READ_SAMPLING_MESSAGE(myCvector.vsamp_socket[0]);
        Sservice.Display_Message();
        sleep(2);
    }

}

