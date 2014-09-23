#include "include/CBasefunction.h"

int main(int argc, char *argv[]) {
    int redemarrage = atoi(argv[7]);
    int position = atoi(argv[6]);
    GUI_ARINC_partition("Partition1", position, redemarrage);

    int ret;

    int i;
    std::string *argv2= new std::string[argc + 3]; //to match c++ convention; previously char* argv2[argc+3]
    argv2[0] = "java";
    argv2[1] = "QuatreBoutonsInterfaceNoyau";
    argv2[2] = argv[0];

    for (i = 1; i < argc; i++) {
        argv2[i + 2] = argv[i];
    }
    argv2[argc + 2] = "0";

    /************************************/
    /**		Main programme		**/
    /************************************/

    ret = execlp("java", (argv2[0]).c_str(), (argv2[1]).c_str(), (argv2[2]).c_str(), NULL);
    if (ret == -1)
        perror("Exec : ");
}

