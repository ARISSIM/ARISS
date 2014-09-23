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

using namespace std;

char myName[10] = "SCAO";

//PIN1 = Yaw
#define PIN1 17
//Yaw 0.05 - 0.024 - 0.123
#define INIT_PIN1 0.05
#define MIN_PIN1 0.025
#define MAX_PIN1 0.12

//PIN2 = Pitch
#define PIN2 23
//Pitch 0.045 - 0.035 - 0.115
#define INIT_PIN2 0.045
#define MIN_PIN2 0.035
#define MAX_PIN2 0.115

// Delay between the motion of the 2 motors
#define TEMPO_MOTEUR 75000

// Angle span
#define ANGLE_MIN_YAW 5
#define ANGLE_MAX_YAW 175
#define ANGLE_MIN_PITCH 5
#define ANGLE_MAX_PITCH 175

#define MAX_COMM_SIZE 4 /* for display of communication vector */

sig_t bye() {
    cout << "From " << myName << " : Goodbye !" << endl;
    exit(0);
}

struct geolocation {
    char LatitudeRef[1];
    int Latitude[6];
    char LongitudeRef[1];
    int Longitude[6];

};

struct orientation {
    float yaw; /* equivalent to french "lacet" */
    float pitch; /* equivalent to french "tangage" */
    float roll; /* equivalent to french "roulis" */

};


//Turn an angle (in degree) into a modulation width (*100 to obtain %)

float angle2pwm(int mode, float angle) {

    float ret;

    // mode = 1 ==> Yaw
    // mode = 1 ==> Pitch    

    if (mode == 1) ret = (angle - ANGLE_MIN_YAW) / (ANGLE_MAX_YAW - ANGLE_MIN_YAW)*(MAX_PIN1 - MIN_PIN1) + MIN_PIN1;
    if (mode == 2) ret = (angle - ANGLE_MIN_PITCH) / (ANGLE_MAX_PITCH - ANGLE_MIN_PITCH)*(MAX_PIN2 - MIN_PIN2) + MIN_PIN2;

    return ret;

}

//Turn a modulation width (*100 to obtain %) into an angle (in degree)

float pwm2angle(int mode, float pwm) {

    float ret;

    // mode = 1 ==> Yaw
    // mode = 1 ==> Pitch

    if (mode == 1) ret = (pwm - MIN_PIN1) / (MAX_PIN1 - MIN_PIN1)*(ANGLE_MAX_YAW - ANGLE_MIN_YAW) + ANGLE_MIN_YAW;
    if (mode == 2) ret = (pwm - MIN_PIN2) / (MAX_PIN2 - MIN_PIN2)*(ANGLE_MAX_PITCH - ANGLE_MIN_PITCH) + ANGLE_MIN_PITCH;

    return ret;

}

void init_orientation(orientation* orientation_to_init) {

    orientation_to_init->yaw = pwm2angle(0, INIT_PIN1);
    orientation_to_init->pitch = pwm2angle(1, INIT_PIN2);
    orientation_to_init->roll = 0;

}

/* Compares two orders and returns 1 if they are equal */
bool compare_orientation(orientation* orientation1, orientation* orientation2) {

    return (orientation1->yaw == orientation2->yaw && orientation1->pitch == orientation2->pitch && orientation1->roll == orientation2->roll);

}

/* Sets values of orientation structure with parameters */
void set_orientation(orientation* orientation_to_set, float yaw, float pitch, float roll) {

    orientation_to_set->yaw = yaw;
    orientation_to_set->pitch = pitch;
    orientation_to_set->roll = roll;

}

/* Sets values of orientation structure with another orientation structure */
void set_orientation(orientation* orientation_to_set, orientation* orientation_values) {

    orientation_to_set->yaw = orientation_values->yaw;
    orientation_to_set->pitch = orientation_values->pitch;
    orientation_to_set->roll = orientation_values->roll;

}



//TODO void ? return a value when an error happens ? How to detect errors ?

void orienter_moteur(orientation orientation_to_apply, char* machine_name) {

    char cmde[250] = "\0";


    // Placement en lacet
    cout << "From " << myName << " : Yaw set-up in progress ..." << endl;
    if (!strcmp(machine_name, "raspberryPI-TSF")) {
        sprintf(cmde, "echo \"%d=%f\" > /dev/pi-blaster", PIN1, angle2pwm(1, orientation_to_apply.yaw));
        system(cmde);
    }
    usleep(TEMPO_MOTEUR);

    //Placement en tangage
    cout << "From " << myName << " : Pitch set-up in progress ..." << endl;
    if (!strcmp(machine_name, "raspberryPI-TSF")) {
        for (int i = 0; i < 250; i++) cmde[i] = '\0';
        sprintf(cmde, "echo \"%d=%f\" > /dev/pi-blaster", PIN2, angle2pwm(2, orientation_to_apply.pitch));
        system(cmde);
    }
    usleep(TEMPO_MOTEUR);

    cout << "From " << myName << " : Satellite oriented @  [" << orientation_to_apply.yaw << " - " << orientation_to_apply.pitch << "] !" << endl;

    /*** "Arret des moteurs" ***/
    if (!strcmp(machine_name, "raspberryPI-TSF")) {
        // Arret en lacet
        for (int i = 0; i < 250; i++) cmde[i] = '\0';
        sprintf(cmde, "echo \"%d=0\" > /dev/pi-blaster", PIN1);
        system(cmde);
        //Arret en tangage
        for (int i = 0; i < 250; i++) cmde[i] = '\0';
        sprintf(cmde, "echo \"%d=0\" > /dev/pi-blaster", PIN2);
        system(cmde);
    }

}

/* Orients motors with initial values*/
void orienter_moteur(char* machine_name) {

    char cmde[250] = "\0";

    if (!strcmp(machine_name, "raspberryPI-TSF")) {
        //Terminaison des précédentes instances de pi-blaster
        system("sudo killall -q pi-blaster");

        //Lancement de Pi-Blaster
        system("sudo ./../../sources/Chaire_SE_Student/Scao/pi-blaster");
    }

    cout << "From " << myName << " : Satellite initial orientation in progress..." << endl;

    // Placement en lacet
    cout << "From " << myName << " : Yaw set-up in progress ..." << endl;
    if (!strcmp(machine_name, "raspberryPI-TSF")) {
        sprintf(cmde, "echo \"%d=%f\" > /dev/pi-blaster", PIN1, INIT_PIN1);
        system(cmde);
    }
    usleep(TEMPO_MOTEUR);

    //Placement en tangage
    cout << "From " << myName << " : Pitch set-up in progress ..." << endl;
    if (!strcmp(machine_name, "raspberryPI-TSF")) {
        for (int i = 0; i < 250; i++) cmde[i] = '\0';
        sprintf(cmde, "echo \"%d=%f\" > /dev/pi-blaster", PIN2, INIT_PIN2);
        system(cmde);
    }
    usleep(TEMPO_MOTEUR);

    cout << "From " << myName << " : Initial orientation reached ! ( Y = " << pwm2angle(1, INIT_PIN1) << " - P = " << pwm2angle(2, INIT_PIN2) << ")" << endl;

    /*** "Arret des moteurs" ***/
    if (!strcmp(machine_name, "raspberryPI-TSF")) {
        // Arret en lacet
        for (int i = 0; i < 250; i++) cmde[i] = '\0';
        sprintf(cmde, "echo \"%d=0\" > /dev/pi-blaster", PIN1);
        system(cmde);
        // Arret en tangage
        for (int i = 0; i < 250; i++) cmde[i] = '\0';
        sprintf(cmde, "echo \"%d=0\" > /dev/pi-blaster", PIN2);
        system(cmde);
    }
}

/* Gives satellite current geolocation */
void geolocate(geolocation* still_geolocation, double* geolocation_date, Horloge* clock) {

    still_geolocation->Latitude[0] = 40;
    still_geolocation->Latitude[1] = 1;
    still_geolocation->Latitude[2] = 33;
    still_geolocation->Latitude[3] = 1;
    still_geolocation->Latitude[4] = 4830;
    still_geolocation->Latitude[5] = 100;
    sprintf(still_geolocation->LatitudeRef, "N");
    still_geolocation->Longitude[0] = 1;
    still_geolocation->Longitude[1] = 1;
    still_geolocation->Longitude[2] = 28;
    still_geolocation->Longitude[3] = 1;
    still_geolocation->Longitude[4] = 3880;
    still_geolocation->Longitude[5] = 100;
    sprintf(still_geolocation->LongitudeRef, "E");

    *geolocation_date = clock->get_temps();

}

int receive_orientation(orientation* orientation_order, double* date_action, CSampling* Sservice, COMMUNICATION_VECTOR* myCvector, int partition) {

    // Variables liées à la communication
    int ret_mess = 0;
    char* mess_recu;
    Type_Message msg;

    // Variables liées aux ordres
    set_orientation(orientation_order, 0, 0, 0);
    *date_action = 0;

    //cout << "From " << myName << " : Reading message from partition " << partition << " at sampling port with socket " << myCvector.vsamp_socket[partition] << "..." << endl;
    ret_mess = Sservice->READ_SAMPLING_MESSAGE(myCvector->vsamp_socket[partition]);


    //Priorité au messages du MASTER
    if (ret_mess > 0) {

        // Recuperation message
        msg = Sservice->get_Message();
        mess_recu = msg.m_message;

        // Stockage des angles
        sscanf(mess_recu, "%f %f %lf", &(orientation_order->yaw), &(orientation_order->pitch), date_action);

        ret_mess = 1;

    } else ret_mess = 0;

    return ret_mess;

}

/* Reports geolocation to Leica */
void report_orientation(orientation orientation_order, char* machine_name, CQueuing* Qservice, COMMUNICATION_VECTOR* myCvector) {


    char message[100] = "\0";

    sprintf(message, "%f %f", orientation_order.yaw, orientation_order.pitch);

    cout << "From " << myName << " : Sending orientation report to Managers... ( @ [" << orientation_order.yaw << " - " << orientation_order.pitch << "] )" << endl;

    //cout<< "From " << myName << " : Sending report to MASTER at queuing port " << myCvector.vqueuing_port[0] << " with socket " << myCvector.vqueuing_socket[0] <<endl;
    Qservice->WRITE_QUEUING_MESSAGE(machine_name, myCvector->vqueuing_port[0], myCvector->vqueuing_socket[0], myCvector->emetteur, message);
    //cout<< "From " << myName << " : Sending report to SLAVE at queuing port " << myCvector.vqueuing_port[1] << " with socket " << myCvector.vqueuing_socket[1] << endl;
    Qservice->WRITE_QUEUING_MESSAGE(machine_name, myCvector->vqueuing_port[1], myCvector->vqueuing_socket[1], myCvector->emetteur, message);

    cout << "From " << myName << " : Report sent." << endl;

}

/* Reports geolocation to Leica */
void report_geolocation(geolocation still_geolocation, double geolocation_date, char* machine_name, CSampling* Sservice, COMMUNICATION_VECTOR* myCvector) {

    char message[100] = "\0";

    sprintf(message, "%d/%d,%d/%d,%d/%d %s %d/%d,%d/%d,%d/%d %s %g",
            still_geolocation.Latitude[0],
            still_geolocation.Latitude[1],
            still_geolocation.Latitude[2],
            still_geolocation.Latitude[3],
            still_geolocation.Latitude[4],
            still_geolocation.Latitude[5],
            still_geolocation.LatitudeRef,
            still_geolocation.Longitude[0],
            still_geolocation.Longitude[1],
            still_geolocation.Longitude[2],
            still_geolocation.Longitude[3],
            still_geolocation.Longitude[4],
            still_geolocation.Longitude[5],
            still_geolocation.LongitudeRef,
            geolocation_date);

    //cout << "From " << myName << " : Sending geolocation report to LEICA. (" << message << ")" << endl;
    cout << "From " << myName << " : Sending geolocation report to LEICA." << endl;


    Sservice->WRITE_SAMPLING_MESSAGE(machine_name, myCvector->vsamp_port[2], myCvector->vsamp_socket[2], myCvector->emetteur, message);

    cout << "From " << myName << " : Geolocation sent." << endl;

}

void orienter_satellite(orientation orientation_order, double date_action, COMMUNICATION_VECTOR* myCvector, CQueuing* Qservice, CSampling* Sservice, orientation* current_orientation, char *arg, Horloge* clock) {

    double geolocation_date;
    geolocation still_geolocation;


    // SATELLITE ORIENTATION
    cout << "From " << myName << " : Satellite orientation in progress..." << endl;
    orienter_moteur(orientation_order, arg);


    // SETUP CURRENT ORIENTATION VALUES
    set_orientation(current_orientation, &orientation_order);


    // REPORTS TO MANAGERS
    report_orientation(*current_orientation, arg, Qservice, myCvector);


    // WAIT GEOLOCATION TIME (CAPTURE TIME)
    cout << "From " << myName << " : Waiting action date for geolocation... (" << date_action - clock->get_temps() << " seconds)" << endl;
    while (clock->get_temps() < date_action) {
        usleep(100000);
    }

    // GEOLOCATION
    geolocate(&still_geolocation, &geolocation_date, clock);

    // REPORT TO LEICA
    report_geolocation(still_geolocation, geolocation_date, arg, Sservice, myCvector);

}

void execute_orientation(COMMUNICATION_VECTOR* myCvector, CQueuing* Qservice, CSampling* Sservice, char* machine_name, Horloge* clock, orientation* current_orientation) {

    // Variables liées aux ordres
    int ret_mess;
    orientation orientation_order;
    init_orientation(&orientation_order);
    double date_action;


    cout << endl << "From " << myName << " : Checking the reception of a new orientation..." << endl;

    ret_mess = receive_orientation(&orientation_order, &date_action, Sservice, myCvector, 0);
    if (ret_mess > 0) {

        if (!compare_orientation(&orientation_order, current_orientation)) {

            cout << "From " << myName << " : New orientation received from MASTER." << endl;

            orienter_satellite(orientation_order, date_action, myCvector, Qservice, Sservice, current_orientation, machine_name, clock);

        } else {
            cout << "From " << myName << " : The orientation received from MASTER and the current orientation are the same !" << endl;
        }

    } else {

        ret_mess = receive_orientation(&orientation_order, &date_action, Sservice, myCvector, 1);
        if (ret_mess > 0) {

            if (!compare_orientation(&orientation_order, current_orientation)) {

                cout << "From " << myName << " : New orientation received from SLAVE." << endl;

                orienter_satellite(orientation_order, date_action, myCvector, Qservice, Sservice, current_orientation, machine_name, clock);

            } else {
                cout << "From " << myName << " : The orientation received from SLAVE and the current orientation are the same !" << endl;
            }

        }

    }

}

/* SCAO Partition Manager : execute SCAO's functions (currently only one) */
void scao_manager(COMMUNICATION_VECTOR* myCvector, CQueuing* Qservice, CSampling* Sservice, char* machine_name, Horloge* clock, orientation* current_orientation) {

    // Receives orientation orders, orientates satellite, does geolocation and sends reports
    execute_orientation(myCvector, Qservice, Sservice, machine_name, clock, current_orientation);

}

/* Displays contents of the communication vector */
void display_communication(COMMUNICATION_VECTOR* myCvector) {

    cout << endl << "########## COMMUNICATION TABLE #########" << endl;

    cout << "QUEUING\t\t|SAMPLING" << endl;
    cout << "Port\t|Socket\t|Port\t|Socket" << endl;
    for (int i = 0; i < MAX_COMM_SIZE; i++) cout << myCvector->vqueuing_port[i] << "\t|" << myCvector->vqueuing_socket[i] << "\t|" << myCvector->vsamp_port[i] << "\t|" << myCvector->vsamp_socket[i] << endl;
    cout << "----------------------------------------------------------------------" << endl << endl;


}

int main(int argc, char *argv[]) {

    int pid_partition;
    string mode = "c";
    int redemarrage = atoi(argv[7]);
    int position = atoi(argv[6]);

    pid_partition = GUI_ARINC_partition("Scao", position, redemarrage);

    //For future processing
    if (pid_partition) {

    }

    // COMMUNICATION OBJECTS
    COMMUNICATION_VECTOR myCvector;
    CQueuing Qservice; /* Communication Queuing */
    CSampling Sservice; /* Communication Sampling */

    // COMMUNICATION VECTOR CONFIGURATION
    myCvector = init_communication(argc, argv, &mode);
    size_t size = myCvector.emetteur.size() + 1;
    char * machine_name = new char[ size ];
    // copier la chaîne
    strncpy(machine_name, myCvector.emetteur.c_str(), size);

    // COMMUNICATION VECTOR DISPLAY
    //display_communication(&myCvector)

    // SATELLITE OBJECTS
    Horloge clock;

    // ORIENTATION VARIABLE
    orientation current_orientation;
    init_orientation(&current_orientation);

    orienter_moteur(machine_name);

    while (1) {

        usleep(100000);
        scao_manager(&myCvector, &Qservice, &Sservice, machine_name, &clock, &current_orientation);

    };


    signal(SIGINT, (sig_t) bye);

    return 0;
}
