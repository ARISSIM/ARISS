#ifndef CQUEUING_H
#define CQUEUING_H 
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include "CCommunication.h"

#define MSG_LENGTH 1000

class CQueuing
{
	public :
	
	CQueuing();
	CQueuing(std::string name, int portID, int numSocket);
	int WRITE_QUEUING_MESSAGE(char *name, int portId, int sock,std::string emetteur, std::string addr_message);
	int READ_QUEUING_MESSAGE(int sock);
	void Display_Message();
	void Trace_Message(Type_Message *msg);
	Type_Message get_Message();
	
	
	private :
		

	
	Type_Message myMessage;
	Type_Message myReceivedMessage;
	
	int portName;
	int m_id,m_numSocket;
	std::string m_name;
	std::vector<int> m_portId;
	std::vector<int> m_portName;
	//trasfert mode : queueing !
	bool m_source; // the port allows messages to be sent
	int m_segmentLength;
	
};


#endif
