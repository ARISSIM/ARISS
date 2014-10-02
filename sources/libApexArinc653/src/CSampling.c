#include "CSampling.h"
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>

int WRITE_SAMPLING_MESSAGE(char *name, int portId, int sock, char *emetteur, char *message) {
    Type_Message myMessage;
    const char *str1 = message; //convert char to const char
    strcpy(myMessage.m_message, str1);
    myMessage.m_length = sizeof (myMessage.m_message);
    const char *str2 = emetteur;
    strcpy(myMessage.m_sender, str2);

    struct hostent *s_h;
    if ((s_h = gethostbyname(name)) == NULL) {
        printf("<Sampling/W> Problem with gethostbyname.\n");
        perror("gethostbyname");
        return (-1);
    }

    struct sockaddr_in s_a;
    bzero((char *) &s_a, sizeof (s_a));
    bcopy(s_h->h_addr, (char *) &s_a.sin_addr, s_h->h_length);
    s_a.sin_family = htonl(s_h->h_addrtype);
    s_a.sin_port = htons(portId);

    int nbeff;
    if ((nbeff = sendto(sock, &myMessage, sizeof (Type_Message), 0, (struct sockaddr *) &s_a, sizeof (s_a))) == -1) {
        printf("<Sampling/W> Cannot send message on port %d with socket %d. (sendto)\n", portId, sock);
        perror("sendto");
        close(sock);
        return (-1);
    }

    //std::cout<<"<Sampling/W> Message sended."<<std::endl;
    return (0);
}

int READ_SAMPLING_MESSAGE(int sock, Type_Message *rMessage) {

    struct sockaddr_in c_a;
    int lc_a; //longueur structure
    fd_set readfds;

    int ret = 0; // test variable for the function's' return
    int le_t = 0; //retunr code for recvfrom
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 1000;

    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);

    int last_message_in_queue = 0;
    

    //std::cout<<"<Sampling/R> Trying to read at socket " << sock << "..."<<std::endl;

    while (last_message_in_queue != 1) {
        if ((ret = select(getdtablesize(), &readfds, NULL, NULL, &timeout)) == -1) {
            printf("<Sampling/R> Problem with select.\n");
            perror("select()");
            return (-1);
        } else {
            if (ret != 0) {
                //std::cout<<"<Sampling/R> Detection of something at socket " << sock << "..."<<std::endl;
                lc_a = sizeof (c_a);
                if ((le_t = recvfrom(sock, &rMessage, sizeof (Type_Message), 0, (struct sockaddr *) &c_a, (socklen_t *) & lc_a)) == -1) {
                    printf("<Sampling/R> Cannot read message at socket %d. (recvfrom)\n", sock);
                    perror("recvfrom");
                    close(sock);
                   // *rMessage = NULL;
                    return (le_t); //return code are the same as recvfrom : -1 if error, or 0 if the sender has made an ordernly shutdown
                }
                last_message_in_queue = 0;
            } else {
                last_message_in_queue = 1;
                //myReceivedMessage = *rMessage;
                //std::cout<<"<Sampling/R> Message received."<<std::endl;	
                return (le_t); // return the size of the recived message
            }
        }
    }
    return 0;
}

