#ifndef MANAGER_H
#define MANAGER_H 

/*!
 * \file Manager.h
 * \brief Backup structure to save the states of each images
 * \version 2.0
 * \author Lucie BEAUSSART Thomas BETOUS Abdelkader BOUARFA William EXCOFFON
 */


#include "../include/CBasefunction.h"

// Inclusion des objets de gestion du satellite
#include "Analyseur.h"
#include "SCAO.h"
#include "Horloge.h"
#include "Plan.h"
#include "Camera.h"
#include "Memoire_stable.h"
#include "Table.h"
#include "Checkpoint.h"
#include "CarteComm.h"
#include "CarteRecep.h"

#include <sys/types.h>
#include <stdio.h>
#include <iostream>
#include <time.h>

using namespace std;

#define TIMEOUT_IMG 10
/*! \def TIMEOUT_IMG 
 * \brief Timeout limit for image acquisition (in s).
 */
#define TIMEOUT_TSF 10
/*! \def TIMEOUT_TSF 
 * \brief Timeout limit for image transmission (in s).
 */
#define TIMEOUT_SCAO 10
/*! \def TIMEOUT_SCAO 
 * \brief Timeout limit for positionning (in s).
 */

/*! \class Manager
 * \brief Used in Master and Slave as the mission manager object. It regroups all the functionnalities and the objects necessary to manage the mission.
 */

class Manager {
protected:

    string mode; /*<Partition running mode: Primary or Backup*/
    char myPartition[6]; /*<Partition name*/


    // Objets de gestion du satellite
    Analyseur* analyseur; /*<Image analyser*/
    Memoire_stable* memoire_stable; /*<Image memory*/
    CarteComm* carte_communication; /*<Image transmission device*/
    CarteRecep* carte_reception; /*<Plan reception device*/
    image image_capturee; /*<Image pictured by the camera*/
    image image_transfert; /*<Image to be send*/
    Horloge* horloge; /*<Clock*/

    Table table; /*<Checkpoint table structure*/
    Checkpoint check; /*<Current checkpoint*/
    Plan plan; /*<Mission plan*/

    // Variables liees aux tranferts de messages
    char message[256]; /*<String content in the message*/
    Type_Message msg; /*<Last global message structure recieved*/
    Type_Message previous_msg; /*<Last-but-one global message structure recieved*/
    Type_Message msg_check; /*<Checkpoint message*/
    Type_Message message_check; /*<Checkpoint message*/

    // Objets de communications
    COMMUNICATION_VECTOR myCvector; /*<Communication vector*/
    CQueuing Qservice; /*<Communication object: Queuing */
    CSampling Sservice; /*<Communication object: Sampling */

    //Elements en rapport avec la mission
    action action_en_cours; /*<Current action*/
    double date_action; /*<Deadline of the current action*/
    //Fonctions permettant l'exécution de la mission

    /*!
     *  \brief Refresh the mission plan.
     * 
     * \param bool: First launch controller: A special process is runned for the first launch.
     * \param bool: Indicate if the plan is already finished or not
     * 
     * \return True if the plan is finished.
     */
    bool get_new_plan(bool, bool);

    /*!
     *  \brief Grab the next action to be done.
     */
    void find_next_action();



public:

    Manager();
    /*!
     *  \brief Mutator on the mode.
     * 
     * \param string: New mode value.
     */
    void set_mode(string);

    /*!
     *  \brief Mutator on partition name
     * 
     * \param string: New partition name
     */
    void set_partition(string);

    /*!
     *  \brief Send a checkpoint to the backup partition.
     * 
     * \param Checkpoint: Checkpoint to be send.
     */
    void send_check(Checkpoint);

    /*!
     *  \brief Refresh checkpoint table
     * 
     * \param int: Image id of the checkpoint
     * \param int: Status index
     * \param bool: New state value
     */
    void actualiser_table(int, int, bool);

    /*!
     *  \brief Refresh checkpoint table.
     * 
     * \param Checkpoint: Checkpoint to be updated
     */
    void actualiser_table(Checkpoint);

    /*!
     *  \brief Find the index in the checkpoint table from an image id.
     * 
     * \param int: Image id
     * 
     * \return Index in the table
     */
    int table_id_image_to_position(int);

    /*!
     *  \brief Find the image from its index in the checkpoint table.
     * 
     * \param int: Index in the table
     * 
     * \return Image Id
     */
    int table_position_to_id_image(int);

    /*!
     *  \brief Get a checkpoint from its index in the table.
     * 
     * \param int: Table index
     * 
     * \return Checkpoint at the indexed position
     */
    Checkpoint table_get_check(int);

    /*!
     *  \brief Get the last checkpoint updated.
     * 
     * \return The last checkpoint updated
     */
    Checkpoint recuperer_dernier_check();

    /*!
     *  \brief Mission manager function.
     * 
     * This function uses all the methods to execute the mission. Contains functionnal and recovery functionalities
     * 
     * \param string : Partition name
     * \param string : Partition mode
     * \param Horloge*: Clock
     * \param Analyseur*: Image analyser
     * \param Memoire_stable*: Image memory
     * \param CarteComm*: Image transmission device
     * \param CarteRecep*: Plan transmission device
     * \param COMMUNICATION_VECTOR: Communication information vector
     * 
     * \return 0 in case of error.
     */
    int gerer_mission(string, string, Horloge*, Analyseur*, SCAO*, Camera*, Memoire_stable*, int, char*, CarteComm*, CarteRecep*, COMMUNICATION_VECTOR);

        /*!
         *  \brief Replace the current plan by a new one.
         * 
         * \param Plan New plan to be treated
         */
        void init_plan(Plan);

        /*!
         *  \brief Find an action in the current plan
         * 
         * \param int: action index
         * \return -1 in case of error,else current action id 
         */
        int trouver_action(int);

        /*!
         *  \brief Send a transmission order to the Leica partition.
         * 
         * 
         * \param bool*: Order status (realised or not)
         * \param char*: Image name
         * \param double: Action deadline
         * \param double: Action duration
         * \param double: Maximum execution time.
         * 
         * \return Error code for image transmission.
         */
        int order_transfer(bool*, char*, double, double, double);

        /*!
         *  \brief Get the report of the Leica partition.
         * 
         * \param bool*: Order status (realised or not)
         * \param char*: Image name
         * \param double: Action deadline
         * \param double: Action duration
         * \param double: Maximum execution time.
         * 
         * \return Error code for image transmission.
         */
        int receive_transfer_report(bool*, char*, double, double, double);

        /*!
         *  \brief Create a new Plan
         * 
         * This is a test function to create a random plan.
         * 
         * \param int: First action deadline delay
         * \param int: Number of instruction
         * \param string: Name of file
         */
        void Creer_Plan(int, int, string);

        /*!
         *  \brief Send order to Leica to take a picture.
         * 
         * \param char*: name of the image
         * \param int: Weight of the image (px)
         * \param int: Duration of the action (corresponding to the image length in px)
         * \param int: Jpg compression rate
         * \param double: Action deadline
         * \param double: Maximum execution time
         * \param bool: recovery mode (true or false)
         * 
         * \return Error code for picture
         */
        int capturer_image(char*, int, int, int, double, double, bool);

        /*!
         *  \brief Send positionning orders to SCAO.
         * 
         * \param float : Pitch value
         * \param float : Roll value
         * \param float : Yaw value
         * \param double: Action deadline
         * \param double: Maximum execution time
         * \param bool: recovery mode (true or false)
         * 
         * \return Error code for positionning
         */
        int orienter_satellite(float, float, float, double, double, bool);
    };


#endif
