#ifndef SCAO_H
#define SCAO_H 

#include "../include/CBasefunction.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>   
#include <unistd.h>
#include <iostream>
#include "Horloge.h"

using namespace std;

class SCAO{

	private :
	COMMUNICATION_VECTOR myCvector;
	char* myArg;
	int myNbarg;
	Horloge* horloge;

	CQueuing Qservice;
	CSampling Sservice;

	public :
	
	SCAO();
	void init_comm(int, char*, COMMUNICATION_VECTOR,Horloge*);
	int orienter_satellite(float, float, float, double, double);

};

#endif
