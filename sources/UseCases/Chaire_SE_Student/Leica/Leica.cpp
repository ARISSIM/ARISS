#include "include/CBasefunction.h"

// INCLUSION OF OBJECTS OF SATELLITE
#include "../Analyseur.h"
#include "../Horloge.h"
#include "../Camera.h"
#include "../Memoire_stable.h"
#include "../Checkpoint.h"
#include "../Manager.h"
#include "../CarteComm.h"

// INCLUSION OF OTHER OBJECTS
#include "../RebootFlag.h"

using namespace std;

// DEFINITION OF CONSTANTS
#define DUREE_DEFAUT 1 /* durée de prise d'image par défaut*/
#define CAPTOR_LENGTH 1944 
#define LARGEUR_DEFAUT 2592 
#define LARGEUR_MIN 200
#define IMAGE_ID_LENGTH 30
#define ORDER_NATURE_LENGTH 10
#define DUREE_MIN 1
#define DUREE_MAX 10
#define DEFAULT_QUALITY 75
#define MAX_COMM_SIZE 4
#define NUMBER_EARTH_IMAGES 50

#define TIMEOUT_ANALYSEUR 10
#define TIMEOUT_CAMERA 10
#define TIMEOUT_COMM 10

#define GEOLOCATION_DELAY 2
#define DEFAULT_LATITUDE 0


// NAME OF THE PARTITION
char myName[10] = "LEICA";


// STRUCTURE REPRESENTING ORDER FROM MANAGERS
struct order{

        char nature[ORDER_NATURE_LENGTH];
	char id_image[IMAGE_ID_LENGTH]; /* identifiant de l'image */
        double order_date;
	int larg_px; /* hauteur en pixels */
	double duration; /* duration of the action */
	int qualite; /* qualité de l'image à capturer */

};

struct geolocation{

	char LatitudeRef[1];
	int Latitude[6];
	char LongitudeRef[1];
	int Longitude[6];

};

	
// SUBFUNCTIONS OF PARTITION LEICA


/* Copy an order in another one */
void set_order(order* ordre1, order* ordre2){

        strncpy(ordre1->nature,ordre2->nature,ORDER_NATURE_LENGTH);
	strncpy(ordre1->id_image,ordre2->id_image,IMAGE_ID_LENGTH);
        ordre1->order_date = ordre2->order_date;
	ordre1->larg_px = ordre2->larg_px;
	ordre1->duration = ordre2->duration;
	ordre1->qualite = ordre2->qualite;

}


/* Setup an order with parameters */
void set_order(order* ordre_a_creer, char* nature, char* id_image, double order_date, int larg_px, double duree, int qualite){

        strncpy(ordre_a_creer->nature,nature,ORDER_NATURE_LENGTH);
        strncpy(ordre_a_creer->id_image,id_image,IMAGE_ID_LENGTH);
        ordre_a_creer->order_date = order_date;
	ordre_a_creer->larg_px = larg_px;
	ordre_a_creer->duration = duree;
	ordre_a_creer->qualite = qualite;

}

/* Initialiser rapidement un ordre à des valeurs par défaut */
void init_order(order* ordre_a_initialiser){

	char default_image_id[IMAGE_ID_LENGTH] = "INITIAL_VALUE";
        char default_order_type[ORDER_NATURE_LENGTH] = "IMG";

	set_order(ordre_a_initialiser, default_order_type, default_image_id, 0, LARGEUR_DEFAUT, DUREE_DEFAUT, DEFAULT_QUALITY);

}


/* Recevoir un ordre du Manager par la lecture du port SAMPLING */
order receive_order(CSampling* Sservice, COMMUNICATION_VECTOR* myCvector, int partition, RebootFlag* rebootflag){

	/* Variables liées à la réception du message */
	int ifmsg=0;
	const char* message_utile;
	Type_Message message_lu;

	/* Variables liées à la lecture du message */
	order ordre_lu;
	init_order(&ordre_lu);

	
	// LECTURE DU PORT SAMPLING

	//cout << "From " << myName << " : Reading message from partition " << partition << " on sampling port with socket " << myCvector->vsamp_socket[partition] << "..." << endl;

	ifmsg=Sservice->READ_SAMPLING_MESSAGE(myCvector->vsamp_socket[partition]);
	
		
	// Traitement du message lu
	if(ifmsg>0){

		// Récupération du message
		message_lu = Sservice->get_Message();
		message_utile = message_lu.m_message;

		//cout << "From " << myName << " : Message received -> " << message_utile << endl;
		
		// Conversion du message en ordre à exécuter
		sscanf(message_utile,"%s %s %lf %d %lf %d", ordre_lu.nature, ordre_lu.id_image, &(ordre_lu.order_date), &(ordre_lu.larg_px), &(ordre_lu.duration), &(ordre_lu.qualite));
	
	} /*else {
		//cout << "From " << myName << " : Still no order received from partition " << partition << "." << endl;
		rebootflag->inc();
	
	}*/

	return ordre_lu;

}


int receive_geolocation(geolocation* still_geolocation, CSampling* Sservice, COMMUNICATION_VECTOR* myCvector, Horloge* clock, double date_action){ 
//Receives the sampling message of geoloc sent by scao partition to create the exif file. Return 1 if geolocalisation is OK or 0 if it's not.
	
	int ifmsg=0;	
	Type_Message msg_recu;
	const char* message_recu;
	int ret;
	double date_geoloc;
	double reading_date = clock->get_temps();

	while( ret !=1 && (clock->get_temps() < (reading_date + GEOLOCATION_DELAY)) ){

		ifmsg=Sservice->READ_SAMPLING_MESSAGE(myCvector->vsamp_socket[2]); //Read sampling message from scao partition

		// Message reception
		if(ifmsg>0){

			// Saving message
			msg_recu = Sservice->get_Message();
			message_recu = msg_recu.m_message;
		
			// Treatment of the message
			sscanf(message_recu,"%d/%d,%d/%d,%d/%d %s %d/%d,%d/%d,%d/%d %s %lf", 
			&((still_geolocation->Latitude)[0]),
			&(still_geolocation->Latitude[1]),
			&(still_geolocation->Latitude[2]),
			&(still_geolocation->Latitude[3]),
			&(still_geolocation->Latitude[4]),
			&(still_geolocation->Latitude[5]),
			still_geolocation->LatitudeRef,
			&(still_geolocation->Longitude[0]), 
			&(still_geolocation->Longitude[1]),
			&(still_geolocation->Longitude[2]),
			&(still_geolocation->Longitude[3]),
			&(still_geolocation->Longitude[4]),
			&(still_geolocation->Longitude[5]),
			still_geolocation->LongitudeRef,
			&(date_geoloc));

			if (date_geoloc < reading_date+GEOLOCATION_DELAY || date_geoloc > reading_date+GEOLOCATION_DELAY){
				//cout << "From " << myName << " : Geolocation received from SCAO. (" << message_recu << ")" << endl;
				cout << "From " << myName << " : Geolocation received from SCAO." << endl;
				ret=1;
			} else { //enable to geolocalise the image : geolocation is too old
				cout << "From " << myName << " : ERROR. Bad geolocation received from SCAO." << endl;
				ret=0;
			}
		
		} else { //no geolocation possible
			ret=0;
			cout << "From " << myName << " : ERROR. Sampling port from SCAO is empty." << endl;
		}

	}
	
	return ret;

}


void mark_geolocation(char* still_to_mark, geolocation still_geolocation, char* machine_name){

	char command[300] = "\0";

	/* If on Raspberry PI */
	if (!strcmp(machine_name,"raspberryPI-TSF")){

		// Setting Latitude Reference
		sprintf(command, "exif camera_memory/%s --ifd=GPS --tag=0x1 --set-value=%s -o camera_memory/%s", still_to_mark, still_geolocation.LatitudeRef, still_to_mark);
		//cout << "From " << myName << " Marking tag 1 : " << command << endl;
		system(command);

		// Setting Latitude
		for (unsigned int i=0; i < 300; i++) command[i] = '\0';
		sprintf(command, "exif camera_memory/%s --ifd=GPS --tag=0x2 --set-value='%d %d %d %d %d %d' -o camera_memory/%s", 
		still_to_mark,
		still_geolocation.Latitude[0],
		still_geolocation.Latitude[1],
		still_geolocation.Latitude[2],
		still_geolocation.Latitude[3],
		still_geolocation.Latitude[4],
		still_geolocation.Latitude[5],
		still_to_mark);
		//cout << "From " << myName << " Marking tag 2 : " << command << endl;
		system(command);


		// Setting Longitude Reference
		for (unsigned int i=0; i < 300; i++) command[i] = '\0';
		sprintf(command, "exif camera_memory/%s --ifd=GPS --tag=0x3 --set-value=%s -o camera_memory/%s", still_to_mark, still_geolocation.LongitudeRef, still_to_mark);
		//cout << "From " << myName << " Marking tag 3 : " << command << endl;
		system(command);

		// Setting Longitude
		for (unsigned int i=0; i < 300; i++) command[i] = '\0';
		sprintf(command, "exif camera_memory/%s --ifd=GPS --tag=0x4 --set-value='%d %d %d %d %d %d' -o camera_memory/%s", 
		still_to_mark,
		still_geolocation.Longitude[0],
		still_geolocation.Longitude[1],
		still_geolocation.Longitude[2],
		still_geolocation.Longitude[3],
		still_geolocation.Longitude[4],
		still_geolocation.Longitude[5],
		still_to_mark);
		//cout << "From " << myName << " Marking tag 4 : " << command << endl;
		system(command);

	/* If on another PC */
	} else {


	}

	cout << "From " << myName << " : Geolocation marked on " << still_to_mark << "." << endl;

}


/* Reports order status to Managers */
void report_capture(char* id_image, bool status[5], char* machine_name, CQueuing* Qservice, COMMUNICATION_VECTOR* myCvector){

	char message[30]="\0";

	sprintf(message,"%d %d %d %d %d %s",status[0],status[1],status[2],status[3],status[4],id_image);

	cout << "From " << myName << " : Sending report to Managers. (" << message << ")" << endl;

	//cout << "From " << myName << " : Sending to MASTER at queuing port " << myCvector->vqueuing_port[0] << " with socket " << myCvector->vqueuing_socket[0] << "..." << endl;
	Qservice->WRITE_QUEUING_MESSAGE(machine_name, myCvector->vqueuing_port[0], myCvector->vqueuing_socket[0], myCvector->emetteur, message);
	
	//cout << "From " << myName << " : Sending to SLAVE at queuing port " << myCvector->vqueuing_port[1] << " with socket " << myCvector->vqueuing_socket[1] << "..." << endl;
	Qservice->WRITE_QUEUING_MESSAGE(machine_name, myCvector->vqueuing_port[1], myCvector->vqueuing_socket[1], myCvector->emetteur, message);
	
	cout << "From " << myName << " : Report sent." << endl;

}

/* Reports order status to Managers */
void report_transfer(char* id_image, bool status, char* machine_name, CQueuing* Qservice, COMMUNICATION_VECTOR* myCvector){

	char message[30]="\0";

	sprintf(message,"%d %s",status,id_image);

	cout << "From " << myName << " : Sending report to Managers. (" << message << ")" << endl;

	//cout << "From " << myName << " : Sending to MASTER at queuing port " << myCvector->vqueuing_port[0] << " with socket " << myCvector->vqueuing_socket[0] << "..." << endl;
	Qservice->WRITE_QUEUING_MESSAGE(machine_name, myCvector->vqueuing_port[0], myCvector->vqueuing_socket[0], myCvector->emetteur, message);
	
	//cout << "From " << myName << " : Sending to SLAVE at queuing port " << myCvector->vqueuing_port[1] << " with socket " << myCvector->vqueuing_socket[1] << "..." << endl;
	Qservice->WRITE_QUEUING_MESSAGE(machine_name, myCvector->vqueuing_port[1], myCvector->vqueuing_socket[1], myCvector->emetteur, message);
	
	cout << "From " << myName << " : Report sent." << endl;

}

/* Obtenir la largeur d'une image à partir de la durée de prise de vue */
int duree2largeur(double duree){

	int largeur = LARGEUR_DEFAUT;

	if (duree > DUREE_MAX) duree = DUREE_MAX;
	if (duree < DUREE_MIN) duree = DUREE_MIN;

	largeur = ((duree - DUREE_MIN) / (DUREE_MAX - DUREE_MIN))*(LARGEUR_DEFAUT - LARGEUR_MIN) + LARGEUR_MIN;

	return largeur;
	
}


/* Displays an order */
void display_order(order* ordre_a_afficher){

	cout << "From " << myName << " : <Order> Still " << ordre_a_afficher->id_image
	<< " at quality " << ordre_a_afficher->qualite
	<< " during " << ordre_a_afficher->duration
	<< "s (resolution : " << ordre_a_afficher->larg_px
	<< "x" << CAPTOR_LENGTH
	<< ")." << endl;

}


/* Executes a capture order */
int capture(order* ordre_a_executer, char* machine_name){

	char commande[500]="\0";
	int status = 0;
	int image_number = 0;
	sscanf(ordre_a_executer->id_image,"%d.jpg",&image_number);

	// Display order to be execute
	display_order(ordre_a_executer);
	

	// Effacement de la memoire locale de la camera (dossier "camera_memory")
	for(int i = 0; i < 500; i++) commande[i] = '\0';
	sprintf(commande,"rm -f camera_memory/*");
	system(commande);
	
	//cout << "From " << myName << " : Preparing capture..." << endl;

	// Prise de l'image
	for(int i = 0; i < 500; i++) commande[i] = '\0';
	if (!strcmp(machine_name,"raspberryPI-TSF")){

		sprintf(commande, "raspistill -t %d -o camera_memory/%s -q %d -h %d -w %d --exif GPS.GPSLatitudeRef=S --exif GPS.GPSLatitude=1/1,1/1,1/1 --exif GPS.GPSLongitudeRef=W --exif GPS.GPSLongitude=1/1,1/1,1/1", 
		DUREE_DEFAUT,
		ordre_a_executer->id_image,
		(ordre_a_executer->qualite)%101,
		CAPTOR_LENGTH,
		LARGEUR_DEFAUT); // normalement duree2largeur(ordre_a_executer.duree);
		
	} else { // compatibilité du code sur PC
	
		sprintf(commande, "cp ../../sources/Chaire_SE_Student/TERRE/%d.jpg camera_memory/%s", 
		(image_number%(NUMBER_EARTH_IMAGES)+1), ordre_a_executer->id_image);
	
	}

	system(commande);

	//usleep(((ordre_a_executer->duree)%3)*1000000);

	cout << "From " << myName << " : Still " << ordre_a_executer->id_image << " captured." << endl;
	
	status = 1; // TODO : prévoir erreurs
	
	
	return status;
}

/* Displays a still */
void display(char* id_image, char* machine_name){

	char cmde[250]="\0";

	if (!strcmp(machine_name,"raspberryPI-TSF")){
	
		system("sudo killall -q gpicview");

		sprintf(cmde, "gpicview camera_memory/%s &", id_image);
		system(cmde);

	} else {

		system("killall -q display");

		sprintf(cmde, "display camera_memory/%s &", id_image);
		system(cmde);
	
	}

}

/*
int still_process(char* still_to_process){

	int image_number = 0;
	int process_status = 0;

	sscanf(ordre_a_traiter.id_image,"%d.jpg",&image_number);
	
	if (image_number%10 == 0){
		status = 1;
		cout << "<Analyseur> Processing of image " << still_to_process << " correct." << endl;
	}
	else{
		status = 0;
		cout << "<Analyseur> Processing of image " << still_to_process << " incorrect !!!" << endl;
	}


	return process_status;

}


int still_compress(char* still_to_compress){

	int compress_status = 0;
	char cmd[200] = "\0";
	
	// Compression
	for (int i=0; i<100; i++) cmd[i] = '\0';
	sprintf(cmd, "zip -jq camera_memory/%s.zip camera_memory/%s", 
	still_to_compress, still_to_compress);
	system(cmd);
	
	// Suppression de l'image non compressee
	for (int i=0; i<100; i++) cmd[i] = '\0';
	sprintf(cmd, "rm -f camera_memory/%s", still_to_compress);
	system(cmd);

	compress_status = 1; //TODO : gérer erreurs

	return compress_status;

}
*/


void execute_capture(CQueuing* Qservice, CSampling* Sservice, COMMUNICATION_VECTOR* myCvector, char* machine_name, Horloge* clock, Analyseur* analyzer, Memoire_stable* stable_memory, order* capture_order, order* last_order_P, order* last_order, RebootFlag* rebootflag){

	// CAPTURE RELATED VARIABLES
	int capture_status = 0, process_status = 0, save_status = 0, geolocation_status = 0;
        bool order_status[5] = {false,false,false,false,false};
	geolocation still_geolocation;

        // Waiting date for capturing still
        cout << "From " << myName << " : " << " Waiting for capturing still (" << capture_order->order_date - clock->get_temps() << " seconds)." << endl;
        do {
            usleep(10000);
        } while (clock->get_temps() < capture_order->order_date);
        
        cout << "From " << myName << " : " << " Beginning capture." << endl;
       
        capture_status = capture(capture_order, machine_name);

        // Processing still if capture is done
        if (capture_status == 1){

                order_status[0] = true;

                // Displaying still
                display(capture_order->id_image, machine_name);

                // Processing still
                process_status = analyzer->traiter_image(capture_order->id_image);
                if (process_status == 1){
                        order_status[2] = true;

                } else {
                        order_status[2] = false;
                        cout << "From " << myName << " : " << "ERROR while processing still " << capture_order->id_image << "." << endl;
                        rebootflag->inc();
                }
                order_status[1] = true; // TODO: le traitement ne s'est peut-être pas fait

		// Receiving and marking geolocation
		geolocation_status = receive_geolocation(&still_geolocation, Sservice, myCvector, clock, capture_order->order_date);
		if (geolocation_status == 1) mark_geolocation(capture_order->id_image, still_geolocation, machine_name);
		else rebootflag->inc();

                // Compressing still
                analyzer->compresser_image(capture_order->id_image);

                // Saving still
                save_status = stable_memory->stocker(capture_order->id_image);
                if (save_status == 1){
                        order_status[3] = true;
                } else {
                        order_status[3] = false;
                        cout << "From " << myName << " : " << "ERROR while saving still " << capture_order->id_image << "." << endl;
                        rebootflag->inc();
                }

        } else {

                order_status[0] = false;
                cout << "From " << myName << " : " << "ERROR while capturing still " << capture_order->id_image << "." << endl;
                rebootflag->inc();

        }
        
        // Setting last orders
        set_order(last_order_P,capture_order);
        set_order(last_order,capture_order);

        // Reporting order status to Managers
        report_capture(capture_order->id_image, order_status, machine_name, Qservice, myCvector);

}

/* Compares two orders and returns 1 if they are equal */
bool compare_orders(order* order1, order* order2){
    
    return (!strcmp(order1->nature,order2->nature) && !strcmp(order1->id_image,order2->id_image));
    
}


void execute_transfer(order* transfer_order, CarteComm* communication_card, CQueuing* Qservice, COMMUNICATION_VECTOR* myCvector, char* machine_name, Memoire_stable* stable_memory, Horloge* clock, RebootFlag* rebootflag){

	bool card_status = false, transfer_status = false;

	card_status = communication_card->activer_carte(); // TODO : prévoir défaillance de la carte ?
	if (card_status == true){
            
            // 
            if ( (clock->get_temps() < (transfer_order->order_date + transfer_order->duration)) ){
                

                transfer_status = communication_card->envoyer(transfer_order->id_image, machine_name);
                if (!transfer_status){
                    
                    cout << "From " << myName << " : " << " ERROR while transmitting still " << transfer_order->id_image << "..." << endl;
                    rebootflag->inc();
                    // TODO: gerer soucis
                    
                }

                stable_memory->vider(transfer_order->id_image);
                
                report_transfer(transfer_order->id_image, transfer_status, machine_name, Qservice, myCvector);
    
            } else { cout << "From " << myName << " : " << "ERROR : Transfer limit time exceeded. Ground station out of visibility." << endl; rebootflag->inc(); }
            
            
            
        } else {
            
            cout << "From " << myName << " : " << " ERROR while activating communication card..." << endl;
            rebootflag->inc();
         
        }
        
        communication_card->desactiver_carte();

}


sig_t bye()
{
  cout << "From " << myName <<  " : Bye !" << endl;
  exit(0);
}


void leica_manager(CarteComm* communication_card, CQueuing* Qservice, CSampling* Sservice, COMMUNICATION_VECTOR* myCvector, char* machine_name, Camera* camera, Analyseur* analyzer, Memoire_stable* stable_memory, Horloge* clock, order* dernier_ordre_M, order* dernier_ordre_S, order* dernier_ordre, RebootFlag* rebootflag){
    
    	// CAPTURE RELATED VARIABLES
	order ordre_recu_M, ordre_recu_S;
	char default_image_name[IMAGE_ID_LENGTH] = "INITIAL_VALUE";
        char type_IMG[ORDER_NATURE_LENGTH] = "IMG", type_TSF[ORDER_NATURE_LENGTH] = "TSF";

	// INITIALIZATION OF ORDER RELATED VARIABLES
	init_order(&ordre_recu_M);
	init_order(&ordre_recu_S);

	cout << endl << "From " << myName << " : Waiting for orders..." << endl;

	// LISTENING MASTER SAMPLING PORT FOR ORDER
	ordre_recu_M = receive_order(Sservice, myCvector, 0, rebootflag);
                
        
        if (!compare_orders(&ordre_recu_M,dernier_ordre_M) && !compare_orders(&ordre_recu_M, dernier_ordre) && strcmp(ordre_recu_M.id_image, default_image_name)){
            
            
            if (!strcmp(ordre_recu_M.nature, type_IMG)) {
                
                cout << "From " << myName << " : New capture order received from MASTER. Execution..." << endl;
                execute_capture(Qservice, Sservice, myCvector, machine_name, clock, analyzer, stable_memory, &ordre_recu_M, dernier_ordre_M, dernier_ordre, rebootflag);
                
            } else if (!strcmp(ordre_recu_M.nature, type_TSF)) {

                cout << "From " << myName << " : New transfer order received from MASTER. Execution..." << endl;
                execute_transfer(&ordre_recu_M, communication_card, Qservice, myCvector, machine_name, stable_memory, clock, rebootflag);

            }
            
        }
        else {
            
            // LISTENING SLAVE SAMPLING PORT FOR ORDER
            ordre_recu_S = receive_order(Sservice, myCvector, 1, rebootflag);
            
            if (!compare_orders(&ordre_recu_S,dernier_ordre_S) && !compare_orders(&ordre_recu_S, dernier_ordre) && strcmp(ordre_recu_S.id_image, default_image_name)){
            
                if (!strcmp(ordre_recu_S.nature, type_IMG)) {
                
                    cout << "From " << myName << " : New capture order received from SLAVE. Execution..." << endl;
                    execute_capture(Qservice, Sservice, myCvector, machine_name, clock, analyzer, stable_memory, &ordre_recu_S, dernier_ordre_S, dernier_ordre, rebootflag);
                    
                } else if (!strcmp(ordre_recu_S.nature, type_TSF)) {
                    
                    cout << "From " << myName << " : New transfer order received from SLAVE. Execution..." << endl;
                    execute_transfer(&ordre_recu_S, communication_card, Qservice, myCvector, machine_name, stable_memory, clock, rebootflag);
                    
                }
                
            }
            
        }
    
}


/* Displays contents of the communication vector */
void display_communication(COMMUNICATION_VECTOR* myCvector){

	cout << endl << "########## COMMUNICATION TABLE #########" << endl;
	
	cout << "QUEUING\t\t|SAMPLING" << endl;
	cout << "Port\t|Socket\t|Port\t|Socket" << endl;
	for (int i=0;i<MAX_COMM_SIZE;i++) cout << myCvector->vqueuing_port[i] << "\t|" << myCvector->vqueuing_socket[i] << "\t|" << myCvector->vsamp_port[i] << "\t|" << myCvector->vsamp_socket[i]<< endl;
	cout << "----------------------------------------------------------------------" << endl << endl;


}


// FONCTION PRINCIPALE
int main(int argc, char *argv[])
{
	string mode="c";
	int pid_partition;
	int redemarrage=atoi(argv[7]);
	
	int position=atoi(argv[6]);
	
	pid_partition=GUI_ARINC_partition("Leica",position, redemarrage);

	// OBJETS DE COMMUNICATIONS
	CQueuing Qservice; /* Communication Queuing */
	CSampling Sservice; /* Communication Sampling */
	COMMUNICATION_VECTOR myCvector; /* Vecteur de communication */

	// Configuration du vecteur de communication
	myCvector= init_communication(argc, argv,&mode);
	size_t size = myCvector.emetteur.size() + 1;
   	char * machine_name  = new char[ size ];
        // copier la chaîne
    	strncpy( machine_name , myCvector.emetteur.c_str(), size );


	// Affichage du vecteur de communication
	//display_communication(&myCvector);

	// Variables conservant les derniers ordres reçus
	order dernier_ordre_M, dernier_ordre_S, dernier_ordre;
	
	// Initialisation des variables de dernier ordre
	init_order(&dernier_ordre_M);
	init_order(&dernier_ordre_S);
	init_order(&dernier_ordre);

	// OBJETS DU SATELLITE
	Camera camera;
	Analyseur analyzer;
        Memoire_stable stable_memory;
	CarteComm communication_card;
        Horloge clock;
        
        // OTHER OBJECTS
	RebootFlag rebootflag;
	rebootflag.set_flag_limit(100000);
	rebootflag.set_partition_pid(pid_partition);

	//while (rebootflag.flag()) {
	while (true) {

                leica_manager(&communication_card, &Qservice, &Sservice, &myCvector, machine_name, &camera, &analyzer, &stable_memory, &clock, &dernier_ordre_M, &dernier_ordre_S, &dernier_ordre, &rebootflag);

		usleep(10000); // attente de 10ms (vérification 25 fois par cycle)

	}
	
	rebootflag.reboot();
		
	signal(SIGINT, (sig_t)bye);


	return 0;

}



