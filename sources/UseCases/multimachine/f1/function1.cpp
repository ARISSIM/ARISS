#include "include/CBasefunction.h"

int main(int argc, char *argv[]) {
    ////////////		initialization		//////////
    int redemarrage = atoi(argv[7]);
    int position = atoi(argv[6]);
    GUI_ARINC_partition("Partition1", position, redemarrage);

    int nbarg = argc;
    char **argument =new char*[argc];
    int i = 0;
    for (i = 0; i <= nbarg; i++) {
        argument[i] = argv[i];
    }
    COMMUNICATION_VECTOR myCvector;
    myCvector = init_communication(nbarg, argument, NULL);

    //////////////////////////////////
    //INIT comm Queuing //
    /////////////////////////////////
    CQueuing Qservice;
    char result[256];
    std::string targetHostIP;
    targetHostIP = "casals.laas.fr"; //beware this IP adress is harcoded !!!!
    size_t size = targetHostIP.size() + 1;
    char * machine_name = new char[ size ];
    // copier la chaîne
    strncpy(machine_name, targetHostIP.c_str(), size);
    int ret = 0;
    i = 0;
    sleep(4);
    for (;;) {
        i++;
        sprintf(result, "marthym f1 : message numero %d", i);
        Qservice.WRITE_QUEUING_MESSAGE(machine_name, myCvector.vqueuing_port[0], myCvector.vqueuing_socket[0], myCvector.emetteur, result);
        std::cout << "emis vers partition 2 : " << machine_name << " " << myCvector.vqueuing_port[0] << " " << myCvector.vqueuing_socket[0] << " " << myCvector.emetteur << " " << result << std::endl;
        //	std::cout<<"queuing message wrote to the port :"<<vqueuing_port[0]<<std::endl;

        ret = Qservice.READ_QUEUING_MESSAGE(myCvector.vqueuing_socket[0]);
        if (ret == 1) {
            Qservice.Display_Message();
            ret = 0;
        }
        sleep(2);
    }
}

