#include "Manager.h"

using namespace std;

Manager::Manager() {

    //cout << "<Manager> Construction Manager." << endl;

}

void Manager::set_mode(string smode) {

    mode = smode;

}

void Manager::set_partition(string myPart) {

    strncpy(myPartition, myPart.c_str(), 6);
}

void Manager::send_check(Checkpoint check) {

    size_t size = myCvector.emetteur.size() + 1;
    char * nom_machine = new char[ size ];
    strncpy(nom_machine, myCvector.emetteur.c_str(), size);
    char message[256];
    sprintf(message, "%d %d %d %d %d %d %d", check.get_id_image_sauvegarde(), check.get_id_action(), check.get_etat(0), check.get_etat(1), check.get_etat(2), check.get_etat(3), check.get_etat(4));

    //cout << "From " << myPartition << " : " << "<Manager> Sending checkpoint : " << message << "..." << endl;
    Qservice.WRITE_QUEUING_MESSAGE(nom_machine, myCvector.vqueuing_port[0], myCvector.vqueuing_socket[0],
            myCvector.emetteur, message);

}

void Manager::actualiser_table(Checkpoint nouveau_checkpoint) {
    table.maj(nouveau_checkpoint);
}

int Manager::table_id_image_to_position(int i) {
    return table.id_image_to_position(i);
}

int Manager::table_position_to_id_image(int i) {
    return table.position_to_id_image(i);
}

void Manager::actualiser_table(int id_image, int num_etat, bool val_etat) {
    table.maj(id_image, num_etat, val_etat);
}

Checkpoint Manager::table_get_check(int i) {
    return table.get_check(i);
}

Checkpoint Manager::recuperer_dernier_check() {
    Checkpoint checkpoint_recupere;
    checkpoint_recupere = table.get_dernier_check();

    return checkpoint_recupere;
}

bool Manager::get_new_plan(bool first_launch, bool plan_termine) {
    int id_action_en_cours;
    int retour_reception;
    cout << endl << "--------------------------------------" << endl;

    if (plan_termine == true || first_launch) {

        if (first_launch) first_launch = false;

        cout << "<Manager> Current plan done. Changing plan..." << endl;

        retour_reception = carte_reception->recevoir_plan();
        // 1 si un nouveau plan a été reçu, 0 sinon => lecture du plan par défaut

        plan.set_plan(retour_reception);

        if (retour_reception == 1) {// Si un nouveau plan a été trouvé

            id_action_en_cours = trouver_action(plan.get_id_action_courante());

            if (id_action_en_cours != -1) {//Si la date d'au moins une action n'est pas passée 

                plan_termine = false;
                // Mise à jour du n° de l'action dans le plan
                plan.set_id_action_courante(id_action_en_cours - 1);
                // - 1 car on fait +1 dans la fonction next_action (cf plus bas)
            }
            //Si toutes les dates d'action sont déjà passées, on laisse "plan_termine" à "true"

        } else { //Si on a chargé le plan par défaut
            plan_termine = false;
        }
    }

    return plan_termine;
}

int Manager::orienter_satellite(float command_pitch, float command_roll, float command_yaw, double date_action, double timeout, bool recovery) {

    double date_actuelle, timeout_end;
    float command_yaw_recue, command_pitch_recue;
    bool orientation_effectuee = false;
    int ifmsg = 0;
    Type_Message msg_recu;
    const char* message_recu;
    char message[256];
    size_t size = myCvector.emetteur.size() + 1;
    char * nom_machine = new char[ size ];
    strncpy(nom_machine, myCvector.emetteur.c_str(), size);

    if (horloge->get_temps() < date_action - timeout) timeout_end = timeout + horloge->get_temps();
    else timeout_end = date_action;

    orientation_effectuee = false;

    //envoi du message à la partition SCAO (selection du bon socket selon la partition)	
    sprintf(message, "%f %f %g", command_yaw, command_pitch, date_action);
    //cout << "<SCAO> Sending orientation " << message << " to SCAO on sampling port " << myCvector.vsamp_port[2] << " with socket " << myCvector.vsamp_socket[2] << "..." << endl;
    Sservice.WRITE_SAMPLING_MESSAGE(nom_machine, myCvector.vsamp_port[2], myCvector.vsamp_socket[2], myCvector.emetteur, message);

    //attente de la réponse tant que la date de l'action n'est pas passee

    date_actuelle = horloge->get_temps();

    while (((date_actuelle < timeout_end) && (!orientation_effectuee)) || recovery) {

        // Lecture du port queuing
        //cout << "<SCAO> Waiting for report from SCAO at queuing port with socket " << myCvector.vqueuing_socket[2] << ". (" << date_action-date_actuelle << "s last)" << endl;
        recovery = false;
        ifmsg = Qservice.READ_QUEUING_MESSAGE(myCvector.vqueuing_socket[2]);

        if (ifmsg > 0) {

            // Lecture du message		
            msg_recu = Qservice.get_Message();
            message_recu = msg_recu.m_message;

            // Extraction de données du checkpoint à partir du message
            sscanf(message_recu, "%f %f", &command_yaw_recue, &command_pitch_recue);
            //cout << "<SCAO> Report received from SCAO : " << message_recu << "." << endl;

            // Verification que le message est bien celui attendu
            if ((command_yaw_recue == command_yaw)&&(command_pitch_recue == command_pitch)) {

                // Si c'est le message attendu, on quitte le while
                orientation_effectuee = true;
            }

        }

        //actualisation heure actuelle
        date_actuelle = horloge->get_temps();
    }

    //selection de la valeur de retour
    if (orientation_effectuee == true) {
        return 1;
    } else {
        return 0;
    }
}

void Manager::find_next_action() {
    // Lecture de la prochaine action du plan
    action_en_cours = plan.next_action();
    date_action = horloge->date2seconds(action_en_cours.date_action);

    cout << "From " << myPartition << " : " << "<Manager> Next action : " << plan.get_id_action_courante() + 1 << " / " << plan.get_taille() << endl;
}

int Manager::order_transfer(bool* order_status, char* name, double date_action, double duration, double timeout) {

    int flag = 1;
    char message[256];
    char type_action[10] = "TSF";
    size_t size = myCvector.emetteur.size() + 1;
    char * nom_machine = new char[ size ];
    strncpy(nom_machine, myCvector.emetteur.c_str(), size);

    sprintf(message, "%s %s %g %d %g %d", type_action, name, date_action, 0, duration, 0);

    //cout << "<Camera> Sending order to LEICA on sampling port " << myCvector.vsamp_port[1] << " with socket " << myCvector.vsamp_socket[1] << "..." << endl;

    Sservice.WRITE_SAMPLING_MESSAGE(nom_machine, myCvector.vsamp_port[1], myCvector.vsamp_socket[1], myCvector.emetteur, message);

    flag = receive_transfer_report(order_status, name, date_action, duration, timeout);

    return flag;

}

int Manager::receive_transfer_report(bool* order_status, char* name, double date_action, double duration, double timeout) {

    int flag = 1;
    char id_recu[30] = "INITIAL_VALUE";
    int status = 0;
    double date_actuelle, timeout_start;
    int ifmsg = 0;
    Type_Message msg;

    if (horloge->get_temps() < date_action) timeout_start = date_action;
    else timeout_start = horloge->get_temps();

    while (strcmp(name, id_recu) && flag) {//on sort si le timeout est dépassé ou si on a reçu un message et qu'il contient la valeur de l'ID de l'image

        date_actuelle = horloge->get_temps();
        flag = (int) (date_actuelle < (timeout_start + timeout + duration)); //verification de la date

        //cout << "<Camera> Waiting for report from LEICA on queuing port with socket " << myCvector.vqueuing_socket[1] << "..." << endl;
        ifmsg = Qservice.READ_QUEUING_MESSAGE(myCvector.vqueuing_socket[1]); //recherche de message

        if (ifmsg > 0) { //lecture du message
            msg = Qservice.get_Message();

            cout << " Report received from LEICA : " << msg.m_message << "." << endl;

            sscanf(msg.m_message, "%d %s", &status, id_recu);

            if (status == 1) {
                flag = 1;
            } else flag = 0;
        }

    }

    *order_status = (status == 1);

    return flag;

}

int Manager::capturer_image(char* name, int largeur, int duree_image, int qualite_jpg, double date_action, double timeout, bool recovery) {

    //arg est le argv[0] du master

    string id_recu = "INITIAL_VALUE";
    string id_ref = name;
    char id_recu_temp[30] = "INITIAL_VALUE";
    string id_recu_precedemment = "INITIAL_VALUE";
    int status[5] = {0, 0, 0, 0, 0};
    double date_actuelle;
    int ifmsg = 0;
    bool flag = true;
    char message[256];
    char type_action[10] = "IMG";
    Type_Message msg;
    Checkpoint dernier_checkpoint_recu;
    Checkpoint avant_dernier_checkpoint_recu;

    //double date_reference = horloge->get_temps();
    size_t size = myCvector.emetteur.size() + 1;
    char * machine_name = new char[ size ];
    // copier la chaîne
    strncpy(machine_name, myCvector.emetteur.c_str(), size);

    sprintf(message, "%s %s %g %d %g %d", type_action, name, date_action, largeur, (double) duree_image, qualite_jpg);

    //cout << "<Camera> Sending order to LEICA on sampling port " << myCvector.vsamp_port[1] << " with socket " << myCvector.vsamp_socket[1] << "..." << endl;

    Sservice.WRITE_SAMPLING_MESSAGE(machine_name, myCvector.vsamp_port[1], myCvector.vsamp_socket[1], myCvector.emetteur, message);

    while ((id_ref != id_recu) && flag) {//on sort si le timeout est dépassé ou si on a reçu un message et qu'il contient la valeur de l'ID de l'image

        date_actuelle = horloge->get_temps();

        flag = (int) (date_actuelle < (date_action + timeout)); //verification de la date
        if (recovery) {
            if (id_recu_precedemment != id_recu) {
                size_t size = id_recu.size() + 1;
                char * c_id_recu = new char[ size ];

                // copier la chaîne
                strncpy(c_id_recu, id_recu.c_str(), size);
                avant_dernier_checkpoint_recu.set_checkpoint(1, atoi(c_id_recu));
                avant_dernier_checkpoint_recu.set_etat(0, status[0]);
                avant_dernier_checkpoint_recu.set_etat(1, status[1]);
                avant_dernier_checkpoint_recu.set_etat(2, status[2]);
                avant_dernier_checkpoint_recu.set_etat(3, status[3]);
                avant_dernier_checkpoint_recu.set_etat(4, status[4]);
                id_recu_precedemment = id_recu;
            }
        }
        //cout << "<Camera> Waiting for report from LEICA on queuing port with socket " << myCvector.vqueuing_socket[1] << "..." << endl;
        ifmsg = Qservice.READ_QUEUING_MESSAGE(myCvector.vqueuing_socket[1]); //recherche de message

        if (ifmsg > 0) { //lecture du message
            msg = Qservice.get_Message();

            cout << "<Camera> Report received from LEICA : " << msg.m_message << "." << endl;

            sscanf(msg.m_message, "%d %d %d %d %d %s", &status[0], &status[1], &status[2], &status[3], &status[4], id_recu_temp);
            id_recu = id_recu_temp;

            dernier_checkpoint_recu.set_checkpoint(action_en_cours.numero_action, action_en_cours.numero_image);
            dernier_checkpoint_recu.set_etat(0, status[0]);
            dernier_checkpoint_recu.set_etat(1, status[1]);
            dernier_checkpoint_recu.set_etat(2, status[2]);
            dernier_checkpoint_recu.set_etat(3, status[3]);
            dernier_checkpoint_recu.set_etat(4, status[4]);
            /*if (status[0]==1){//??????
                    flag=1;///????
            }///?????
            else flag=false;//????*/

        }

    }

    if (id_ref == id_recu) {
        if (recovery) {
            table.maj(avant_dernier_checkpoint_recu);
            send_check(avant_dernier_checkpoint_recu);
        }
        table.maj(dernier_checkpoint_recu);
        send_check(dernier_checkpoint_recu);

    } else {

        dernier_checkpoint_recu.set_checkpoint(action_en_cours.numero_action, action_en_cours.numero_image);
        dernier_checkpoint_recu.set_etat(0, 0);
        dernier_checkpoint_recu.set_etat(1, 0);
        dernier_checkpoint_recu.set_etat(2, 0);
        dernier_checkpoint_recu.set_etat(3, 0);
        dernier_checkpoint_recu.set_etat(4, 0);
        table.maj(dernier_checkpoint_recu);
        send_check(dernier_checkpoint_recu);
        if (recovery) {
            table.maj(avant_dernier_checkpoint_recu);
            send_check(avant_dernier_checkpoint_recu);
        }
    }

    return flag;
}

int Manager::gerer_mission(string myPart, string smode, Horloge* hlge, Analyseur* analyseur_sat, SCAO* scao_sat, Camera* camera_sat, Memoire_stable* mem_stable_sat, int nbarg, char* arg, CarteComm* carte_comm_sat, CarteRecep* carte_recep_sat, COMMUNICATION_VECTOR myCvector_sat) {

    // Variables liees aux actions
    double date_actuelle;
    int nombre_transfert, duree_transfert;
    int id_image_a_envoyer;
    int retour_scao, retour_camera, retour_carte;
    bool plan_termine = false;
    bool first_launch = true;
    Checkpoint dernier_check;
    int flag_default = 0;
    // bool etat_IMG[5] = {false, false, false, false, false};
    bool etat_TSF = false;
    char image_name[30] = "\0";

    //Synchronisation des variables
    analyseur = analyseur_sat;
    //scao = scao_sat;                                                      unused ???
    //camera = camera_sat;                                                  unused ???
    horloge = hlge;
    memoire_stable = mem_stable_sat;
    mode = smode;
    carte_communication = carte_comm_sat;
    carte_reception = carte_recep_sat;

    // Definition du mode de fonctionnement : Primary, Backup
    set_mode(smode);

    // Definition de la partition hote
    strncpy(myPartition, myPart.c_str(), 6);

    // Configuration du vecteur de communication
    myCvector = myCvector_sat;

    /*if (mode=='p') {
            //Creation du fichier plan initial      
            Creer_Plan(5, 42, "NewPlan.txt");
	
            //Importation du plan
            plan.set_plan(1); // 1 <=> NewPlan.txt
    }*/

    while (true) {

        // MODE PRIMARY -------------------------------------------------------
        if (mode == "p") {

            plan_termine = get_new_plan(plan_termine, first_launch);



            /* tant que le plan n'est pas termine */
            while (plan_termine == false) {// TODO: a modifier

                /// Lecture de la prochaine action du plan
                find_next_action();

                // ## ACTION IMAGE ##
                if (strcmp(action_en_cours.nature.c_str(), "IMG") == 0) {
                    check.set_checkpoint(action_en_cours.numero_action, action_en_cours.numero_image);
                    check.set_etat(0, 0);
                    check.set_etat(1, 0);
                    check.set_etat(2, 0);
                    check.set_etat(3, 0);
                    check.set_etat(4, 0);
                    table.maj(check);
                    send_check(check);
                    // 1- Orientation satellite
                    retour_scao = orienter_satellite(action_en_cours.angle_prise_vue[0], action_en_cours.angle_prise_vue[1], action_en_cours.angle_prise_vue[2], date_action, TIMEOUT_SCAO, false);
                    // return 1 si bien deroule
                    if (retour_scao == 0) {
                        cout << "From " << myPartition << " : " << "<Manager/SCAO> Satellite orientation ERROR.";
                        cout << " The current action will be skipped." << endl;

                    } else {
                        // TODO : tolerance faute mineure ( reboot scao ? ou command again )

                        cout << "From " << myPartition << " : " << "<Manager/SCAO> Satellite orientation OK." << endl;

                        cout << "From " << myPartition << " : " << "<Manager> Waiting for capturing still (" << date_action - horloge->get_temps() << " seconds)." << endl;
                        //cout << "From " << myPartition << " : " << "<Manager> Date de l'action : " << date_action << " secondes, date actuelle : "<< horloge->get_temps() << " secondes."<< endl;



                        // 3- Prise d'image
                        //check.set_checkpoint(action_en_cours.numero_action, action_en_cours.numero_image);

                        sprintf(image_capturee, "%ld.jpg", action_en_cours.numero_image); // Creation de l'identifiant de l'image : jour mois heure minute seconde
                        retour_camera = capturer_image(image_capturee, 1000, action_en_cours.duree_action, 75, date_action, TIMEOUT_IMG, false);

                        //retour_camera = camera->capturer(etat_IMG, image_capturee, 1000, action_en_cours.duree_action, 75, date_action, TIMEOUT_IMG);
                        if (retour_camera == 0) {


                        } else {


                            cout << "From " << myPartition << " : " << "<Manager> ERROR while executing capture order." << endl;

                        }
                        //attention checkpoints envoyés durant la capture!
                    }
                }
                    // ## ACTION TRANSFERT ##
                else if (strcmp(action_en_cours.nature.c_str(), "TSF") == 0) {

                    // Recuperation des arguments de l'action
                    duree_transfert = action_en_cours.duree_action;
                    nombre_transfert = action_en_cours.nbre_images_a_envoyer;

                    // Attente date action

                    cout << "From " << myPartition << " : " << "<Manager> Waiting for transfer time ("
                            << date_action - horloge->get_temps() << " seconds)." << endl;

                    do {

                        date_actuelle = horloge->get_temps();

                    } while (date_actuelle < date_action);


                    // Transfert

                    int cpt_transfert = 0;

                    cout << "From " << myPartition << " : " << "<Manager> Transfer in progress..." <<
                            endl;


                    while ((horloge->get_temps() < (date_action + duree_transfert)) && cpt_transfert < nombre_transfert) {

                        cout << "From " << myPartition << " : " << "<Manager> Transfer #" << cpt_transfert + 1 << "/" << nombre_transfert << " :" << endl;

                        //cout << "<Manager> Boucle transfert " << cpt_transfert << endl;

                        id_image_a_envoyer = action_en_cours.id_images_a_envoyer[cpt_transfert];

                        cout << "From " << myPartition << " : " << "<Manager> Sending still : " << id_image_a_envoyer << ".jpg." << endl;
                        //vérification de l'image par récupération du checkpoint dans la table

                        //cout << "From " << myPartition << " : " << "<Manager> Position dans la table : " << table_id_image_to_position(id_image_a_envoyer) << "." << endl;

                        // Récupération du checkpoint correspondant à cet id_image	
                        dernier_check = table_get_check(table_id_image_to_position(id_image_a_envoyer)); // TODO : gestion de l'erreur check non trouvé : renvoyer un check vide

                        if (id_image_a_envoyer == dernier_check.get_id_image_sauvegarde()) {
                            //on trouve un checkpoint correspondant à l'image dans la table

                            //cout << "From " << myPartition << " : " << "<Manager> Still found in checkpoint table." << endl;

                            if (dernier_check.get_etat(3) == true) { //l'image est envoyable (analysée et compréssée

                                //cout << "From " << myPartition << " : " << "<Manager> Still found in stable memory." << endl;

                                sprintf(image_name, "%d.jpg", id_image_a_envoyer);

                                retour_carte = order_transfer(&etat_TSF, image_name, date_action, duree_transfert, TIMEOUT_TSF);
                                if (retour_carte == 1) {


                                } else {

                                    //TODO : problème à gérer
                                    cout << "From " << myPartition << " : " << "<Manager/CarteComm> Transmission ERROR." << endl;

                                }

                                // Envoi du checkpoint
                                table.maj(id_image_a_envoyer, 4, etat_TSF); //sauvegarde de l'envoi sur la partition primaire

                                check.set_checkpoint(dernier_check.get_id_action(), id_image_a_envoyer); //MAJ checkpoint à envoyer au backup

                                check.set_etat(0, dernier_check.get_etat(0));
                                check.set_etat(1, dernier_check.get_etat(1));
                                check.set_etat(2, dernier_check.get_etat(2));
                                check.set_etat(3, dernier_check.get_etat(3));
                                check.set_etat(4, etat_TSF);

                                //envoi checkpoint au backup
                                send_check(check);

                            } //si l'image n'est pas propre à l'envoie on ne fait rien
                        } //si l'image n'existe pas on ne fait rien non plus

                        cpt_transfert++;

                        // Attente reception du plan
                        // check.set_checkpoint(action_en_cours.numero_action,1);

                        cout << "From " << myPartition << " : " << "<Manager> Still " << id_image_a_envoyer << ".jpg transferred." << endl;


                    }


                    cout << "From " << myPartition << " : " << "<Manager> Tranfert done." << endl;

                } else if (strcmp(action_en_cours.nature.c_str(), "PLA") == 0) {

                    cout << "From " << myPartition << " : " << "<Manager> Changing plan order..." << endl;

                    //Creation du fichier plan
                    Creer_Plan(5, 20, "NewPlan.txt");

                    plan.set_plan(1); // 1 <=> Newplan.txt

                    cout << "From " << myPartition << " : " << "<Manager> Plan changed." << endl;
                } else if (strcmp(action_en_cours.nature.c_str(), "ATT") == 0) {

                    cout << endl << "From " << myPartition << " : " << "<Manager> Waiting action..." << endl;
                    usleep(500000);
                    if (flag_default == 0) {
                        orienter_satellite(49, 0, 170, horloge->get_temps() + TIMEOUT_SCAO, TIMEOUT_SCAO, false);
                        flag_default = 1;
                    } else if (flag_default == 1) {
                        orienter_satellite(49, 0, 10, horloge->get_temps() + TIMEOUT_SCAO, TIMEOUT_SCAO, false);
                        flag_default = 0;
                    }

                    usleep(500000);
                    cout << "From " << myPartition << " : " << "<Manager> End of waiting action." << endl << endl;

                }


                // Vérification de terminaison du plan
                if (plan.get_id_action_courante() + 1 >= plan.get_taille()) plan_termine = true;

                //RAZ du checkpoint
                for (int i = 0; i < 5; i++) check.set_etat(i, false);

                cout << "--------------------------------------" << endl << endl;
            } //END while(plan_termine == false)

        }



        // MODE BACKUP -------------------------------------------------------
        if (mode == "b") { // ne fait rien
            if (first_launch) first_launch = false;
        }

    }


    return 0;
}

void Manager::init_plan(Plan pl) {// remplace le plan courant par pl

    plan.set_plan_courant(pl.get_plan_courant());
    plan.set_id_action_courante(pl.get_id_action_courante());
    plan.set_liste_actions(pl.get_liste_actions());
    plan.set_taille(pl.get_taille());
}

int Manager::trouver_action(int i) {

    //cout << "<Trouver Action>" << endl;

    action action_trouvee;
    if (i <= 0) action_trouvee = plan.next_action();
    else action_trouvee = plan.get_action(i);

    //plan.afficher_action(action_trouvee);

    int security = 0;

    /*cout << "<Trouver Action> : " <<
    horloge->date2seconds(action_trouvee.date_action) << " - " << horloge->get_temps() << " = " <<
    horloge->date2seconds(action_trouvee.date_action) - horloge->get_temps() << endl;
    cout << "<Trouver Action> : Security = " << security << " <? " << (plan.get_taille()) << endl;*/

    while (((horloge->date2seconds(action_trouvee.date_action) - horloge->get_temps()) < 0) &&
            (security < plan.get_taille())) {

        /*cout << "<Trouver Action> : WHILE" << endl;
        cout << "<Trouver Action> : " <<
        horloge->date2seconds(action_trouvee.date_action) << " - " << horloge->get_temps() << " = " <<
        horloge->date2seconds(action_trouvee.date_action) - horloge->get_temps() << endl;
        cout << "<Trouver Action> : Security = " << security << " <? " << (plan.get_taille()+1) << endl;*/

        security++;
        action_trouvee = plan.next_action();
    }

    if (security == plan.get_taille()) {
        //cout << "<Trouver Action> : SECURITY RETURN = -1" << endl;
        return -1;
    } else {

        //cout << "<Trouver Action> : RETURN = " << plan.get_id_action_courante() << endl;
        return plan.get_id_action_courante();
    }
}

void Manager::Creer_Plan(int date_debut, int nb_instr, string fichier) {


    int temp = 0;
    char ligne[30];
    int delai[10];
    int new_s[10];
    int new_mn[10];
    int new_hr[10];
    int NB_LIGNES;
    int delta = 0;
    char nom_fichier[250];


    time_t rawtime;
    struct tm * timer;

    time(&rawtime);
    timer = localtime(&rawtime); // acquisition de la date sous format Y/M/D/H/m/s
    timer->tm_mon = timer->tm_mon + 1;
    timer->tm_year = timer->tm_year + 1900;
    timer->tm_isdst = -1;


    delai[0] = date_debut;
    NB_LIGNES = nb_instr;
    sprintf(nom_fichier, "../../sources/Chaire_SE_Student/plans/%s", fichier.c_str());
    /*std::cout<< "Délai avant début de la première instruction (en s): " << std::endl;
    std::cin>>delai[0];
    std::cout<< std::endl <<"Nombre d'instructions :" << std::endl;
    std::cin>>NB_LIGNES;
    std::cout<< std::endl <<"Nom de fichier :" << std::endl;
    std::cin>>nom_fichier;*/


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

        // CHANGE PLAN ACTION
        if (k == 20) {
            Plan << k << " PLA" << std::endl;
        }            // TRANSFER ACTION
        else if (k == 5) {

            delta = 0;
            temp = rand() % 3 + 1;
            Plan << k << " TSF  " << timer->tm_mday << " " << timer->tm_mon << " " << timer->tm_year << " " << new_hr[k % 10] << " " << new_mn[k % 10] << " " << new_s[k % 10] << " " << temp * 7 << " " << temp;

            for (int i = 1; i < temp + 1; i++) {
                sprintf(ligne, " %d%d%d%d%d ", timer->tm_mday, timer->tm_mon, new_hr[(k - i) % 10], new_mn[(k - i) % 10], new_s[(k - i) % 10]);
                Plan << ligne;
            }
            if (k != NB_LIGNES) {
                Plan << std::endl;
            }

            delai[k % 10] += temp * 7;

        }// CAPTURE ACTION
        else {
            if ((rand() % 100 < 90) || (k < 10) || (delta < 10)) {
                sprintf(ligne, " %d%d%d%d%d ", timer->tm_mday, timer->tm_mon, new_hr[k % 10], new_mn[k % 10], new_s[k % 10]);
                Plan << k << " IMG " << ligne << " " << timer->tm_mday << " " << timer->tm_mon << " " << timer->tm_year << " " << new_hr[k % 10] << " " << new_mn[k % 10] << " " << new_s[k % 10] << " " << rand() % 2 + 2 << "  " << 5 + rand() % 170 << " " << 5 + rand() % 170 << " " << 5 + rand() % 170;
                if (k != NB_LIGNES) {
                    Plan << std::endl;
                }
                delta++;
            } else {
                delta = 0;
                temp = rand() % 10;
                Plan << k << " TSF  " << timer->tm_mday << " " << timer->tm_mon << " " << timer->tm_year << " " << new_hr[k % 10] << " " << new_mn[k % 10] << " " << new_s[k % 10] << " " << temp * 7 << " " << temp;

                for (int i = 1; i < temp + 1; i++) {
                    sprintf(ligne, " %d%d%d%d%d ", timer->tm_mday, timer->tm_mon, new_hr[(k - i) % 10], new_mn[(k - i) % 10], new_s[(k - i) % 10]);
                    Plan << ligne;
                }
                if (k != NB_LIGNES) {
                    Plan << std::endl;
                }

                delai[k % 10] += temp * 7;

            }
        }



    }

    Plan.close();

}

