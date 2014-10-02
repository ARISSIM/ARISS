#ifndef CMANAGER_H
#define CMANAGER_H

#include "CPartition.h"
#include "CConfig.h"
#include "Cport_service.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <signal.h>
#include <strings.h>

#define SEM_MAX 2




class CManager
{
public:
	CManager();
	
private :
	std::vector<CPartition> m_vpart;
	std::string m_filepath;
	std::string m_compath;
	std::string m_pidpath;
	pthread_t *thread_watchdogM;
	
	union semun
	{
		int val;
		struct semid_ds *buf;
	}argin,argout;
	
	void init_process(); //lecture des fichiers textes
	void create_part(); //lancement des partitions avec boucle fork et boucle d'ordonnancement
	std::string init_arg(std::vector<int> varg);
	static void * f_thread_watchdogM(void *);
	void f_thread_watchdog(pid_t*);
	
};

struct arguments_f_thread_watchdogM{
	pid_t *pid_to_watch;
	CManager* myCManager;
	pid_t *pid_result;
};

#endif // CMANAGER
