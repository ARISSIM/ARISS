#include "include/CBasefunction.h"

int main(int argc, char *argv[])
{
GUI_ARINC_partition("Partition1",1);


//////////////////////////////////
//	PROGRAMME PRINCIPAL	//
//////////////////////////////////
std::cout<<"------------ F1 -------------"<< argv[0]<<	std::endl ;	
system("pwd");
//std::cout<<"avant Quatre saison"<< std::endl ;	
system("java QuatreBoutonsInterfaceNoyau");
//std::cout<<"après Quatre saison"<< std::endl ;
system("java Dcpt");
}
