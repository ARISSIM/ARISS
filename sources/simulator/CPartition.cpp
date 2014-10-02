#include "CPartition.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <vector>

CPartition::CPartition(std::string nameProcess, std::string pathProcess, int time)
: m_nameProcess(nameProcess), m_pathProcess(pathProcess), m_time(time) {
    m_vwQport.clear();
    std::vector<int>(m_vwQport).swap(m_vwQport);
    m_vrQport.clear();
    std::vector<int>(m_vwQport).swap(m_vwQport);
    m_vwSport.clear();
    std::vector<int>(m_vwSport).swap(m_vwSport);
    m_vrSport.clear();
    std::vector<int>(m_vrSport).swap(m_vrSport);
}


/*int CPartition::nbSPort()
{
        return(m_nbSamplingPort);
}*/

/*int CPartition::nbQport()
{
        return(m_nbQueuingPort);
}*/

int CPartition::wSport_add(int wport) {
    for (unsigned int i = 0; i < m_vwSport.size(); i++) {
        if (m_vwSport[i] == wport) {
            return (0);
        }
    }
    m_vwSport.push_back(wport);
    return (1);
}

int CPartition::wQport_add(int wport) {
    for (unsigned int i = 0; i < m_vwQport.size(); i++) {
        if (m_vwQport[i] == wport) {
            return (0);
        }
    }
    m_vwQport.push_back(wport);
    return (1);
}

int CPartition::rSport_add(int rport) {
    for (unsigned int i = 0; i < m_vrSport.size(); i++) {
        if (m_vrSport[i] == rport) {
            return (0);
        }
    }
    m_vrSport.push_back(rport);
    return (1);
}

int CPartition::rQport_add(int rport) {
    for (unsigned int i = 0; i < m_vrQport.size(); i++) {
        if (m_vrQport[i] == rport) {
            return (0);
        }
    }
    m_vrQport.push_back(rport);
    return (1);
}

std::vector<int> CPartition::get_wSport() {
    return (m_vwSport);
}

std::vector<int> CPartition::get_wQport() {
    return (m_vwQport);
}

std::vector<int> CPartition::get_rSport() {
    return (m_vrSport);
}

std::vector<int> CPartition::get_rQport() {
    return (m_vrQport);
}

void CPartition::vSsock_add(int sock) {
    m_vSsock.push_back(sock);
}

std::vector<int> CPartition::get_vSsock() {
    return (m_vSsock);
}

void CPartition::vQsock_add(int sock) {
    m_vQsock.push_back(sock);
}

std::vector<int> CPartition::get_vQsock() {
    return (m_vQsock);
}

void CPartition::Display() {
    std::cout << "                    " << std::endl;
    std::cout << "Partition :" << m_nameProcess << std::endl;
    std::cout << "Path :" << m_pathProcess << std::endl;
    std::cout << "Running time :" << m_time << std::endl;
    std::cout << "********************" << std::endl;
}

std::string CPartition::nameProcess() {
    return (m_nameProcess);
}

std::string CPartition::pathProcess() {
    return (m_pathProcess);
}

int CPartition::time() {
    return (m_time);
}






