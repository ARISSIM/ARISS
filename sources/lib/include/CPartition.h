#ifndef CPARTITION_H
#define CPARTITION_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <vector>


class CPartition
{
public:
	CPartition(std::string nameProcess, std::string pathProcess,int time);
	void Display();

//accesseurs		
	std::string nameProcess();
	std::string pathProcess();

	std::vector<int> get_wSport();
	std::vector<int> get_wQport();
	std::vector<int> get_rSport();
	std::vector<int> get_rQport();
	
	std::vector<int> get_vSsock();
	std::vector<int> get_vQsock();
	
	int time();
	//int nbSPort();
	//int nbQport();

//initialisation des ports	
	int wSport_add(int wport);
	int wQport_add(int wport);
	int rSport_add(int rport);
	int rQport_add(int rport);
	
//initialisation des numéros de sockets	
	void vSsock_add(int sock);
	void vQsock_add(int sock);
	
	
private:
	std::string m_nameProcess;
	std::string m_pathProcess;
	int m_time;

	std::vector<int> m_vSsock; //numeros de descripteur des sockets créés pour la partition
	std::vector<int> m_vQsock;
	
	std::vector<int> m_vwSport; //numeros de ports dans lesquels la partition a le droit d'écrire
	std::vector<int> m_vwQport;

	std::vector<int> m_vrSport; //numeros de ports de reception de la partition
	std::vector<int> m_vrQport;
	
	int m_nbQueuingPort;
	int m_nbSamplingPort;
	
	
};
#endif // CPARTITION
