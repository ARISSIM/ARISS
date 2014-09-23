
#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>



int main(int argc, char *argv[]){
	
	int temp = 0;
	char ligne[30];
	int delai[10];
	int new_s[10];
	int new_mn[10];
	int new_hr[10];
	int NB_LIGNES;
	int delta=0;
	char nom_fichier[56];
	
	
	time_t rawtime;
	struct tm * timer;

	time(&rawtime);
	timer = localtime ( &rawtime ); // aquisition de la date sous format Y/M/D/H/m/s
	timer->tm_mon = timer->tm_mon+1;
	timer->tm_year = timer->tm_year+1900;
	timer->tm_isdst = -1;
	
	std::cout<< "Délai avant début de la première instruction (en s): " << std::endl;
	std::cin>>delai[0];
	std::cout<< std::endl <<"Nombre d'instructions :" << std::endl;
	std::cin>>NB_LIGNES;
	//std::cout<< std::endl <<"Nom de fichier :" << std::endl;
	//std::cin>>nom_fichier;
	sprintf(nom_fichier, "NewPlan.txt");

		
		/** Ouverture en écriture Plan.txt **/
	    std::ofstream Plan(nom_fichier, std::ios::trunc);
	    if (!Plan) {
		std::cout << "<PlanGen> ERROR : " << nom_fichier << " can't be opened" << std::endl;
	    }




	    for (int k = 1; k < (NB_LIGNES + 1); k++) {

		delai[k % 10] = delai[(k - 1) % 10] + 30 + rand() % 5;

		new_s[k % 10] = (timer->tm_sec + delai[k % 10]) % 60;
		new_mn[k % 10] = (timer->tm_min + (timer->tm_sec + delai[k % 10]) / 60) % 60;
		new_hr[k % 10] = (timer->tm_hour + (timer->tm_min + (timer->tm_sec + delai[k % 10]) / 60) / 60) % 24;


		//Uncomment to add "CHANGE PLAN" ACTION
		/*if (k == 20) {
		    Plan << k << " PLA" << std::endl;
		}*/
		
		// CAPTURE ACTION
		    if ((rand() % 100 < 90) || (k < 10) || (delta < 10)) {
		        sprintf(ligne, " %d%d%d%d%d ", timer->tm_mday, timer->tm_mon, new_hr[k % 10], new_mn[k % 10], new_s[k % 10]);
		        Plan << k << " IMG " << ligne << " " << timer->tm_mday << " " << timer->tm_mon << " " << timer->tm_year << " " << new_hr[k % 10] << " " << new_mn[k % 10] << " " << new_s[k % 10] << " " << rand() % 2 + 2 << "  " << 30 + rand() % 20 << " " << 5 + rand() % 170 << " " << 5 + rand() % 170;
		        if (k != NB_LIGNES) {
		            Plan << std::endl;
		        }
		        delta++;
		    }
		    // TRANSFER ACTION
		    else {
		        delta = 0;
		        temp = rand() % 10;
		        Plan << k << " TSF  " << timer->tm_mday << " " << timer->tm_mon << " " << timer->tm_year << " " << new_hr[k % 10] << " " << new_mn[k % 10] << " " << new_s[k % 10] << " " << temp*7 << " " << temp;

		        for (int i = 1; i < temp + 1; i++) {
		            sprintf(ligne, " %d%d%d%d%d ", timer->tm_mday, timer->tm_mon, new_hr[(k - i) % 10], new_mn[(k - i) % 10], new_s[(k - i) % 10]);
		            Plan << ligne;
		        }
		        if (k != NB_LIGNES) {
		            Plan << std::endl;
		        }
		        
		        //Duration of the Transfert Action : 7s by image
		        delai[k % 10] += temp*7;

		    }


	    }//END FOR

	    Plan.close();
	
	std::cout << std::endl << "Envoi du plan..."  << std::endl;
	
	//Raspberry-PI on LAAS network
	system("scp NewPlan.txt pi@raspberrypi-tsf:/home/pi/Desktop/Code/ARINC653_simulator/sources/Chaire_SE_Student/plans");
	
	//Raspberry PI via AdHoc Wi-Fi
	//system("scp NewPlan.txt pi@192.168.1.1:/home/pi/Desktop/Code/ARINC653_simulator/sources/Chaire_SE_Student/plans");
	
	std::cout << "Plan envoyé." << std::endl;
	return(0);
};
