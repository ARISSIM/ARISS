#include "CBasefunction.h"
#include <iostream>

int main(int argc, char *argv[]) {
    int redemarrage = atoi(argv[7]);
    int position = atoi(argv[6]);
    GUI_ARINC_partition("Partition2", position, redemarrage);
    std::cout << "write \"exit\" to stop the partitiion 2" << std::endl;
    char * input_chain= NULL;
    int ret=0;
    for (;;) {
        std::cout << "in looop" << input_chain << std::endl;


        std::cin>>input_chain;
        std::cout << "you wrote : " << input_chain << std::endl;
        ret= strcmp(input_chain,"exit");
        if (ret==0) {
            std::cout << "now exiting the partiton" << input_chain << std::endl;
            return (0);
        }
    }
}

