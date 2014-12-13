#include "CManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
#include <strings.h>
#include <vector>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <signal.h>
#include <semaphore.h>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/time.h>

struct sigaction m_action;

/**
 * \brief {CManager main class of the ARINC 653 emulator, it manage the whole partition creation and scheduling
 * it makes the initialization of arguments}
 * \ 
 * \ The 2 following path are hardcoded, that's why the make file creates automaticaly symlinks
 */

CManager::CManager() {
    m_vpart.clear();
    std::vector<CPartition>(m_vpart).swap(m_vpart);
    m_filepath = "Configuration/Config.txt";
    m_compath = "Configuration/Communication.txt";


    init_process();
    create_part();
}

void CManager::init_process() {
    CConfig config;
    std::vector<int> vSWPort;
    std::vector<int> vQWPort;
    std::vector<int> vSRPort;
    std::vector<int> vQRPort;

    std::cout << "********** Process Initialization **********" << std::endl;
    config.read_process(m_vpart, m_filepath); //lecture des process a créer
    config.read_communication(m_vpart, m_compath);

    std::cout << "There are :" << m_vpart.size() << " partitions " << std::endl;
    std::cout << "   ***** Partitions parameters *****" << std::endl;

    for (unsigned int i = 0; i < m_vpart.size(); i++) {
        //on vide la mémoire des vectors
        vSWPort.clear();
        std::vector<int>(vSWPort).swap(vSWPort);

        vQWPort.clear();
        std::vector<int>(vQWPort).swap(vQWPort);

        vSRPort.clear();
        std::vector<int>(vSRPort).swap(vSRPort);

        vQRPort.clear();
        std::vector<int>(vQRPort).swap(vQRPort);


        std::cout << "name Process : " << (m_vpart[i]).nameProcess() << std::endl;
        std::cout << "path Process : " << (m_vpart[i]).pathProcess() << std::endl;
        std::cout << "time processing : " << (m_vpart[i]).time() << std::endl;

        vSWPort = (m_vpart[i]).get_wSport();
        for (unsigned int j = 0; j < vSWPort.size(); j++) {
            std::cout << " can write in sampling port number :" << vSWPort[j] << std::endl;
        }

        vQWPort = (m_vpart[i]).get_wQport();
        for (unsigned int j = 0; j < vQWPort.size(); j++) {
            std::cout << " can read in queuing port :" << vQWPort[j] << std::endl;
        }

        vSRPort = (m_vpart[i]).get_rSport();
        for (unsigned int j = 0; j < vSRPort.size(); j++) {
            std::cout << " sampling associated partion :" << vSRPort[j] << std::endl;
        }

        vQRPort = (m_vpart[i]).get_rQport();
        for (unsigned int j = 0; j < vQRPort.size(); j++) {
            std::cout << " queuing associated partion :" << vQRPort[j] << std::endl;
        }

        std::cout << "********** End of process initialization **********" << std::endl;
    }

}

/**
 * 
 */
void * CManager::f_thread_watchdogM(void * args) {
    struct arguments_f_thread_watchdogM *argt = (arguments_f_thread_watchdogM*) args;
    (argt->myCManager)->f_thread_watchdog(argt->pid_result);
    return NULL;
}

void CManager::f_thread_watchdog(pid_t *pid_found) {
    int stat;
    std::cout << "Watchdog thread for patition's monitoring" << std::endl;
    while (1) {
        pid_t pid_test = waitpid((pid_t) - 1, &stat, 0);
        *pid_found = pid_test;
        std::cout << "Partition failure detected" << std::endl;
    }
}

void CManager::create_part() {

    pid_t pid;
    pid_t pid_to_send;
    pid_t prec[10];
    pid_t pid_result = 0;
    pid_t pid_intermediaire = 0;

    struct arguments_f_thread_watchdogM arguments;

    //struct timeval time1, time2;



    std::vector<int> vSportID; //enregistre les ports sampling dans un vecteur
    std::vector<int> vQportID; //enregistre les ports queuing dans un vecteur
    std::vector<std::string> vpath; //enregistre le chemin de chaque partition

    char name_machine[40];
    Cport_service PortService;
    const char **partitionTable = new const char*[m_vpart.size()]; //table des process que l'on va exécuter

    std::string arg_id_partition_master = "b";
    std::string arg_id_partition_slave = "b";
    std::string arg_id_partition = "b";
    std::string *arg1 = new std::string[m_vpart.size()];
    std::string *arg2 = new std::string[m_vpart.size()];
    std::string *arg3 = new std::string[m_vpart.size()];
    std::string *arg4 = new std::string[m_vpart.size()];


    std::vector<int> vWSportID;
    std::vector<int> vWQportID;
    std::vector<int> vSampling_sock; //num de sockets de la partition
    std::vector<int> vQueuing_sock;

    pid_t *part_pid = new pid_t[m_vpart.size()]; //tableau des pids des partitions
    int ret;

    std::vector<int> vtimeTable;

    pthread_attr_t *thread_attributes;

    int g = 0;


    /*********************************/
    /*Saving partition execution path*/
    /*********************************/


    for (unsigned int i = 0; i < m_vpart.size(); i++) {
        vpath.push_back((m_vpart[i]).pathProcess());
        std::cout << "vpath " << vpath[i] << std::endl;
        partitionTable[i] = vpath[i].c_str();
        std::cout << "Partition : " << partitionTable[i] << std::endl;
    }


    /*****************************/
    /*	SOCKETS' CREATION  	 */
    /*****************************/


    for (unsigned int i = 0; i < m_vpart.size(); i++) //for each partition
    {
        //vector's initialization
        vSportID.clear();
        std::vector<int>(vSportID).swap(vSportID);
        vQportID.clear();
        std::vector<int>(vQportID).swap(vQportID);


        vSportID = (m_vpart[i]).get_rSport(); //enregistre les ports sampling dans un vecteur
        for (unsigned int j = 0; j < vSportID.size(); j++) //pour chaque port Sampling
        {
            std::cout << "INIT sampling port" << std::endl;
            std::cout << " PORT :" << vSportID[j] << std::endl;
            //creation du socket associé avec le numéro de port et enregistrement grâce à la méthode vSsock_init
            //de la partition i contenue dans le vecteur m_vpart
            (m_vpart[i]).vSsock_add(PortService.CREATE_SAMPLING_PORT(vSportID[j], i + 1, 30, true, 0)); //Les deux derniers arguments ne sont pas pertinents
            std::cout << "END init sampling port" << std::endl;
        }

        std::cout << "					  " << std::endl;

        vQportID = (m_vpart[i]).get_rQport(); //enregistre les ports queuing dans un vecteur
        for (unsigned int j = 0; j < vQportID.size(); j++) //pour chaque port Queuing
        {

            std::cout << "INIT queuing ports" << std::endl;
            std::cout << " PORT :" << vQportID[j] << std::endl;
            (m_vpart[i]).vQsock_add(PortService.CREATE_QUEUING_PORT(vQportID[j], i + 1, 30, true, 0)); //Les deux derniers arguments ne sont pas pertinents
            std::cout << "END INIT PORTS" << std::endl;
        }

    }

    /*****************************/
    /*		INIT ARGUMENTS	 */
    /*****************************/


    std::cout << "********** Arguments' Initialazion **********" << std::endl;



    gethostname(name_machine, sizeof (name_machine));
    const char *arg = name_machine;



    for (unsigned int j = 0; j < m_vpart.size(); j++) {

        vWSportID.clear();
        std::vector<int>(vWSportID).swap(vWSportID); // réinitialise la taille du vecteur
        vWQportID.clear();
        std::vector<int>(vWQportID).swap(vWQportID);
        vSampling_sock.clear();
        std::vector<int>(vSampling_sock).swap(vSampling_sock);
        vQueuing_sock.clear();
        std::vector<int>(vQueuing_sock).swap(vQueuing_sock);


        vWSportID = (m_vpart[j]).get_wSport();
        vWQportID = (m_vpart[j]).get_wQport();
        vSampling_sock = (m_vpart[j]).get_vSsock(); //num de sockets de la partition
        vQueuing_sock = (m_vpart[j]).get_vQsock();


        std::cout << "NAME PROCESS : " << (m_vpart[j]).nameProcess() << std::endl;
        std::cout << " SIZE " << vQueuing_sock.size() << std::endl;
        for (unsigned int k = 0; k < vQueuing_sock.size(); k++) {
            std::cout << " sock =" << vQueuing_sock[k] << std::endl;
        }

        arg1[j] = init_arg(vSampling_sock);
        std::cout << " arg1 =" << arg1[j] << std::endl;

        arg2[j] = init_arg(vQueuing_sock);
        std::cout << " arg2 =" << arg2[j] << std::endl;

        arg3[j] = init_arg(vWSportID);
        std::cout << " arg3 =" << arg3[j] << std::endl;

        arg4[j] = init_arg(vWQportID);
        std::cout << " arg4 =" << arg4[j] << std::endl;
    }

    for (unsigned int i = 0; i < m_vpart.size(); i++) {
        std::cout << " arg1 " << i << " =" << arg1[i] << std::endl;
    }

    std::cout << "********** End of anguments' initialization **********" << std::endl;



    /*****************************/
    /*	CREATE PARTITIONS	 */
    /*****************************/



    for (unsigned int partitionNumber = 0; partitionNumber < m_vpart.size(); partitionNumber++) //boucle de fork().
    {
        std::cout << "Partition :" << partitionNumber << std::endl;
        std::cout << " Arg1 =" << arg1[partitionNumber] << std::endl;
        if ((pid = fork()) == 0) {
            //child
            std::cout << "IM the child and my PID is " << (int) getpid() << std::endl;
            std::cout << "Partition " << partitionNumber << std::endl;
            std::cout << "Path to binary : " << partitionTable[partitionNumber] << std::endl;
            sleep(1);
            char interm[3];
            sprintf(interm, "%d", partitionNumber);
            const char* argument5 = interm;
            const char* argument6[2] = {"0"};
            //            if (partitionNumber == 0) {
            //                arg_id_partition_master = "p";
            //                arg_id_partition = arg_id_partition_master;
            //            } else if (partitionNumber == 1) {
            //                arg_id_partition_slave = "b";
            //                arg_id_partition = arg_id_partition_slave;
            //            }
            ret = execlp(partitionTable[partitionNumber], arg, (arg1[partitionNumber]).c_str(), (arg2[partitionNumber]).c_str(), (arg3[partitionNumber]).c_str(), (arg4[partitionNumber]).c_str(), arg_id_partition.c_str(), argument5, argument6, NULL);

            if (ret == -1)
                perror(" exec ");

            exit(0);

        }
        prec[partitionNumber] = pid;
        part_pid[partitionNumber] = pid;
        std::cout << "IM the godfather and my PID is " << (int) getpid() << std::endl;
        std::cout << "I've created " << prec[partitionNumber] << std::endl;

        /*
         *\ Creation of monitoring thread if we are in manager
         */
        if (partitionNumber == 0) {
            // If in managager
            pid_to_send = prec[partitionNumber];
            arguments.pid_to_watch = &pid_to_send;
            arguments.myCManager = this;
            arguments.pid_result = &pid_result;
            thread_attributes = (pthread_attr_t *) malloc(sizeof (pthread_attr_t));
            thread_watchdogM = (pthread_t *) malloc(sizeof (pthread_t));
            pthread_attr_init(thread_attributes);
            if (pthread_create(thread_watchdogM, thread_attributes, (void*(*)(void*)) & CManager::f_thread_watchdogM, (void *) &arguments) != 0)
                perror(" Monitoring thread creation failed : ");
        }


    }//end initialazation fork(), every partition are running

    //pause le temps de finir d'initialiser les partitions /!\ valeur en dur
    usleep(10000);

    //mise en pause de toute les partitions
    for (unsigned int i = 0; i < m_vpart.size(); i++) {
        kill(part_pid[i], SIGSTOP); // suspend ieme partition
        std::cout << "part id :" << part_pid[i] << std::endl;
    }

    // creation de la table des temps

    for (unsigned int i = 0; i < m_vpart.size(); i++) {
        vtimeTable.push_back((m_vpart[i]).time());
    }


    /****************************************/
    /**			Schedulling				**/
    /****************************************/

    std::cout << "********** Scheduling loop**********" << std::endl;
    while (1) {
        if (pid_result != 0) {
            pid_intermediaire = pid_result;
            pid_result = 0;
            for (unsigned int partitionNumber = 0; partitionNumber < m_vpart.size(); partitionNumber++) {
                if (pid_intermediaire == prec[partitionNumber]) {
                    g = partitionNumber;
                }

            }
            std::cout << "Detection of partition failure... reboot..." << std::endl;

            if ((pid = fork()) == 0) {
                //child

                std::cout << "IM the child and my PID is " << (int) getpid() << std::endl;
                std::cout << "partition " << g << std::endl;
                std::cout << "path to binary " << partitionTable[g] << std::endl;
                sleep(1);
                char interm[3];
                sprintf(interm, "%d", g);
                const char* argument5 = interm;
                const char* argument6[2] = {"1"};
                if (g == 0) {
                    if (arg_id_partition_master.compare("p")) {
                        arg_id_partition_master = "b";
                        arg_id_partition = arg_id_partition_master;
                    }
                } else if (g == 1) {
                    if (arg_id_partition_slave.compare("p")) {
                        arg_id_partition_slave = "b";
                        arg_id_partition = arg_id_partition_slave;
                    }

                }
                ret = execlp(partitionTable[g], arg, (arg1[g]).c_str(), (arg2[g]).c_str(), (arg3[g]).c_str(), (arg4[g]).c_str(), arg_id_partition.c_str(), argument5, argument6, NULL);

                if (ret == -1)
                    perror(" exec ");
                exit(0);

            }
            prec[g] = pid;
            part_pid[g] = pid;
            std::cout << "IM the godfather and my PID is " << (int) getpid() << std::endl;
            std::cout << "I've created " << prec[g] << std::endl;
            pid_intermediaire = 0;
            g = 0;
        }
        for (unsigned int i = 0; i < m_vpart.size(); i++) {
//            std::cout << " **ACTIVATION PARTITION " << i + 1 << std::endl;
            kill(part_pid[i], SIGCONT); //réveille partition i
            usleep(vtimeTable[i]); //budget temps p1 // ordonnanceur s'endort
//            std::cout << " **ARRET PARTITION" << i + 1 << std::endl;
            kill(part_pid[i], SIGSTOP); // suspend la partition i

        }
    }

}

std::string CManager::init_arg(std::vector<int> varg) {
    std::string tab_arg;
    char intStr[10];
    std::string test;

    if (varg.size() == 0) {
        tab_arg = "0";
        return (tab_arg);
    }


    sprintf(intStr, "%d", varg[0]);
    tab_arg = std::string(intStr);


    if (varg.size() == 1) {
        return (tab_arg);
    }

    for (unsigned int i = 1; i < varg.size(); i++) {

        sprintf(intStr, "%d", varg[i]);
        test = std::string(intStr);
        tab_arg = tab_arg + " " + test;
    }
    return (tab_arg);
}




