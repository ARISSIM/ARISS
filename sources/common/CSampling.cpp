#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include "include/CSampling.h"

CSampling::CSampling()
	{}

CSampling::CSampling(std::string name, int portID, int numSocket)
: m_numSocket(numSocket),m_name(name), m_portId(portID)
	{
	 std::cout<<"name is "<<m_name<<std::endl;
	 std::cout<<"using port "<<portID<<std::endl;
	 std::cout<<" socket num : "<<m_numSocket<<std::endl;
	}

int CSampling::READ_SAMPLING_MESSAGE(int sock)
	{
	struct sockaddr_in c_a;
	int lc_a; //longueur structure
	fd_set readfds;
	
	Type_Message rMessage;
	int ret = 0; // test variable for the function's' return
	int le_t = 0; //retunr code for recvfrom
	struct timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 1000;

	FD_ZERO(&readfds);
	FD_SET(sock, &readfds);
	
	int last_message_in_queue =0;
	rMessage=myReceivedMessage;
	
	//std::cout<<"<Sampling/R> Trying to read at socket " << sock << "..."<<std::endl;
	
	while (last_message_in_queue != 1)
		{
		
		
		if((ret = select(getdtablesize(), &readfds, NULL, NULL, &timeout)) == -1)
			{
			std::cout << "<Sampling/R> Problem with select." <<std::endl;
			perror("select()");
			return (-1);
			}
		else
			{
			if(ret != 0)
				{
				//std::cout<<"<Sampling/R> Detection of something at socket " << sock << "..."<<std::endl;
				lc_a = sizeof(c_a);
				if((le_t = recvfrom(sock,&rMessage,sizeof(struct Type_Message),0,(struct sockaddr *)&c_a, (socklen_t *)&lc_a))==-1)
					{
					std::cout << "<Sampling/R> Cannot read message at socket " << sock << ". (recvfrom)" <<std::endl;
					perror("recvfrom");
					close(sock);
					return (le_t);//return code are the same as recvfrom : -1 if error, or 0 if the sender has made an ordernly shutdown
					}
				
				last_message_in_queue = 0;
				}
			else
				{
				
				last_message_in_queue = 1;
				myReceivedMessage=rMessage;
				//std::cout<<"<Sampling/R> Message received."<<std::endl;	
				return (le_t);// return the size of the recived message
				}
			}
		}
	return 0;
	}



void CSampling::Display_Message()
	{
	std::cout<<"			"<<std::endl;
	std::cout<<"message from : "<<myReceivedMessage.m_sender<<" length : "<<myReceivedMessage.m_length<<std::endl;
	std::cout<<"receive :"<<myReceivedMessage.m_message<<std::endl;
	std::cout<<"			"<<std::endl;
	}

Type_Message  CSampling::get_Message(){
	return (myReceivedMessage);
	}

							
int CSampling::WRITE_SAMPLING_MESSAGE(char *name, int portId, int sock,std::string emetteur, std::string Dgram)
	{
	const char *str1 = Dgram.c_str(); //convert string to const char
	strcpy(myMessage.m_message,str1);
	myMessage.m_length = sizeof(myMessage.m_message);
	const char *str2 = emetteur.c_str();
	strcpy(myMessage.m_sender,str2);

	struct hostent *s_h;
	if ((s_h = gethostbyname(name)) == NULL)
		{
		std::cout << "<Sampling/W> Problem with gethostbyname." <<std::endl;
		perror("gethostbyname");
		return(-1);
		}
		
	struct sockaddr_in s_a;	
		bzero((char *) &s_a, sizeof(s_a));
		bcopy(s_h->h_addr, (char *) &s_a.sin_addr, s_h->h_length);
		s_a.sin_family = htonl(s_h->h_addrtype);
		s_a.sin_port = htons(portId);	


	int nbeff;	
	if((nbeff = sendto(sock,&myMessage,sizeof(struct Type_Message),0,(struct sockaddr *)&s_a, sizeof(s_a)))==-1)
		{
		std::cout << "<Sampling/W> Cannot send message on port " << portId << " with socket " << sock << ". (sendto)" <<std::endl;
		perror("sendto");	
		close(sock);
		return(-1);
		}
	
	//std::cout<<"<Sampling/W> Message sended."<<std::endl;
	return(0);
	}
