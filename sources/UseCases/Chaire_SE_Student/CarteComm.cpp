#include "CarteComm.h"


CarteComm::CarteComm(){
	
	//cout << "<CarteComm> Construction Carte Communication." << endl;
	active = false;
	
}


bool CarteComm::activer_carte(){
	
	active = true;
	
	cout << "<CarteComm> Communication card activated." << endl;
	
	return true;

}

bool CarteComm::envoyer(char* id_image_a_envoyer, char* machine_name){

	char commande_envoi[250] = "\0";
	
	//cout << "<CarteComm> Préparation de la commande d'envoi." << endl;
	
	if (!strcmp(machine_name,"raspberryPI-TSF")){

		sprintf(commande_envoi,"scp stable_memory/%s.zip", id_image_a_envoyer);

		strcat(commande_envoi, " chaire_n7@grenache:/home/chaire_n7/Desktop/Station_Sol");

	} else { // Compatibility with PC
		
		// "Creation" of the ground station (folder "ground_station")
		system("mkdir ground_station");
		
		// Move the comrpessed image to ground_station
		sprintf(commande_envoi, "cp stable_memory/%s.zip ground_station/", 
		id_image_a_envoyer);
	
	}
	
	cout << "<CarteComm> Commande envoi : " << commande_envoi << endl;

	system(commande_envoi);
	
	cout << "<CarteComm> Image " << id_image_a_envoyer << " sent to ground station." << endl;

	return true;
}

bool CarteComm::desactiver_carte(){

	active = false;
	
	cout << "<CarteComm> Communication card desactivated.." << endl;
	
	return true;
}


bool CarteComm::get_etat(){

	return active;
}


