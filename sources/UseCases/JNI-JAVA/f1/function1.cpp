#include "include/CBasefunction.h"
#include "common/include/CBasefunction.h"

int main(int argc, char *argv[]) {
    int redemarrage = atoi(argv[7]);
    int position = atoi(argv[6]);
    GUI_ARINC_partition("partition1", position, redemarrage);

    //////////////////////////////////
    //	PROGRAMME PRINCIPAL	//
    //////////////////////////////////
  /*  std::cout << "------------ F1 -------------" << argv[0] << std::endl;
    system("pwd");
    //std::cout<<"avant Quatre saison"<< std::endl ;	
    system("java QuatreBoutonsInterfaceNoyau");
    //std::cout<<"après Quatre saison"<< std::endl ;
    system("java Dcpt");*/
}

