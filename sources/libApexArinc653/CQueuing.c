#include "CQueuing.h"
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
/*typedef int  int QUEUING_PORT_ID_TYPE;
typedef int  int SYSTEM_TIME_TYPE;
typedef void void* MESSAGE_ADDR_TYPE;
typedef int  int MESSAGE_SIZE_TYPE;
typedef int  int RETURN_CODE_TYPE;*/


/**
 * prototype de fonction un peu plus comforme au standard ARINC 653
 **/
/**
 * 
 * @param QUEUING_PORT_ID
 * @param MESSAGE_ADDR
 * @param LENGTH
 * @param TIME_OUT
 * @param RETURN_CODE
 */
/*void SEND_QUEUING_MESSAGE(int QUEUING_PORT_ID,void *MESSAGE_ADDR, int LENGTH,long TIME_OUT,int *RETURN_CODE){
    struct hostent *s_h;
    if ((s_h = gethostbyname(name)) == NULL) {
        perror("gethostbyname :");
        return (-1);
    }

    struct sockaddr_in s_a;
    bzero((char *) &s_a, sizeof (s_a));
    bcopy(s_h->h_addr, (char *) &s_a.sin_addr, s_h->h_length);
    s_a.sin_family = htonl(s_h->h_addrtype);
    s_a.sin_port = htons(portId);

    int nbeff;
    if ((nbeff = sendto(sock, *MESSAGE_ADDR, LENGTH, 0, (struct sockaddr *) &s_a, sizeof (s_a))) == -1) {
        perror("sendto");
        close(sock);
        return (-1);
    }
}*/
/**
 * 
 * @param QUEUING_PORT_ID
 * @param TIME_OUT
 * @param MESSAGE_ADDR
 * @param LENGTH
 * @param RETURN_CODE
 * @return 
 */
/*int RECEIVE_QUEUING_MESSAGE(QUEUING_PORT_ID_TYPE QUEUING_PORT_ID, SYSTEM_TIME_TYPE TIME_OUT, MESSAGE_ADDR_TYPE MESSAGE_ADDR, MESSAGE_SIZE_TYPE LENGTH, RETURN_CODE_TYPE RETURN_CODE) {

}*/
/**
 * 
 * @param name
 * @param portId
 * @param sock
 * @param emetteur
 * @param message
 * @return 
 */
int SEND_QUEUING_MESSAGE(char *name, int portId, int sock, char *emetteur, char *message) {

    const char *str1 = message; //convert char to const char
    Type_Message myMessage;
    strcpy(myMessage.m_message, str1);
    myMessage.m_length = sizeof (myMessage.m_message);
    const char *str2 = emetteur;
    strcpy(myMessage.m_sender, str2);

    struct hostent *s_h;
    if ((s_h = gethostbyname(name)) == NULL) {
        perror("gethostbyname :");
        return (-1);
    }

    struct sockaddr_in s_a;
    bzero((char *) &s_a, sizeof (s_a));
    bcopy(s_h->h_addr, (char *) &s_a.sin_addr, s_h->h_length);
    s_a.sin_family = htonl(s_h->h_addrtype);
    s_a.sin_port = htons(portId);

    int nbeff;
    if ((nbeff = sendto(sock, &myMessage, sizeof (Type_Message), 0, (struct sockaddr *) &s_a, sizeof (s_a))) == -1) {
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
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);

    if ((ret = select(getdtablesize(), &readfds, NULL, NULL, &timeout)) == -1) {
        perror("select()");
        return (-1);
    } else {

        if (ret != 0) {
            lc_a = sizeof (c_a);
            if ((le_t = recvfrom(sock, rMessage, sizeof (Type_Message), 0, (struct sockaddr *) &c_a, (socklen_t *) & lc_a)) == -1) {
                perror("recvfrom");
                close(sock);
                return (le_t); // return code are the same as recvfrom : -1 if error, or 0 if the sender has made an ordernly shutdown:  to do : implement the erno code or verify that it is correct
            }
            return (le_t); // return the size of the recived message
        } else {
            return (0); //no detection
        }
    }
}
