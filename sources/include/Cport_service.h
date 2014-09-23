#ifndef CPORT_SERVICE_H
#define CPORT_SERVICE_H 

#include <vector>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>



class Cport_service
{
	
	public :
	
	Cport_service();
	int CREATE_SAMPLING_PORT(int portID,int portName, int maxMessage_size,bool portDirection, int refreshPeriod);
	int CREATE_QUEUING_PORT(int portID,int portName, int maxMessage_size,bool portDirection, int refreshPeriod);
	
	
	private :

//structure for future implementation	
/*	
struct SAMPLING_PORT_STATUS_TYPE{
	int MAX_MESSAGE_SIZE;
	bool PORT_DIRECTION;
	int REFRESH_PERIOD;
	int LAST_MSG_VALIDITY;
	};
*/	


//	bool m_source; // the port allows messages to be sent
//	int m_segmentLength;
	
};


#endif
