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
#include "include/CQueuing.h"

CQueuing::CQueuing()
	{
	}

CQueuing::CQueuing(std::string name, int portID, int numSocket)
: m_numSocket(numSocket),m_name(name), m_portId(portID)
	{
	 std::cout<<"name is "<<m_name<<std::endl;
	 std::cout<<"using port "<<portID<<std::endl;
	 std::cout<<" socket num : "<<m_numSocket<<std::endl;
	}


void CQueuing::Trace_Message(Type_Message *msg) // test function, not necessary in production
	{
	std::cout<<"			"<<std::endl;
	std::cout<<"Trace message :"<<msg->m_message<<std::endl;
	std::cout<<"			"<<std::endl;
	}
	


int CQueuing::READ_QUEUING_MESSAGE(int sock)
	{
	struct sockaddr_in c_a;
	int lc_a; //longueur structure
	fd_set readfds;
	
	Type_Message rMessage;
	int ret = 0 ;
	int le_t = 0;
	struct timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 1000;
	
	FD_ZERO(&readfds);
	FD_SET(sock, &readfds);
	
	if((ret = select(getdtablesize(), &readfds, NULL, NULL, &timeout)) == -1)
		{
		std::cout << "<Queuing/R> Problem with select." <<std::endl;
		perror("select()"); 
		return(-1);
		}
	else
		{
		if(ret != 0)
			{
			//std::cout<<"<Queuing/R> Detection of something at socket " << sock << "..."<<std::endl;
			lc_a = sizeof(c_a);	
			if((le_t = recvfrom(sock,&rMessage,sizeof(struct Type_Message),0,(struct sockaddr *)&c_a, (socklen_t *)&lc_a))==-1)
				{
				std::cout << "<Queuing/R> Cannot read message at socket " << sock << ". (recvfrom)" <<std::endl;
				perror("recvfrom");
				close(sock);
				return (le_t); // return code are the same as recvfrom : -1 if error, or 0 if the sender has made an ordernly shutdown:  to do : implement the erno code or verify that it is correct
				}			
			myReceivedMessage=rMessage;
			//std::cout<<"<Queuing/R> Message received."<<std::endl;
			return(le_t); // return the size of the recived message
			}
		else
			{
			//std::cout<<"<Queuing/R> No detection at socket " << sock << "."<<std::endl;	
			return(0); //no detection
			}
		}
	}

void CQueuing::Display_Message()
	{
	std::cout<<"			"<<std::endl;
	std::cout<<"Display message : "<<myReceivedMessage.m_message<<std::endl;
	std::cout<<"length "<<myReceivedMessage.m_length<<std::endl;
	std::cout<<"total length "<<sizeof(myReceivedMessage)<<std::endl;
	std::cout<<"receive :"<<myReceivedMessage.m_message<<std::endl;
	std::cout<<"			"<<std::endl;
	}
	
Type_Message CQueuing::get_Message(){
	return (myReceivedMessage);
	}


int CQueuing::WRITE_QUEUING_MESSAGE(char *name, int portId, int sock,std::string emetteur, std::string Dgram)
	{
	const char *str1 = Dgram.c_str(); //convert string to const char
	strcpy(myMessage.m_message,str1);
	myMessage.m_length = sizeof(myMessage.m_message);
	const char *str2 = emetteur.c_str();
	strcpy(myMessage.m_sender,str2);
	int nbeff;

	struct hostent *s_h;
	if ((s_h = gethostbyname(name)) == NULL)
		{
		std::cout << "<Queuing/W> Problem with gethostbyname." <<std::endl;
		perror("gethostbyname");
		return(-1);
		}

	struct sockaddr_in s_a;	
		bzero((char *) &s_a, sizeof(s_a));
		bcopy(s_h->h_addr, (char *) &s_a.sin_addr, s_h->h_length);
		s_a.sin_family = htonl(s_h->h_addrtype);
		s_a.sin_port = htons(portId);
//		s_a.sin_port = ntohs(portId);	
	
	if((nbeff = sendto(sock,&myMessage,sizeof(struct Type_Message),0,(struct sockaddr *)&s_a,sizeof(s_a)))==-1)
		{
		std::cout << "<Queuing/W> Cannot send message on port " << portId << " with socket " << sock << ". (sendto)" <<std::endl;	
		perror("sendto");	
		close(sock);
		return(-1);
		}
	//std::cout<<" sock : "<<sock <<" &myMessage : "<<&myMessage<<" sizeof(struct Type_Message) : "<<sizeof(struct Type_Message)<<" flag : "<<0<<"  (struct sockaddr *)&s_a: "<<(struct sockaddr *)&s_a<<" sizeof(s_a) : "<< sizeof(s_a)<<std::endl;
	//std::cout<<"<Queuing/W> Message sended."<<std::endl;
	return(0);
	}
