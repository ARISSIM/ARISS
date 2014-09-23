#include "Analyseur.h"

Analyseur::Analyseur() {

	//cout << "<Analyseur> Construction." << endl;

}

int Analyseur::traiter_image(char* image_traitee){

	int image_number = 0;
	int process_status = 0;

	sscanf(image_traitee,"%d.jpg",&image_number);
	
	if (image_number%10 != 0){
		process_status = 1;
		cout << "<Analyseur> Processing of image " << image_traitee << " correct." << endl;
	}
	else{
		process_status = 0;
		cout << "<Analyseur> Processing of image " << image_traitee << " incorrect !!!" << endl;
	}


	return process_status;

}


int Analyseur::compresser_image(char* image_traitee){
	
	// Nom de l'image compressee (même nom mais avec l'extension .zip)
	int compress_status = 0;
	char cmd[200] = "\0";
	
	
	// Compression
	for (int i=0; i<100; i++) cmd[i] = '\0';
	sprintf(cmd, "zip -jq camera_memory/%s.zip camera_memory/%s", 
	image_traitee, image_traitee);
	system(cmd);
	
	// Suppression de l'image non compressee
	for (int i=0; i<100; i++) cmd[i] = '\0';
	sprintf(cmd, "rm -f camera_memory/%s", image_traitee);
	system(cmd);

	compress_status = 1; //TODO : gérer erreurs
	

	return compress_status;

}

