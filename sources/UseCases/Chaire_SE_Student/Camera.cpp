#include "Camera.h"

Camera::Camera() {

    //cout << "<Camera> Construction Camera." << endl;
    nb = 0;
}

int Camera::capturer(bool* order_status, char* name, int largeur, int duree_image, int qualite_jpg, double date_action, double timeout) {

    //arg est le argv[0] du master

    char id_recu[30] = "INITIAL_VALUE";
    int status[5] = {0, 0, 0, 0, 0};
    double date_actuelle;
    int ifmsg = 0;
    bool flag = true;
    char message[256];
    char type_action[10] = "IMG";
    Type_Message msg;
    //double date_reference = horloge->get_temps();
    size_t size = myCvector.emetteur.size() + 1;
    char * machine_name = new char[ size ];
    // copier la chaîne
    strncpy(machine_name, myCvector.emetteur.c_str(), size);

    sprintf(message, "%s %s %g %d %g %d", type_action, name, date_action, largeur, (double) duree_image, qualite_jpg);

    //cout << "<Camera> Sending order to LEICA on sampling port " << myCvector.vsamp_port[1] << " with socket " << myCvector.vsamp_socket[1] << "..." << endl;

    Sservice->WRITE_SAMPLING_MESSAGE(machine_name, myCvector.vsamp_port[1], myCvector.vsamp_socket[1], myCvector.emetteur, message);

    while (strcmp(name, id_recu) && flag) {//on sort si le timeout est dépassé ou si on a reçu un message et qu'il contient la valeur de l'ID de l'image

        date_actuelle = horloge->get_temps();

        flag = (int) (date_actuelle < (date_action + timeout)); //verification de la date

        //cout << "<Camera> Waiting for report from LEICA on queuing port with socket " << myCvector.vqueuing_socket[1] << "..." << endl;
        ifmsg = Qservice->READ_QUEUING_MESSAGE(myCvector.vqueuing_socket[1]); //recherche de message

        if (ifmsg > 0) { //lecture du message
            msg = Qservice->get_Message();

            cout << "<Camera> Report received from LEICA : " << msg.m_message << "." << endl;

            sscanf(msg.m_message, "%d %d %d %d %d %s", &status[0], &status[1], &status[2], &status[3], &status[4], id_recu);

            if (status[0] == 1) {
                flag = 1;
            } else flag = false;
        }

    }

    for (unsigned int i = 0; i < 5; i++) order_status[i] = (status[i] == 1);

    return flag;
}

void Camera::init_cam(CQueuing* Qserv, CSampling* Sserv, COMMUNICATION_VECTOR vector, Horloge* Hlge) { // TODO : vérifier qu'on réinitialise bien la camera si le SLAVE prend la main

    Qservice = Qserv;
    Sservice = Sserv;
    myCvector = vector;
    horloge = Hlge;



}
