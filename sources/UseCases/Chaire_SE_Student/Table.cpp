#include "Table.h"

Table::Table(){

	//cout << "<Table> Construction Table." << endl;

	sprintf(plan_courant,"OldPlan.txt");
	etat_courant_table = -1;
	/*Checkpoint checkpoint_vide;
	for(int i=0; i<75;i++){
		liste_checkpoints[i]=checkpoint_vide;
	}*/

}

void Table::afficher(){

	cout << endl << "<TABLE> Reading of the table ---------- " << endl;
	
	if ( etat_courant_table == -1) cout << "Table empty" << endl;
	else {
	
		for(int cpt = 0;cpt <= etat_courant_table; cpt++){
			cout << "# Chk " << cpt+1 << ">\t N° action: "<< liste_checkpoints[cpt].get_id_action() <<
			" - N° Image : " << liste_checkpoints[cpt].get_id_image_sauvegarde() << "\t States: " <<
			"P:" << liste_checkpoints[cpt].get_etat(0) << " A:" << liste_checkpoints[cpt].get_etat(1)
			<< " V:" << liste_checkpoints[cpt].get_etat(2) << " S:" << liste_checkpoints[cpt].get_etat(3)
			<< " T:" << liste_checkpoints[cpt].get_etat(4) << endl;
		}
	}
	
	cout << "------------------------------------------ " << endl;
}

int Table::maj(Checkpoint checkpoint){

	int resultat = 1;
	bool detecte = false;
	
	int cpt = 0;
	
	while(cpt<TAILLE_TABLE && detecte == false){

		if (checkpoint.get_id_image_sauvegarde() == liste_checkpoints[cpt].get_id_image_sauvegarde()){
	
			detecte = true;
			liste_checkpoints[cpt].set_tous_etats(checkpoint);
			liste_checkpoints[cpt].set_taille_image(checkpoint);
			liste_checkpoints[cpt].set_pointeur_mem(checkpoint);
			//cout << "<Table> Image " << checkpoint.get_id_image_sauvegarde() << " trouvée à la ligne " << cpt << "." << endl;
		}
		
		cpt++;
	}
	
	//if (checkpoint.get_id_image_sauvegarde() > liste_checkpoints[cpt].get_id_image_sauvegarde()) {
	if (detecte != true){
	
		//cout << "<Table> Coucou 2 ... num_act : " << checkpoint.get_numero_action() << " taille : " << liste_checkpoints.size() << endl;
		
		num_action_dernier_checkpoint = checkpoint.get_id_action();
		etat_courant_table++;
		if(etat_courant_table == TAILLE_TABLE){
			etat_courant_table = 0;
		}
		liste_checkpoints[etat_courant_table] = checkpoint;
		
		//cout << "<Table> Image " << checkpoint.get_id_image_sauvegarde() << " non trouvée. Création à la ligne " << etat_courant_table << "." << endl;
		
		//cout << "<Table> Coucou 2 !!!" << endl;
	
	}
	
	// TODO Faire message d'erreur. Mais dans quel cas ??
	/*if (checkpoint.get_id_action() < 0) {
	
		cout << "<Table> Erreur : problème de numéro de checkpoint : " << checkpoint.get_id_action() << " "
		<< etat_courant_table << endl;

		resultat = 0;
	
	}*/

	
	return resultat;
	
}

int Table::maj(int id_image, int num_etat, bool val_etat){

	//cout << endl << "<Table> Utilisation de la deuxième méthode maj(int,int,bool)." << endl << endl;

	int resultat = 1;
	Checkpoint checkpoint;
	
	bool detecte = false;
	int cpt = 0;
	
	while(cpt<TAILLE_TABLE && detecte == false){

		if (id_image == liste_checkpoints[cpt].get_id_image_sauvegarde()){
		
			detecte = true;
	
			liste_checkpoints[cpt].set_etat(id_image,num_etat,val_etat);
	
		}
		
		cpt++;
		
	}
	
	if ( detecte != true ) {
	
		cout << "<Table> Checkpoint Problem !" << endl;
		
		resultat = -1;
	
	}
	
	
	/*for(int cpt=0;cpt<=etat_courant_table;cpt++){

		if (id_image == liste_checkpoints[cpt].get_id_image_sauvegarde()){
	
		     liste_checkpoints[cpt].set_etat(id_image,num_etat,val_etat);
	
			//cout << "<Table> Coucou 1 ... num_act : " << checkpoint.get_numero_action() << " taille : " << liste_checkpoints.size() << endl;
			
			//cout << "<Table> Coucou 1 !!!" << endl;
	
		} else if (id_image > liste_checkpoints[cpt].get_id_image_sauvegarde()) {
	
			//cout << "<Table> Coucou 2 ... num_act : " << checkpoint.get_numero_action() << " taille : " << liste_checkpoints.size() << endl;
			checkpoint.set_checkpoint(0,id_image);
			for(int i=0;i<num_etat;i++){
			   checkpoint.set_etat(i,true);
			
			}
			checkpoint.set_etat(num_etat,val_etat);
			etat_courant_table++;
			if(etat_courant_table==76){
				etat_courant_table=0;
			}
			liste_checkpoints[etat_courant_table]=checkpoint; 
		
			//cout << "<Table> Coucou 2 !!!" << endl;
	
		} else {
	
			cout << "<Table> Erreur : problème de numéro de checkpoint : " << checkpoint.get_id_action() << " " << etat_courant_table << endl;
		
			resultat = 0;
	
		}

	}*/
	
	
	//afficher();
	
	return resultat;
}

/*
bool Table::supprimer_checkpoint(int id_image_sauvegarde_a_supprimer){
       int i=0;
       bool found=false;
	while((i<50)&&(found=false)){
		if(liste_checkpoints[i].get_id_image_sauvegarde()==id_image_a_supprimer){
			for(int j=i; j<etat_courant_table;j++){
				liste_checkpoints[j]=liste_checkpoints[j+1];
					
			}
			etat_courant_table=etat_courant_table-1;
			found=true;
		}
		i=i+1;	
	}
	
	if(found=false){
		cout<<"Erreur lors de l'effacement du checkpoint"<<endl;
	//TODO gestion  des erreurs	
		return true;
	}
	
	else{
		return false; 
	}
}*/

/*
void Table::vider(){

	int numero_plan;
	Checkpoint checkpoint_vide;

	sscanf(plan_courant,"%*s%d",&numero_plan);
	
	numero_plan++;

	// Actualisation du nom du nouveau plan
	sprintf(plan_courant,"instancePlan%d.txt",numero_plan);
	
	cout << "<Table> Vidage de la table." << endl;

	liste_checkpoints.clear();
	
	for(int i=0;i<=etat_courant_table;i++){
	liste_checkpoints[i]=checkpoint_vide;
	}
	
	num_action_dernier_checkpoint = 0;
	etat_courant_table=0;

	
}*/


int Table::get_num_check(){

	return num_action_dernier_checkpoint;
}

Checkpoint Table::get_dernier_check(){

	return liste_checkpoints[etat_courant_table];
}

int Table::id_image_to_position(int id_image){

	int cpt=0;
	int flag=-1;

	//cout << "<Table> Recherche de l'image " << id_image << " dans la table." << endl;

	while(cpt<TAILLE_TABLE && flag==-1){

		//cout << "<Table> Ligne n° " << cpt << "..." << endl;

		if ( id_image == liste_checkpoints[cpt].get_id_image_sauvegarde()){
		flag=cpt;
		//cout << "<Table> Image " << id_image << " existante à la ligne " << cpt << "." << endl;
		}
		
		cpt++;
	
	}
	
	return(flag);
}

	
int Table::position_to_id_image (int position){
	return(liste_checkpoints[position].get_id_image_sauvegarde());
}


Checkpoint Table::get_check(int position){
	return(liste_checkpoints[position]);
}

