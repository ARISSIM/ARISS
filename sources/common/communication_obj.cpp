#include"include/communication_obj.h"

communication_obj::communication_obj() {

    perror("error, can't use the default constructor");
    exit(2);
}

communication_obj::communication_obj(intnbarg, char*argument[]) {
    my_vector = init_communication(nbarg, argument);
}

char communication_obj::get_emetteur() {
    return (my_vector.emetteur[0]);
}

int communication_obj::get_vsamp_socket() {
    return (my_vector.vsamp_socket[0]);
}

int communication_obj::get_vqueuing_socket() {
    return (my_vector.vqueuing_socket[0]);
}

int communication_obj::get_vsamp_port() {
    return (my_vector.vsamp_port[0]);
}

int communication_obj::get_vqueuing_port() {
    return (my_vector.vqueuing_port[0]);
}


