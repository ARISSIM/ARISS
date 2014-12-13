#include "CBasefunction.h"
#include <iostream>

#define SEMKEY_2 50001

int main(int argc, char *argv[]) {
    int j = 0;
    int redemarrage = atoi(argv[7]);
    int position = atoi(argv[6]);
    GUI_ARINC_partition("Partition2", position, redemarrage);

    //creation of trheads
    //pour le moment rien mais à mettre ici

    for (;;) //main code
    {
        //test_flag(2);
        std::cout << "j vaut : " << j << std::endl;
        j++;
        sleep(1);
    }

}
