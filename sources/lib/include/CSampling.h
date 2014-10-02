#ifndef CSAMPLING_H
#define CSAMPLING_H 
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include "CCommunication.h"



class CSampling
{
	public :
	
	CSampling();
	CSampling(std::string name, int portID, int numSocket);
	int WRITE_SAMPLING_MESSAGE(char *name, int portId, int sock,std::string emetteur, std::string addr_message);
	int READ_SAMPLING_MESSAGE(int sock);
	void Display_Message();
	Type_Message get_Message();

	
	
	
	private :
	


	Type_Message myMessage;
	Type_Message myReceivedMessage;
	
	int portName;
	int m_id,m_numSocket;
	std::string m_name;
	std::vector<int> m_portId;
	std::vector<int> m_portName;
	//trasfert mode : sampling !
	bool m_source; // the port allows messages to be sent
	int m_segmentLength;
	
};


#endif
