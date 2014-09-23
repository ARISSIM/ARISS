#include "CarteRecep.h"

CarteRecep::CarteRecep() {

    //cout << "<CarteRecep> Construction Carte Reception." << endl;
    etat = false;

}

bool CarteRecep::activer_carte() {

    etat = true;

    cout << "<CarteComm> Communication card activated." << endl;

    return true;

}

int CarteRecep::recevoir_plan() {

    int ret;

    const char* chemin = "../../sources/Chaire_SE_Student/plans/NewPlan.txt";

    ifstream fichier_plan;

    /*cout << endl << "<CarteRecep> Verification en cours..." << endl;
    sleep(5);*/

    fichier_plan.open(chemin, ios::in);
    if (fichier_plan) {

        fichier_plan.close();
        cout << "<CarteRecep> New plan received !" << endl;
        ret = 1;
    } else {

        cout << "<CarteRecep> No new plan." << endl;
        ret = 0;
    }

    return ret;
}

bool CarteRecep::desactiver_carte() {

    etat = false;

    cout << "<CarteComm> Communication card desactivated." << endl;

    return true;
}

bool CarteRecep::get_etat() {

    return etat;
}


