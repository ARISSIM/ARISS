#include "Plan.h"

Plan::Plan() {

    //cout << "<Plan> Construction." << endl;

    id_action_courante = -1;
    taille_plan = 0;
    sprintf(plan_courant, "vide");

    //set_plan();

}

int Plan::set_plan_recouvrement() {

    int nb_action_enregistree = 0;
    action action_temp;
    int flag = 1; // -1 for error, 1 for success

    ifstream fichier_plan;

    // OPENING NEW PLAN

    char chemin[256] = "\0";

    sprintf(chemin, "../../sources/Chaire_SE_Student/plans/OldPlan.txt");
    fichier_plan.open(chemin, ios::in);

    if (!fichier_plan) {

        fichier_plan.close();
        cout << "<Plan> No OldPlan.txt file." << endl;

        sprintf(chemin, "../../sources/Chaire_SE_Student/plans/DefaultPlan.txt");
        fichier_plan.open(chemin, ios::in);

        if (!fichier_plan) {
            fichier_plan.close();
            cout << "<Plan> No DefaultPlan.txt file." << endl;
            cout << "<Plan> No plan to load.. ERROR !" << endl;
            return -1;
        } else {

            sprintf(plan_courant, "DefaultPlan.txt");
        }
    } else {
        sprintf(plan_courant, "OldPlan.txt");
    }

    cout << "<Plan> Loading actions from file " << plan_courant << "." << endl;

    // READING PLAN

    taille_plan = 0;
    id_action_courante = -1;

    cout << "<Plan> Loading actions from file..." << endl;

    while ((fichier_plan.eof() != true) && (taille_plan < 50)) {

        fichier_plan >> action_temp.numero_action >> action_temp.nature;

        if (!strcmp(action_temp.nature.c_str(), "IMG")) {

            fichier_plan >> action_temp.numero_image >> action_temp.date_action.tm_mday >>
                    action_temp.date_action.tm_mon >> action_temp.date_action.tm_year >>
                    action_temp.date_action.tm_hour >> action_temp.date_action.tm_min >> action_temp.date_action.tm_sec
                    >> action_temp.duree_action >> action_temp.angle_prise_vue[0] >>
                    action_temp.angle_prise_vue[1] >> action_temp.angle_prise_vue[2];

            // Months and years conversion
            action_temp.date_action.tm_year -= 1900;
            action_temp.date_action.tm_mon -= 1;
        } else if (!strcmp(action_temp.nature.c_str(), "TSF")) {

            fichier_plan >> action_temp.date_action.tm_mday >> action_temp.date_action.tm_mon
                    >> action_temp.date_action.tm_year >> action_temp.date_action.tm_hour >>
                    action_temp.date_action.tm_min >> action_temp.date_action.tm_sec >> action_temp.duree_action
                    >> action_temp.nbre_images_a_envoyer;

            for (int i = 0; i < action_temp.nbre_images_a_envoyer; i++) {
                fichier_plan >> action_temp.id_images_a_envoyer[i];
            }

            // Months and years conversion
            action_temp.date_action.tm_year -= 1900;
            action_temp.date_action.tm_mon -= 1;

        }


        liste_actions[nb_action_enregistree] = action_temp;
        nb_action_enregistree++;

        taille_plan++;

    }

    cout << "<Plan> Plan loaded. Plan length : " << taille_plan << "." << endl;

    fichier_plan.close();



    return flag;


}

int Plan::set_plan(int type) {


    int nb_action_enregistree = 0;
    action action_temp;

    ifstream fichier_plan;

    char chemin[256] = "\0";

    if (type == 1) {
        sprintf(chemin, "../../sources/Chaire_SE_Student/plans/NewPlan.txt");
        sprintf(plan_courant, "NewPlan.txt");
    } else if (type == 0) {
        sprintf(chemin, "../../sources/Chaire_SE_Student/plans/DefaultPlan.txt");
        sprintf(plan_courant, "DefaultPlan.txt");
    }
    // LECTURE DU PLAN

    taille_plan = 0;
    id_action_courante = -1;

    fichier_plan.open(chemin, ios::in);

    if (!fichier_plan) {

        cout << "<Plan> ERROR while opening " << plan_courant << "." << endl;

    } else {

        cout << "<Plan> Loading actions from file " << plan_courant << " ..." << endl;

        while ((fichier_plan.eof() != true) && (taille_plan < 50)) {

            fichier_plan >> action_temp.numero_action >> action_temp.nature;

            if (!strcmp(action_temp.nature.c_str(), "IMG")) {

                fichier_plan >> action_temp.numero_image >> action_temp.date_action.tm_mday >>
                        action_temp.date_action.tm_mon >> action_temp.date_action.tm_year >>
                        action_temp.date_action.tm_hour >> action_temp.date_action.tm_min >> action_temp.date_action.tm_sec
                        >> action_temp.duree_action >> action_temp.angle_prise_vue[0] >>
                        action_temp.angle_prise_vue[1] >> action_temp.angle_prise_vue[2];

                // Conversion des mois et des années
                action_temp.date_action.tm_year -= 1900;
                action_temp.date_action.tm_mon -= 1;
            } else if (!strcmp(action_temp.nature.c_str(), "TSF")) {

                fichier_plan >> action_temp.date_action.tm_mday >> action_temp.date_action.tm_mon
                        >> action_temp.date_action.tm_year >> action_temp.date_action.tm_hour >>
                        action_temp.date_action.tm_min >> action_temp.date_action.tm_sec >> action_temp.duree_action
                        >> action_temp.nbre_images_a_envoyer;

                for (int i = 0; i < action_temp.nbre_images_a_envoyer; i++) {
                    fichier_plan >> action_temp.id_images_a_envoyer[i];
                }

                // Conversion des mois et des années
                action_temp.date_action.tm_year -= 1900;
                action_temp.date_action.tm_mon -= 1;

            }


            liste_actions[nb_action_enregistree] = action_temp;
            nb_action_enregistree++;

            taille_plan++;

        }

        cout << "<Plan> Plan loaded. Plan length : " << taille_plan << "." << endl;

        fichier_plan.close();


        if (type == 1) {

            //suppression ancien plan
            sprintf(chemin, "rm -f ../../sources/Chaire_SE_Student/plans/OldPlan.txt");
            system(chemin);

            // Renommation de "NewPlan.txt" en "OldPlan.txt"
            sprintf(chemin, "mv -f ../../sources/Chaire_SE_Student/plans/NewPlan.txt ../../sources/Chaire_SE_Student/plans/OldPlan.txt");
            system(chemin);

            // MAJ du string contenant le nom du fichier plan utilisé
            sprintf(plan_courant, "OldPlan.txt");
        }

    }

    return 0;

}

action Plan::next_action() {

    id_action_courante++;
    return liste_actions[id_action_courante];



}

int Plan::get_taille() {

    //return liste_actions.size();
    return taille_plan;



}

void Plan::set_taille(int nouvelle_taille) {


    taille_plan = nouvelle_taille;

}

int Plan::get_id_action_courante() {

    return id_action_courante;

}

action Plan::get_action(int numero) {

    int action_id = 0;

    if (numero > (taille_plan - 1)) action_id = taille_plan - 1;
    else action_id = numero;

    return liste_actions[action_id];
}

void Plan::set_id_action_courante(int numero) {
    id_action_courante = numero;
}

void Plan::set_plan_courant(char* nouveau_fichier) {

    strncpy(plan_courant, nouveau_fichier, 30);

}

char* Plan::get_plan_courant() {

    return plan_courant;

}

void Plan::set_action(action nouvelle_action, int rang) {

    liste_actions[rang].numero_action = nouvelle_action.numero_action;
    liste_actions[rang].nature = nouvelle_action.nature;
    liste_actions[rang].date_action = nouvelle_action.date_action;
    liste_actions[rang].numero_image = nouvelle_action.numero_image;
    liste_actions[rang].duree_action = nouvelle_action.duree_action;
    for (int i = 0; i < 3; i++) liste_actions[rang].angle_prise_vue[i] = nouvelle_action.angle_prise_vue[i];
    liste_actions[rang].nbre_images_a_envoyer = nouvelle_action.nbre_images_a_envoyer;
    for (int i = 0; i < 10; i++) liste_actions[rang].id_images_a_envoyer[i] = nouvelle_action.id_images_a_envoyer[i];

}

void Plan::set_liste_actions(action* nouvelle_liste) {

    for (int i = 0; i < 50; i++) set_action(nouvelle_liste[i], i);
}

action* Plan::get_liste_actions() {

    return liste_actions;
}

void Plan::afficher_action(action action_a_afficher) {

    cout << "N°Act: " << action_a_afficher.numero_action << "\t " << action_a_afficher.nature << "\t " <<
            asctime(&(action_a_afficher.date_action));

    if (!strcmp(action_a_afficher.nature.c_str(), "IMG")) {
        cout << "N°Im: " << action_a_afficher.numero_image << "\t " << action_a_afficher.duree_action << "s \t " <<
                "SCAO: " << action_a_afficher.angle_prise_vue[0] << "/" << action_a_afficher.angle_prise_vue[1] << "/" <<
                action_a_afficher.angle_prise_vue[2] << endl;
    } else if (!strcmp(action_a_afficher.nature.c_str(), "TSF")) {
        cout << action_a_afficher.nbre_images_a_envoyer << " images";
        for (int i = 0; i < action_a_afficher.nbre_images_a_envoyer; i++) cout << " " << action_a_afficher.id_images_a_envoyer[i];
        cout << endl;
    }

}
