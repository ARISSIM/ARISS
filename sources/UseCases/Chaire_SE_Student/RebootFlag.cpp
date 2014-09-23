#include "RebootFlag.h"
#include "../include/CBasefunction.h"
RebootFlag::RebootFlag() {

    flag_value = 0;
    flag_limit = 100;

}

void RebootFlag::inc() {

    flag_value++;

    if (!flag()) {
        std::cout << flag_limit << " consecutives errors ! Rebooting..." << endl;
        sleep(5);
        reboot();
    }
}

void RebootFlag::dec() {
    if (flag_value > 0) flag_value--;
}

int RebootFlag::get_flag_value() {
    return flag_value;
}

void RebootFlag::set_flag_limit(int limit) {
    flag_limit = limit;
}

void RebootFlag::set_partition_pid(int pid) {
    partition_pid = pid;
}

bool RebootFlag::flag() {
    return flag_value < flag_limit;
}

void RebootFlag::reboot() {
    GUI_ARINC_partition_kill(partition_pid);
}
