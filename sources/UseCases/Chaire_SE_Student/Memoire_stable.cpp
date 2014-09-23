#include "Memoire_stable.h"

Memoire_stable::Memoire_stable(){

	//cout << "<Memoire stable> Construction Memoire Stable." << endl;
	
}

int Memoire_stable::stocker( image stock ){ 
	
	char commande[500]="\0";
	int status = 1;
	
	sprintf(commande,"cp camera_memory/%s.zip stable_memory/", stock);
        //cout << "<Memoire stable> Commande stockage : " << commande << endl;
	system(commande);

	return status;

}



void Memoire_stable::vider( image suppr ){
  
	char destination[250]="\0";
	
	sprintf(destination,"stable_memory/%s.zip",suppr);

	char commande[500]="\0";
	
	strcat(commande,"rm -f ");
	strcat(commande, destination);
	
	cout << "<Memoire stable> Image " << suppr << ".zip deleted." << endl;
	
	//cout << "<Memoire stable> Commande vidage : " << commande << endl;

	system(commande);

}

void Memoire_stable::purger(){

	char destination[250];
	
	sprintf(destination,"stable_memory/*");

	char commande[500]="\0";
	
	strcat(commande,"rm -f ");
	strcat(commande, destination);
	
	//cout << "<Memoire stable> Commande purge : " << commande << endl;

	system(commande);
  	

}
