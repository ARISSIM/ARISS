#include "include/CBasefunction.h"

// Inclusion des objets de gestion du satellite
#include "../Analyseur.h"
#include "../SCAO.h"
#include "../Horloge.h"
#include "../Plan.h"
#include "../Camera.h"
#include "../Memoire_stable.h"
#include "../Table.h"
#include "../Checkpoint.h"
#include "../Manager.h"
#include "../CarteComm.h"

#define MAX_COMM_SIZE 4
#define TIMEOUT_IMG 10
#define TIMEOUT_TSF 10
#define TIMEOUT_SCAO 10

using namespace std;



struct Arg_Manager{
     Analyseur* analyseur_sat;
     SCAO* scao_sat;
     Camera* camera_sat;
     Memoire_stable* mem_stable_sat;
     Horloge* horloge_sat; 
     Manager* manager_sat;
     COMMUNICATION_VECTOR myCvector;
     CQueuing Qservice; 
     CarteComm* carte_communication_sat;
     CarteRecep* carte_reception_sat; 
     int myArgumentc;
     string mode_fonctionnement;	
     char* myArgumentv;

};

struct Arg_Recovery{
	CSampling Sservice;
	COMMUNICATION_VECTOR myCvector;
	Horloge* horloge_sat;
	bool* flag_cancel_thread;
};
struct Arg_Non_Fonctionnel{
	Analyseur* analyseur_sat;
	SCAO* scao_sat;
	Camera* camera_sat;
	Memoire_stable* mem_stable_sat;
	Horloge* horloge_sat;
	Manager* manager_sat;
	COMMUNICATION_VECTOR myCvector;
	CSampling Sservice;
	CQueuing Qservice; 
	CarteComm* carte_communication_sat; 
	string mode_fonctionnement;
     	int myArgumentc;
     	char* myArgumentv;
};


	char myName[10] = "MASTER";
	


sig_t bye()
{
  cout << "From " << myName <<  " : Goodbye !" << endl;
  exit(0);
}

void reactivate_watchdog(CSampling Sservice, COMMUNICATION_VECTOR myCvector, Horloge* myHorloge){
	char message[256];
	sprintf(message,"%g", myHorloge->get_temps());		
	size_t size = myCvector.emetteur.size() + 1;
   	char * nom_machine = new char[ size ];
	        // copier la chaîne
    	strncpy( nom_machine, myCvector.emetteur.c_str(), size );
	
	Sservice.WRITE_SAMPLING_MESSAGE(nom_machine, myCvector.vsamp_port[0], myCvector.vsamp_socket[0], myCvector.emetteur,message);

}

int research_crash(CSampling Sservice, COMMUNICATION_VECTOR myCvector, Horloge* myHorloge, double* my_p_date_watchdog, int myDelayWD){
	Type_Message msg;
	const char* mess;
	double date_lue;
	int ifmsg;
	int ret=0;
	// Lecture du port sampling
	ifmsg=Sservice.READ_SAMPLING_MESSAGE(myCvector.vsamp_socket[0]); // Lecture du port sampling
	// Traitement du message lu
	if(ifmsg>0){ 
		// Recuperation message
		msg = Sservice.get_Message();
	        mess = msg.m_message;
			
		// Conversion du message en entier
		//date_lue = atof(mess);
		date_lue=strtod(mess, NULL);

		*my_p_date_watchdog=myHorloge->get_temps();

		if ( (myHorloge->get_temps() - date_lue) > myDelayWD) {
			ret = 1;
					}
		}
	 if ((ifmsg==0)&&(myHorloge->get_temps()-*my_p_date_watchdog)>(myDelayWD+5)) {
		ret=1;
		cout << "From " << myName << " : " <<"Empty sampling port, there must be an error..."<<endl;
		}

	return ret;		

}


void receive_checkpoints(CQueuing Qservice, COMMUNICATION_VECTOR myCvector, Manager* myManager, char MyName[]){
	int ifmsg_check=1;
	const char* mess_check;
	Type_Message msg_check;
	int numero_action_lue; 
	int etat_lu[5], id_image_lu;
	Checkpoint check_lu;
	bool table_etat_lu[5];

	while ( ifmsg_check > 0){
					
		// Lecture du port Queuing
		ifmsg_check = Qservice.READ_QUEUING_MESSAGE(myCvector.vqueuing_socket[0]);
						
		if(ifmsg_check>0){
			
		// Lecture du message
		//arg->Qservice.Display_Message();
		msg_check = Qservice.get_Message();
		mess_check = msg_check.m_message;
			
		// Extraction de données du checkpoint à partir du message
		sscanf(mess_check,"%d %d %d %d %d %d %d", &id_image_lu, &numero_action_lue, &etat_lu[0], &etat_lu[1], &etat_lu[2], &etat_lu[3], &etat_lu[4]);
			
		cout << "From " << MyName << " : " << "<Checkpoint> Reading checkpoint : " << mess_check << endl;
			
		// Conversion des entiers "etat_lu" en booléens
		for (int i = 0; i < 5; i++) table_etat_lu[i] = (etat_lu[i] == 1) ;
			
			// Stockage de ces données dans un checkpoint
			check_lu.set_checkpoint(numero_action_lue, id_image_lu);
			check_lu.set_tous_etats(table_etat_lu);
			
			// Mise à jour de la table
			myManager->actualiser_table(check_lu);
						}
		}

}

void r_image(Manager* myManager, Horloge* myHorloge, Camera* myCamera, action action_en_cours,int numero_derniere_action, Plan plan, Checkpoint dernier_check){
	action prochaine_action;
	char liste_etat[5];
	//Checkpoint dernier_check;
	int etat_derniere_action;
	char image_name[30] = "\0";
	bool etat_IMG[5] = {false, false, false, false, false};
	int return_orientation;
	
				
	sprintf(liste_etat, "%d%d%d%d%d", dernier_check.get_etat(0), dernier_check.get_etat(1), dernier_check.get_etat(2),
	dernier_check.get_etat(3), dernier_check.get_etat(4));
	etat_derniere_action = atoi(liste_etat);
	prochaine_action = plan.get_action(numero_derniere_action);
	cout << "From " << myName << " : " << " Last checkpoint received : Action N°" << numero_derniere_action << " \tStates " << etat_derniere_action << endl;
	cout << "From " << myName << " : " << "<Recovery> Action to do : Still capture." << endl;

	switch(etat_derniere_action){
            
                case 00000 :
                    if (myHorloge->get_temps() <  (myHorloge->date2seconds(prochaine_action.date_action)-TIMEOUT_IMG-TIMEOUT_SCAO)){
                            cout << "From " << myName << " : " << "<Recovery> Still not yet captured." << endl;
                            cout << "From " << myName << " : " << "<Recovery> Must be captured, processed and saved..." << endl;
                            
                            sprintf(image_name, "%ld.jpg", action_en_cours.numero_image);

			
		            return_orientation=myManager->orienter_satellite(action_en_cours.angle_prise_vue[0], action_en_cours.angle_prise_vue[1], action_en_cours.angle_prise_vue[2],myHorloge->date2seconds(action_en_cours.date_action), TIMEOUT_SCAO, true);
			    
			    if(return_orientation==1) {
					cout << "From " << myName << " : <Recovery> Satellite orientation OK."<<endl;
					myManager->capturer_image(image_name, 1000, action_en_cours.duree_action, 75, myHorloge->date2seconds(action_en_cours.date_action), TIMEOUT_IMG, true);
			    }
                            /*myCamera->capturer(etat_IMG, image_name, 1000, action_en_cours.duree_action, 75, myHorloge->date2seconds(action_en_cours.date_action), TIMEOUT_IMG);*/
			    else{
                            dernier_check.set_etat(0, etat_IMG[0]);
                            dernier_check.set_etat(1, etat_IMG[1]);
                            dernier_check.set_etat(2, etat_IMG[2]);
                            dernier_check.set_etat(3, etat_IMG[3]);
                            
                            myManager->actualiser_table(dernier_check);
                            myManager->send_check(dernier_check);
                            }
                    } else {
                        
                            cout << "From " << myName << " : " << "<Recovery> Still not yet captured but too late." << endl;
                    }

                break;

		case 11110 : case 11010 : //Image prise, analysée, stockée mais non transmise
			cout << "From " << myName << " : " << "<Recovery> Still captured, processed and saved." << endl;
		break;

		default :
			cout << "From " << myName << " : " << "<Recovery> Incorrect checkpoint !" << endl;
		break;
	}	
} // 

void r_TSF(Manager* myManager, Horloge* myHorloge, action action_en_cours){

	int cpt_transfert=0;
	int  duree_transfert, nombre_transfert, retour_carte;
	double date_action, date_actuelle;
	int id_image_a_envoyer;
	Checkpoint dernier_check;
	char image_name[30] = "\0";
	bool etat_TSF;
	cout << "From " << myName << " : " << "<Recovery> Action to do : Stills transfer." << endl;

	// Recuperation des arguments de l'action
	duree_transfert = action_en_cours.duree_action;
	nombre_transfert = action_en_cours.nbre_images_a_envoyer;
	date_action =myHorloge->date2seconds(action_en_cours.date_action);
        				
	// Attente date action
	cout << "From " << myName << " : " << "<Recovery> Waiting for transfer time (" << date_action - myHorloge->get_temps() << " seconds)." << endl;

	do{

		date_actuelle = myHorloge->get_temps();

	} while(date_actuelle < date_action);

	cout << "From " << myName << " : " << "<Recovery> Beginning action." << endl;
			
	// Début Transfert

	// Initialisation nombre d'images à transférer
	

	cout << "From " << myName << " : " << "<Manager> Transfer in progress..." << endl;

        while ((myHorloge->get_temps() < (date_action + duree_transfert)) && cpt_transfert < nombre_transfert) {

            cout << "From " << myName << " : " << "<Manager> Transfer #" << cpt_transfert + 1 << "/" << nombre_transfert << " :" << endl;

            //cout << "<Manager> Boucle transfert " << cpt_transfert << endl;

            id_image_a_envoyer = action_en_cours.id_images_a_envoyer[cpt_transfert];

            cout << "From " << myName << " : " << "<Manager> Sending still : " << id_image_a_envoyer << ".jpg." << endl;
            //vérification de l'image par récupération du checkpoint dans la table

            //cout << "From " << myName << " : " << "<Manager> Position dans la table : " << table_id_image_to_position(id_image_a_envoyer) << "." << endl;

            // Récupération du checkpoint correspondant à cet id_image	
            dernier_check = myManager->table_get_check(myManager->table_id_image_to_position(id_image_a_envoyer)); // TODO : gestion de l'erreur check non trouvé : renvoyer un check vide

            if (id_image_a_envoyer == dernier_check.get_id_image_sauvegarde()) {
                //on trouve un checkpoint correspondant à l'image dans la table

                //cout << "From " << myName << " : " << "<Manager> Still found in checkpoint table." << endl;

                if (dernier_check.get_etat(3) == true) { //l'image est envoyable (analysée et compréssée

                    //cout << "From " << myName << " : " << "<Manager> Still found in stable memory." << endl;

                    sprintf(image_name, "%d.jpg", id_image_a_envoyer);

                    retour_carte = myManager->order_transfer(&etat_TSF, image_name, date_action, duree_transfert, TIMEOUT_TSF);
                    if (retour_carte == 1) {


                    } else {

                        //TODO : problème à gérer
                        cout << "From " << myName << " : " << "<Manager/CarteComm> Transmission ERROR." << endl;

                    }

                    // Envoi du checkpoint
                    myManager->actualiser_table(id_image_a_envoyer, 4, etat_TSF); //sauvegarde de l'envoi sur la partition primaire
                    dernier_check.set_etat(4,etat_TSF);
                    myManager->send_check(dernier_check);

                } //si l'image n'est pas propre à l'envoie on ne fait rien
            } //si l'image n'existe pas on ne fait rien non plus

            cpt_transfert++;

            // Attente reception du plan
            // check.set_checkpoint(action_en_cours.numero_action,1);

            cout << "From " << myName << " : " << "<Manager> Still " << id_image_a_envoyer << ".jpg transferred." << endl;


        }

	cout << "From " << myName << " : " << "<Manager> Transfer done." << endl;

} 

// Thread Fonctionnel
void* f_thread_F(void *args) 
{
	
	struct Arg_Manager *arg=(Arg_Manager*)args;
	arg->manager_sat->gerer_mission("MASTER", arg->mode_fonctionnement, arg->horloge_sat, arg->analyseur_sat, arg->scao_sat, arg->camera_sat,
	arg->mem_stable_sat, arg->myArgumentc, arg->myArgumentv,
	arg->carte_communication_sat,arg->carte_reception_sat, arg->myCvector);

 	cout << "From " << myName << " : End of functional thread." << endl;
 	

	return NULL;
}

void* f_thread_recovery(void *args){
	
	cout << "From " << myName << " : Start of SPECIAL thread." << endl;

	struct Arg_Recovery *arg=(Arg_Recovery*)args;
	while(*arg->flag_cancel_thread == false){
	
		reactivate_watchdog(arg->Sservice, arg->myCvector, arg->horloge_sat);
		usleep(50000);	
				
		}
	
	cout << "From " << myName << " : End of SPECIAL thread." << endl;
	return NULL;
}


// Thread Non Fonctionnel
void* f_thread_NF(void *args) // FAILURE DETECTOR
{
    
	struct Arg_Non_Fonctionnel *arg=(Arg_Non_Fonctionnel*)args;
    	// Variables liees au failure detector
	
	int crash_detecte=0;
	int delai_WD = 2;
		
	//Variables liées au recovery
	Plan plan;
	action action_en_cours;
	Checkpoint dernier_check;
	bool flag_cancel_thread = false;

	int numero_derniere_action;
	// Variables liees aux actions
	
	int  retour_plan=0;	
	
	// Variable de mode de fonctionnement
	string mode_partition = arg->mode_fonctionnement;
	double date_watchdog=arg->horloge_sat->get_temps();	
	double* p_date_watchdog=&date_watchdog;
	struct Arg_Recovery  arguments_recovery;
	arguments_recovery.Sservice=arg->Sservice;
	arguments_recovery.myCvector=arg->myCvector;
	arguments_recovery.horloge_sat=arg->horloge_sat;
	arguments_recovery.flag_cancel_thread = &flag_cancel_thread;

	pthread_attr_t *thread_attributes;
	pthread_t *thread_recovery;

	thread_attributes=(pthread_attr_t *)malloc(sizeof(pthread_attr_t));
	thread_recovery=(pthread_t *)malloc(sizeof(pthread_t));

	pthread_attr_init(thread_attributes);
    	cout << "From " << myName << " : Non-functional thread started." << endl;
	
	while(true){
	
		// MODE PRIMARY = Ecriture du Watchdog ------------------------------------------------------
		if(mode_partition == "p"){
		

				while(true){
					reactivate_watchdog(arg->Sservice, arg->myCvector, arg->horloge_sat);
											
					usleep(500000);

				}

		}
		
	
	
		/// MODE BACK-UP  = Lecture du Watchdog -----------------------------------------------------
		if (mode_partition == "b"){
		
			cout << endl << "--------------------------------------" << endl ;
		
			// Initialisation
				crash_detecte = 0;
	
				// Tant que le Thread Fonctionnel n'est pas mort
				while(crash_detecte == 0){
					cout << "From " << myName << " : " << "Nothing to report !" << endl;
					/// FAILURE DETECTOR
					
					crash_detecte=research_crash(arg->Sservice, arg->myCvector, arg->horloge_sat, p_date_watchdog, delai_WD);
					
					//cout<<"crash detecte: "<<crash_detecte<<endl;
					
					/// Réception des CHECKPOINT
					receive_checkpoints(arg->Qservice, arg->myCvector, arg->manager_sat, myName);

					}

					// ACTIONS DE RECOUVREMENT
					if(crash_detecte == 1){ 
						
						cout << endl << "From " << myName << " : " << "CRASH DETECTED !!" << endl;
						cout << "From " << myName << " : " << "Beginning recovery !!" << endl;
						//reactivate_watchdog(arg->Sservice, arg->myCvector, arg->horloge_sat);	
						// Recuperation plan

						if (pthread_create(thread_recovery, thread_attributes, f_thread_recovery,(void *) &arguments_recovery) != 0) 
						perror ("<SLAVE> ERROR while creating recovery thread !");
						
						retour_plan = plan.set_plan_recouvrement();		
						// -1 plan par defaut, 0 si nouveau et 1 si on garde l'ancien plan
						
						switch(retour_plan){
				
						case -1:
							cout << "From " << myName << " : " << "ERROR while opening current plan." << endl;
						break;
				
						case 1:
							cout << "From " << myName << " : " << "Recovery plan loaded" << endl;
						break;

				
						default:
				
						break;
						}
						
						// Recuperation informations check point
						dernier_check = arg->manager_sat->recuperer_dernier_check();
						numero_derniere_action = dernier_check.get_id_action();
                                              						
						action_en_cours = plan.get_action(numero_derniere_action);	
				
						//int numero_plan = table.get_num_plan_courant();
				
						//cout << "From " << myName << " : " << " Last checkpoint received : Action N°" << numero_derniere_action << " \tStates " << etat_derniere_action << endl;
				
						if(numero_derniere_action>0){
				
						// Recuperation du type d'action
						const char* type_action = action_en_cours.nature.c_str();
				
						// ## ACTION IMG
						if(strcmp(type_action, "IMG") == 0){
						
							r_image(arg->manager_sat, arg->horloge_sat, arg->camera_sat, action_en_cours,numero_derniere_action, plan,dernier_check);

						} 
						
						
						// ## ACTION TSF
						else if(strcmp(type_action, "TSF") == 0){

							r_TSF(arg->manager_sat, arg->horloge_sat, action_en_cours);
											
				
						} 
				
						}
					
						plan.set_id_action_courante(numero_derniere_action-1); //TODO -1 ou +1 ?
						arg->manager_sat->init_plan(plan);

			
						// Passage en mode PRIMARY
						cout << "From " << myName << " : " << " End of recovery." << endl;
						cout << "From " << myName << " : " << " Changing mode to PRIMARY." << endl << endl;
						arg->manager_sat->set_mode("p");
						arg->mode_fonctionnement = "p";
						mode_partition = "p";
						
						//Problem on the RasPI with : pthread_cancel(*thread_recovery);
						//==> Use of bool variable instead
						flag_cancel_thread = true;
					}
					
				}
			//}
		//}
	
	
	} // FIN while(true)
	
    	cout << "From " << myName << " : End of non-functional thread." << endl;
	
	return NULL;

}


/* Displays contents of the communication vector */
void display_communication(COMMUNICATION_VECTOR* myCvector){

	cout << endl << "########## COMMUNICATION TABLE #########" << endl;
	
	cout << "QUEUING\t\t|SAMPLING" << endl;
	cout << "Port\t|Socket\t|Port\t|Socket" << endl;
	for (int i=0;i<MAX_COMM_SIZE;i++) cout << myCvector->vqueuing_port[i] << "\t|" << myCvector->vqueuing_socket[i] << "\t|" << myCvector->vsamp_port[i] << "\t|" << myCvector->vsamp_socket[i]<< endl;
	cout << "----------------------------------------------------------------------" << endl << endl;


}


int main(int argc, char *argv[])
	{
	
	string mode="c";
	int pid_partition;
	int redemarrage=atoi(argv[6]);
	int position=atoi(argv[5]);

	pid_partition = GUI_ARINC_partition("Master",position, redemarrage);

	//For future processing
	if(pid_partition){
	
	}

	// Objets de gestion du satellite
	Analyseur analyseur;
	//Camera camera;
	Horloge horloge;
	//SCAO scao;
	Memoire_stable memoire_stable;	
	Manager manager;
	CarteComm carte_communication;
	CarteRecep carte_reception;
		

	// Variables liees aux processus
	pthread_attr_t *thread_attributes;
	pthread_t *thread_F, *thread_NF;
	

	// Objets de communications
	COMMUNICATION_VECTOR myCvector;
	CQueuing Qservice; /* Communication Queuing */
	CSampling Sservice; /* Communication Sampling */
	
	// Configuration du vecteur de communication
	myCvector= init_communication(argc, argv,&mode);
	// Affichage du vecteur de communication
	//display_communication(&myCvector);
	size_t size = myCvector.emetteur.size() + 1;
   	char * nom_machine = new char[ size ];
	
        // copier la chaîne
    	strncpy( nom_machine, myCvector.emetteur.c_str(), size );

        //scao.init_comm(argc, nom_machine, myCvector, &horloge);
	//camera.init_cam(&Qservice,&Sservice,myCvector,&horloge);

        
	struct Arg_Manager arguments;
	struct Arg_Non_Fonctionnel argumentsNF;
	
	//argumentsNF.scao_sat=&scao;
	//argumentsNF.camera_sat=&camera;
	argumentsNF.mem_stable_sat=&memoire_stable;
	argumentsNF.analyseur_sat=&analyseur;
	argumentsNF.Sservice=Sservice;
	argumentsNF.Qservice = Qservice;
	argumentsNF.myCvector=myCvector;
	argumentsNF.myArgumentc = argc;
	argumentsNF.horloge_sat=&horloge;
	argumentsNF.manager_sat=&manager;
	argumentsNF.carte_communication_sat=&carte_communication; 
	argumentsNF.mode_fonctionnement = mode;
	 
	//arguments.scao_sat=&scao;
	arguments.Qservice=Qservice;  
	//arguments.camera_sat=&camera;
	arguments.mem_stable_sat=&memoire_stable;
	arguments.myCvector=myCvector;
	arguments.analyseur_sat=&analyseur;
	arguments.horloge_sat=&horloge;
	arguments.myArgumentc = argc;
	arguments.manager_sat=&manager;
	arguments.mode_fonctionnement = mode;
	arguments.carte_communication_sat=&carte_communication;
	arguments.carte_reception_sat = &carte_reception; 

	argumentsNF.myArgumentv = nom_machine;
	arguments.myArgumentv = nom_machine;

	cout << "Partition mode (p: primary and b: back-up): "<< mode << endl;
	/*for (int k=0; k<=argc; k++)
		{
		argumentsNF.myArgumentv[k]=argv[k];
		}
	
         for (int k=0; k<=argc; k++)
		{
		arguments.myArgumentv[k]=argv[k];
		}	*/	
	
	// Creation du thread Fonctionnel

	thread_attributes=(pthread_attr_t *)malloc(sizeof(pthread_attr_t));
	thread_F=(pthread_t *)malloc(sizeof(pthread_t));

	pthread_attr_init(thread_attributes);
	if (pthread_create(thread_F, thread_attributes, f_thread_F,(void *) &arguments) != 0) 
		perror ("<Manager> ERROR while creating functional thread !");

	// Creation du thread Non Fonctionnel


	thread_attributes=(pthread_attr_t *)malloc(sizeof(pthread_attr_t));
	thread_NF=(pthread_t *)malloc(sizeof(pthread_t));

	pthread_attr_init(thread_attributes);
	if (pthread_create(thread_NF, thread_attributes, f_thread_NF,(void *) &argumentsNF) != 0)  
		perror ("<Manager> ERROR while creating non-functional thread !");
		

	while (1) { usleep(500000);}  /* DOES NOTHING */ ;
	
		
	signal(SIGINT, (sig_t)bye);

	return 0;
	


	}



