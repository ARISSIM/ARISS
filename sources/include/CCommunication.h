#ifndef _communication_
#define _communication_

#define MSG_LENGTH 1000
#include <vector>

struct Type_Message
	{
	char m_sender[MSG_LENGTH];
	int m_length;
	char m_message[MSG_LENGTH];
	};
	
struct SAMPLING_PORT_STATUS_TYPE
	{
	int MAX_MESSAGE_SIZE;
	bool PORT_DIRECTION;
	int REFRESH_PERIOD;
	int LAST_MSG_VALIDITY;
	};
	
struct COMMUNICATION_VECTOR
	{
	std::string emetteur;
	std::vector<int> vsamp_socket ;
	std::vector<int> vqueuing_socket;
	std::vector<int> vsamp_port;
	std::vector<int> vqueuing_port;
	};

#endif
