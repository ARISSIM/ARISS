#ifndef CCONFIG_H
#define CCONFIG_H

#include "CPartition.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <vector>



class CConfig
{
public:
	CConfig();
	void read_process(std::vector<CPartition> &vpart,std::string filepath);
	void read_communication(std::vector<CPartition> &vpart, std::string filepath);
	
private :
	void vwords(std::vector<std::string> &vwords, std::string line);

};


#endif // CCONFIG_H
