#include "CQueuing.h"
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>

int SEND_QUEUING_MESSAGE(char *name, int portId, int sock, char *emetteur, char *message) {

    const char *str1 = message; //convert char to const char
    Type_Message myMessage;
    strcpy(myMessage.m_message, str1);
    myMessage.m_length = sizeof (myMessage.m_message);
    const char *str2 = emetteur;
    strcpy(myMessage.m_sender, str2);

    struct hostent *s_h;
    if ((s_h = gethostbyname(name)) == NULL) {
        printf("<Queuing/W> Problem with gethostbyname.\n");
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
        printf("<Queuing/W> Cannot send message on port %d with socket %d. (sendto)\n", portId, sock);
        perror("sendto");
        close(sock);
        return (-1);
    }

    return (0);
}

int RECEIVE_QUEUING_MESSAGE(int sock, Type_Message *rMessage) {
    struct sockaddr_in c_a;
    int lc_a; //longueur structure
    fd_set readfds;

    int ret = 0;
    int le_t = 0;
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 1000;

    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);

    printf("<Queuing/R> c_a before = %d", c_a.sin_port); ////////////////////////////////////////////////////

    if ((ret = select(getdtablesize(), &readfds, NULL, NULL, &timeout)) == -1) {
        printf("<Queuing/R> Problem with select.\n");
        perror("select()");
        return (-1);
    } else {
        if (ret != 0) {
            //std::cout<<"<Queuing/R> Detection of something at socket " << sock << "..."<<std::endl;
            lc_a = sizeof (c_a);
            if ((le_t = recvfrom(sock, rMessage, sizeof (Type_Message), 0, (struct sockaddr *) &c_a, (socklen_t *) & lc_a)) == -1) {

                printf("<Queuing/R> Cannot read message at socket %d. (recvfrom)\n", sock);
                perror("recvfrom");
                close(sock);
                return (le_t); // return code are the same as recvfrom : -1 if error, or 0 if the sender has made an ordernly shutdown:  to do : implement the erno code or verify that it is correct
            }
            printf("<Queuing/R> c_a after = %d", c_a.sin_port);
            fflush(stdout);
            //         myReceivedMessage = *rMessage;
            //std::cout<<"<Queuing/R> Message received."<<std::endl;
            return (le_t); // return the size of the recived message
        } else {
            printf("<Queuing/R> c_a toto = %d", c_a.sin_port);
            //std::cout<<"<Queuing/R> No detection at socket " << sock << "."<<std::endl;	
            return (0); //no detection
        }
    }
}