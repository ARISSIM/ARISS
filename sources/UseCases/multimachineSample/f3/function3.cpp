#include "include/CBasefunction.h"

int main(int argc, char *argv[]) {

    int redemarrage = atoi(argv[7]);
    int position = atoi(argv[6]);
    GUI_ARINC_partition("Partition3", position, redemarrage);

    /*if (argc > 3)
            {*/

    for (int count = 1; count < argc; count++) {
        std::cout << "argv " << count << " = " << argv[count] << std::endl;
    }

    std::string emetteur = argv[0]; //NOM DU BINAIRE QUI SERA SON IDENFIANT POUR COMMUNIQUER

    std::cout << "nb arg :" << argc << std::endl;
    std::string arg_samp_sock = argv[1]; //ligne à découper
    std::string arg_Qsock = argv[2];
    std::string arg_Sport = argv[3];
    std::string arg_Qport = argv[4];

    CArgument ARG;
    std::vector<int> vsamp_socket = ARG.split_arg(arg_samp_sock);
    std::vector<int> vqueuing_socket = ARG.split_arg(arg_Qsock);
    std::vector<int> vsamp_port = ARG.split_arg(arg_Sport);
    std::vector<int> vqueuing_port = ARG.split_arg(arg_Qport);


    for (unsigned int i = 0; i < vsamp_socket.size(); i++) {
        std::cout << " use socket :" << vsamp_socket[i] << " for sampling comm" << std::endl;
    }
    for (unsigned int i = 0; i < vqueuing_socket.size(); i++) {
        std::cout << " use socket :" << vqueuing_socket[i] << " for queuing com" << std::endl;
    }
    for (unsigned int i = 0; i < vsamp_port.size(); i++) {
        std::cout << " write in port :" << vsamp_port[i] << "by sampling" << std::endl;
    }
    for (unsigned int i = 0; i < vqueuing_port.size(); i++) {
        std::cout << " write in port :" << vqueuing_port[i] << "by queuing" << std::endl;
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
