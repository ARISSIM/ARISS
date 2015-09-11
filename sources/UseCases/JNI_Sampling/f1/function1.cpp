/*
 * Use case JNI_Template function1.cpp
 */
#include "CBasefunction.h"
#include <iostream>
#include <sstream>
std::string convertInt(int num);

int main(int argc, char *argv[])
{
    int redemarrage = atoi(argv[7]);
    int position = atoi(argv[6]);
    GUI_ARINC_partition("Partition1", position, redemarrage);

    int nbarg = argc;
    char **argument = new char*[argc];
    int i = 0;
    for (i = 0; i <= nbarg; i++) {
        argument[i] = argv[i];
    }
    COMMUNICATION_VECTOR myCvector;
    myCvector = init_communication(argument, NULL);
        
        std::string name = argv[0];
        int portID;
        int sock;
        vector_get(&(myCvector.vsamp_port), 0, &portID);
        std::cout << "Sampling port : " << portID << std::endl;
        vector_get(&(myCvector.vsamp_socket), 0, &sock);
        std::cout << "Sampling socket : " << sock << std::endl;
        std::string emetteur = myCvector.emetteur;
        
//        std::string argument[20];
//        argument[0] = "/usr/lib/jvm/java-8-oracle/bin/java";
//        argument[1] = "TestJNI"; // Class Name
//        argument[2] = name;
//        argument[3] = convertInt(portID);
//        argument[4] = convertInt(sock);
//        argument[5] = emetteur;
//        
//        int ret = execlp("java",argument[0],argument[1],argument[2],argument[3],argument[4],argument[5],NULL);
//        if (ret == -1)
//            perror("Exec : ");
//        
        
//        std::string commande = "java TestJNI " + name + " " + convertInt(portID) + " " + convertInt(sock) + " " + emetteur;
        std::string sPortID = convertInt(portID);
        std::string sSock = convertInt(sock);
        std::string sClass = "TestJNISampling";
//        
//        std::cout << "java" <<  sClass.c_str()<<name.c_str()<<sPortID.c_str()<<sSock.c_str()<<emetteur.c_str()<< NULL <<std::endl;
//        
        int ret = execlp("java","/usr/lib/jvm/java-8-oracle/bin/java",sClass.c_str(),name.c_str(),sPortID.c_str(),sSock.c_str(),emetteur.c_str(), NULL);
        if (ret == -1)
            perror("Exec : ");
        
        
        
        
        
   /*      std::string argument[20];
    argument[0] = "/usr/lib/jvm/java-8-oracle/bin/java";
    argument[1] = "-Djdk.home=/usr/lib/jvm/java-8-oracle";
    argument[2] = "-classpath";
    argument[3] = "/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/platform/lib/boot.jar:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/platform/lib/org-openide-modules.jar:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/platform/lib/org-openide-util.jar:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/platform/lib/org-openide-util-lookup.jar:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/platform/lib/locale/boot_ja.jar:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/platform/lib/locale/boot_pt_BR.jar:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/platform/lib/locale/boot_ru.jar:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/platform/lib/locale/boot_zh_CN.jar:/home/ics/NetBeansProjects/modules/suite! s/petshop/ petshop.application/target/petshop/platform/lib/locale/org-openide-modules_ja.jar:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/platform/lib/locale/org-openide-modules_pt_BR.jar:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/platform/lib/locale/org-openide-modules_ru.jar:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/platform/lib/locale/org-openide-modules_zh_CN.jar:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/platform/lib/locale/org-openide-util_ja.jar:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/platform/lib/locale/org-openide-util-lookup_ja.jar:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/platform/lib/locale/org-openide-util-lookup_pt_BR.jar:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/platform/lib/loc! ale/org-op enide-util-lookup_ru.jar:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/platform/lib/locale/org-openide-util-lookup_zh_CN.jar:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/platform/lib/locale/org-openide-util_pt_BR.jar:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/platform/lib/locale/org-openide-util_ru.jar:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/platform/lib/locale/org-openide-util_zh_CN.jar:/usr/lib/jvm/java-8-oracle/lib/dt.jar:/usr/lib/jvm/java-8-oracle/lib/tools.jar";
    argument[4] = "-Dnetbeans.running.environment=gnome";
    argument[5] = "-Dnetbeans.dirs=/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/java:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/apisupport:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/ics:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/harness:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/websvccommon:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/petshop:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/extra:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/etc:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/bin:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/extide:/home/ics/NetBeansProjects/modules/suites/petshop/pe! tshop.appl ication/target/petshop/platform:/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/ide:";
    argument[6] = "-Dnetbeans.home=/home/ics/NetBeansProjects/modules/suites/petshop/petshop.application/target/petshop/platform";
    argument[7] = "-Xmx1024m";
    argument[8] = "-Xss2m";
    argument[9] = "-Xms64m";
    argument[10] = "-XX:+HeapDumpOnOutOfMemoryError";
    argument[11] = "-XX:HeapDumpPath=/home/ics/.petshop/dev/var/log/heapdump.hprof";
    argument[12] = "-Dapexname=" + petshopArg;  
    argument[13] = "org.netbeans.Main";
    argument[14] = "--cachedir";
    argument[15] = "/home/ics/.petshop/dev/var/ca.home=/usr/lib/jvm/java-8-oraclche";
    argument[16] = "--userdir";
    argument[17] = "/home/ics/.petshop/dev";
    argument[18] = "--branding";
    argument[19] = "petshop";

    ret = execlp("java", (argument[0]).c_str(), (argument[1]).c_str(), (argument[2]).c_str(), (argument[3]).c_str(), (argument[4]).c_str(), (argument[5]).c_str(), (argument[6]).c_str(), (argument[7]).c_str(), (argument[8]).c_str(), (argument[9]).c_str(), (argument[10]).c_str(), (argument[11]).c_str(), (argument[12]).c_str(), (argument[13]).c_str(), (argument[14]).c_str(), (argument[15]).c_str(), (argument[16]).c_str(), (argument[17]).c_str(), (argument[18]).c_str(), (argument[19]).c_str(), NULL);
    if (ret == -1)
        perror("Exec : ");*/
        
        
    
}

std::string convertInt(int num){
    std::stringstream ss;
    ss << num;
    return ss.str();
}

