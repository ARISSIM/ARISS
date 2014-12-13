#include "Cport_service.h"
#include <iostream>
#include <vector>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>

Cport_service::Cport_service() {

}

int Cport_service::CREATE_QUEUING_PORT(int portID, int portName, int maxMessage_size, bool portDirection, int refreshPeriod) {
    int sock;
    struct sockaddr_in sa;

    std::cout << "************************" << std::endl;
    std::cout << "CREATE QUEUING PORT" << std::endl;
    std::cout << "portID is " << portID << std::endl;
    std::cout << "partition : " << portName << std::endl;


    bzero(&sa, sizeof (sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    sa.sin_port = htons(portID);
    //socket creation, non-connected, UDP type (DataGram) blocking
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("create socket");
        close(sock);
    }

    if (bind(sock, (struct sockaddr *) &sa, sizeof (sa)) < 0)
    {
        perror("bind 1");
        close(sock);
        return -1;
    }
    std::cout << "socket created : " << sock << std::endl << std::endl;
    return (sock);
}

int Cport_service::CREATE_SAMPLING_PORT(int portID, int portName, int maxMessage_size, bool portDirection, int refreshPeriod) {
    int sock;
    struct sockaddr_in sa;
    std::cout << "************************" << std::endl;
    std::cout << "CREATE SAMPLING PORT" << std::endl;
    std::cout << "portID is " << portID << std::endl;
    std::cout << "partition : " << portName << std::endl;


    bzero(&sa, sizeof (sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    sa.sin_port = htons(portID);
    //socket creation, non-connected, UDP type (DataGram) blocking
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("create socket");
        close(sock);
    }

    if (bind(sock, (struct sockaddr *) &sa, sizeof (sa)) < 0) {
        perror("bind 1");
        close(sock);
        return -1;
    }
    std::cout << "socket :" << sock << std::endl << std::endl;
    return (sock);
}
