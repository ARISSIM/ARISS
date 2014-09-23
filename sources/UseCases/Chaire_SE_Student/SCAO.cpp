#include "SCAO.h"

SCAO::SCAO(){

	//cout << "<SCAO> Construction SCAO." << endl;	
	
}

void SCAO::init_comm(int nbarg, char* arg, COMMUNICATION_VECTOR myCvector_sat, Horloge* horloge_sat){ // TODO : vérifier qu'on réinitialise bien quand le SLAVE prend la main

	myCvector = myCvector_sat;
	myArg=arg;
	myNbarg=nbarg;
	horloge=horloge_sat;
}

int SCAO::orienter_satellite(float command_pitch, float command_roll, float command_yaw, double date_action, double timeout){

		double date_actuelle,timeout_end;
		float command_yaw_recue,command_pitch_recue;
		bool orientation_effectuee=false;
		int ifmsg=0;	
		Type_Message msg_recu;
		const char* message_recu;
		char message[256];
                
                if (horloge->get_temps() < date_action-timeout) timeout_end = timeout+horloge->get_temps();
                else timeout_end = date_action;
		
		orientation_effectuee=false;
		
	//envoi du message à la partition SCAO (selection du bon socket selon la partition)	
		sprintf(message,"%f %f %g",command_yaw,command_pitch, date_action);
		//cout << "<SCAO> Sending orientation " << message << " to SCAO on sampling port " << myCvector.vsamp_port[2] << " with socket " << myCvector.vsamp_socket[2] << "..." << endl;
		Sservice.WRITE_SAMPLING_MESSAGE(myArg, myCvector.vsamp_port[2], myCvector.vsamp_socket[2], myCvector.emetteur, message);

	//attente de la réponse tant que la date de l'action n'est pas passee

                date_actuelle=horloge->get_temps();
                
		while( (date_actuelle<timeout_end) && (!orientation_effectuee) ){

		// Lecture du port queuing
			//cout << "<SCAO> Waiting for report from SCAO at queuing port with socket " << myCvector.vqueuing_socket[2] << ". (" << date_action-date_actuelle << "s last)" << endl;
			ifmsg=Qservice.READ_QUEUING_MESSAGE(myCvector.vqueuing_socket[2]);

			if(ifmsg>0){

			// Lecture du message		
				msg_recu = Qservice.get_Message();
				message_recu = msg_recu.m_message;
			
			// Extraction de données du checkpoint à partir du message
				sscanf(message_recu,"%f %f", &command_yaw_recue, &command_pitch_recue);			
				//cout << "<SCAO> Report received from SCAO : " << message_recu << "." << endl;
			
			// Verification que le message est bien celui attendu
				if((command_yaw_recue==command_yaw)&&(command_pitch_recue==command_pitch)){

				// Si c'est le message attendu, on quitte le while
					orientation_effectuee=true;
				}

			
								
		 	}
		 	
		 	//actualisation heure actuelle
		 	date_actuelle=horloge->get_temps();

		}
	
	//selection de la valeur de retour
		if(orientation_effectuee==true){
			return 1;
		}

		else{
			return 0;
		}
}
