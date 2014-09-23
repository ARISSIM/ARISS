#include "CBasefunction.h"

/************		class				**********/
class communication_obj {
public:

    communication_obj();
    communication_obj(int nbarg, char* argument[]);

    char get_emetteur();
    int get_vsamp_socket();
    int get_vqueuing_socket();
    int get_vsamp_port();
    int get_vqueuing_port();

private:
    COMMUNICATION_VECTOR my_vector;

};
