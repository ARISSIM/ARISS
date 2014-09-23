#include "include/CBasefunction.h"

int main(int argc, char *argv[]) {
    int redemarrage = atoi(argv[7]);
    int position = atoi(argv[6]);
    GUI_ARINC_partition("Partition1", position, redemarrage);
    int ret;

    /*********************************************************/
    /**		Main programme	execvp	**/
    /**********************************************************/

    /**  /!\ absulute path /!\
     * to find the coorect argument corresponding to your system, 
     *  1 / plese open nbexec script located in PETSHOP-DIR/platform/lib 
     *  2/ add, in the main loop (at the very end of file) "echo" after the eval comand
     *  3/ lauch petshop (cd ../../bin/petshop
     *  4/ copy the argument in he following, they are split by a space charater
     * 	;*/



    std::string argument[19];
    argument[0] = "/usr/lib/jvm/java-8-oracle/bin/java";
    argument[1] = "-Djdk.home=/usr/lib/jvm/java-8-oracle";
    argument[2] = "-classpath";
    argument[3] = "/home/cronel/petshop/platform/lib/boot.jar:/home/cronel/petshop/platform/lib/org-openide-modules.jar:/home/cronel/petshop/platform/lib/org-openide-util.jar:/home/cronel/petshop/platform/lib/org-openide-util-lookup.jar:/home/cronel/petshop/platform/lib/locale/boot_ja.jar:/home/cronel/petshop/platform/lib/locale/boot_pt_BR.jar:/home/cronel/petshop/platform/lib/locale/boot_ru.jar:/home/cronel/petshop/platform/lib/locale/boot_zh_CN.jar:/home/cronel/petshop/platform/lib/locale/org-openide-modules_ja.jar:/home/cronel/petshop/platform/lib/locale/org-openide-modules_pt_BR.jar:/home/cronel/petshop/platform/lib/locale/org-openide-modules_ru.jar:/home/cronel/petshop/platform/lib/locale/org-openide-modules_zh_CN.jar:/home/cronel/petshop/platform/lib/locale/org-openide-util_ja.jar:/home/cronel/petshop/platform/lib/locale/org-openide-util-lookup_ja.jar:/home/cronel/petshop/platform/lib/locale/org-openide-util-lookup_pt_BR.jar:/home/cronel/petshop/platform/lib/locale/org-openide-util-lookup_ru.jar:/home/cronel/petshop/platform/lib/locale/org-openide-util-lookup_zh_CN.jar:/home/cronel/petshop/platform/lib/locale/org-openide-util_pt_BR.jar:/home/cronel/petshop/platform/lib/locale/org-openide-util_ru.jar:/home/cronel/petshop/platform/lib/locale/org-openide-util_zh_CN.jar:/usr/lib/jvm/java-8-oracle/lib/dt.jar:/usr/lib/jvm/java-8-oracle/lib/tools.jar";
    argument[4] = "-Dnetbeans.running.environment=gnome";
    argument[5] = "-Dnetbeans.dirs=/home/cronel/petshop/harness:/home/cronel/petshop/java:/home/cronel/petshop/platform:/home/cronel/petshop/petshop:/home/cronel/petshop/extra:/home/cronel/petshop/ics:/home/cronel/petshop/websvccommon:/home/cronel/petshop/apisupport:/home/cronel/petshop/extide:/home/cronel/petshop/ide:";
    argument[6] = "-Dnetbeans.home=/home/cronel/petshop/platform";
    argument[7] = "-Xmx1024m";
    argument[8] = "-Xss2m";
    argument[9] = "-Xms32m";
    argument[10] = "-XX:+HeapDumpOnOutOfMemoryError";
    argument[11] = "-XX:HeapDumpPath=/home/cronel/.petshop/dev/var/log/heapdump.hprof";
    argument[12] = "org.netbeans.Main";
    argument[13] = "--cachedir";
    argument[14] = "/home/cronel/.petshop/dev/var/cache";
    argument[15] = "--userdir";
    argument[16] = "/home/cronel/.petshop/dev";
    argument[17] = "--branding";
    argument[18] = "petshop";

    ret = execlp("java", (argument[0]).c_str(), (argument[1]).c_str(), (argument[2]).c_str(), (argument[3]).c_str(), (argument[4]).c_str(), (argument[5]).c_str(), (argument[6]).c_str(), (argument[7]).c_str(), (argument[8]).c_str(), (argument[9]).c_str(), (argument[10]).c_str(), (argument[11]).c_str(), (argument[12]).c_str(), (argument[13]).c_str(), (argument[14]).c_str(), (argument[15]).c_str(), (argument[16]).c_str(), (argument[17]).c_str(),(argument[18]).c_str(), NULL);
    if (ret == -1)
        perror("Exec : ");
    



    ret = execlp("java", (argument[0]).c_str(), (argument[1]).c_str(), (argument[2]).c_str(), (argument[3]).c_str(), (argument[4]).c_str(), (argument[5]).c_str(), (argument[6]).c_str(), (argument[7]).c_str(), (argument[8]).c_str(), (argument[9]).c_str(), (argument[10]).c_str(), (argument[11]).c_str(), NULL);
    if (ret == -1)
        perror("Exec : ");    

}
