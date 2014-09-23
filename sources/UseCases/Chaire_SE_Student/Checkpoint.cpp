#include "Checkpoint.h"

Checkpoint::Checkpoint() {

    //cout << "<Checkpoint> Construction Checkpoint." << endl;

    for (int i = 0; i <= 5; i++) {
        etat[i] = 0;
    }
    id_action = 0;
    taille_image = 0;
    // pointeur_mem = 0;

}

/*int Checkpoint::comparer(Checkpoint chk){

        int resultat = 0;

        if (this->numero_action == chk.get_numero_action() && this->etat == chk.get_etat()){
                resultat = 1;
        }
	
        return resultat;
}*/

// accesseurs

int Checkpoint::get_id_action() {

    return id_action;

}

bool Checkpoint::get_etat(int i) {
    return etat[i];

}

int Checkpoint::get_id_image_sauvegarde() {

    return id_image_sauvegarde;
}

//mutateurs

void Checkpoint::set_checkpoint(int id_action_prog, int id_image) {
    id_image_sauvegarde = id_image; /*!<connaitre l'id de la sauvegarde de l'image*/
    id_action = id_action_prog;

}

void Checkpoint::set_etat(int numero_etat, bool v_etat) {

    etat[numero_etat] = v_etat;
}

void Checkpoint::set_etat(int id_image, int numero_etat, bool v_etat) {

    id_image_sauvegarde = id_image;
    etat[numero_etat] = v_etat;
}

void Checkpoint::set_tous_etats(Checkpoint new_checkpoint) {
    for (int i = 0; i < 5; i++) {
        etat[i] = new_checkpoint.get_etat(i);
    }


}

void Checkpoint::set_tous_etats(bool new_table_etats[5]) {

    for (int i = 0; i < 5; i++) etat[i] = new_table_etats[i];

}

void Checkpoint::set_taille_image(int taille) {

    taille_image = taille;

}

void Checkpoint::set_taille_image(Checkpoint check) {

    taille_image = check.get_taille_image();

}

void Checkpoint::set_pointeur_mem(int pointeur) {

    pointeur_mem = pointeur;

}

void Checkpoint::set_pointeur_mem(Checkpoint check) {

    pointeur_mem = check.get_pointeur_mem();

}

int Checkpoint::get_taille_image() {

    return taille_image;

}

int Checkpoint::get_pointeur_mem() {

    return pointeur_mem;

}



