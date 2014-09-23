#ifndef REBOOTFLAG_H
#define REBOOTFLAG_H 

/*!
 * \file RebootFlag.h
 * \brief 
 * \version 2.0
 * \author Lucie BEAUSSART Thomas BETOUS Abdelkader BOUARFA William EXCOFFON
 */
#include <iostream>
#include "unistd.h"
#include "signal.h"

using namespace std;

class RebootFlag {
protected:
    int flag_value; /*<Number of consecutive errors in a partition */
    int flag_limit; /*<Limit of consecutive erros tolerated  */
    int partition_pid; /*<PID of the partition controlled */

public:
    RebootFlag();

    /*!
     *  \brief Increase the flag value
     * 
     * Each time this function is called, the flag_value is increased and controlled, if it's over the flag_limit (check by calling flag()) it calls the reboot() function to kill the partition process.
     */
    void inc();
    /*!
     *  \brief Decrease flag_value
     */

    void dec();

    /*!
     *  \brief Accessor: flag_value
     * 
     * \return flag_value
     */

    int get_flag_value();

    /*!
     *  \brief Mutator flag_limit
     */

    void set_flag_limit(int);

    /*!
     *  \brief Mutator partition_pid
     */
    void set_partition_pid(int);

    /*!
     *  \brief Compare flag_value to flag_limit
     * 
     * \return The result of flag_value < flag_limit
     */
    bool flag();

    /*!
     *  \brief Reboot the partition
     */

    void reboot();

};

#endif
