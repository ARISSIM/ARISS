#ifndef CAMERA_H
#define CAMERA_H 

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "../include/CBasefunction.h"
#include "Horloge.h"

#define MAX 100


typedef char image[MAX];

using namespace std;


class Camera {

protected :
	int nb;	
	image img;
	Horloge* horloge;
	
	// Objets de communications
	COMMUNICATION_VECTOR myCvector;
	CQueuing* Qservice; /* Communication Queuing */
	CSampling* Sservice; /* Communication Sampling */
	

public :
		Camera();

		int capturer(bool*, char*,int,int,int, double, double) ;

		void init_cam(CQueuing*, CSampling*, COMMUNICATION_VECTOR, Horloge*);
};



#endif
