#ifndef PLAN_H
#define PLAN_H 

/*!
 * \file Table.h
 * \brief Storage object for instruction
 * \version 2.0
 * \author Lucie BEAUSSART Thomas BETOUS Abdelkader BOUARFA William EXCOFFON
 */



#include <sys/types.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdio.h>
#include <string.h>   
#include <stdlib.h>

/*! \class Plan
 * \brief Import and stock the instructions
 *
 *  It manages the importation and the use of the instructions from the .txt file send by the on-earth station.
 */

using namespace std;

/**
 * \struct action
 * \brief Content an instruction
 * 
 */

struct action {
    int numero_action; /*!< Index of the instruction in the plan*/
    std::string nature; /*!< Type  of the command (IMG, TSF, PLA) */
    struct tm date_action; /*!< Date for the realisation of the instruction */
    long int numero_image; /*!< ID of the image */
    double duree_action; /*!< Duration of the action*/
    double angle_prise_vue[3]; /*!< Attitude angle of the satellite*/
    int nbre_images_a_envoyer; /*!< Number of images to be transferred */
    int id_images_a_envoyer[10]; /*!< Image Id to be transferred */
};

class Plan {
protected:

    char plan_courant[30]; /*!< Name of the current plan */
    int id_action_courante; /*!< Index of the action in the plan */

    action liste_actions[50]; /*!< List of the actions to be done */

    int taille_plan; /*!< Length of the plan */

public:
    Plan();


    /*!
     *  \brief Update the plan
     *
     * Update the plan from an existing .txt file. If there's no new plan it keeps the old one, if the old one is finished then it switchs on the default plan. The new plan shall named NewPlan.txt and will be renamed OldPlan.txt.
     * 
     *  \return -1: upload the default plan. 0: upload the new one. 1: keep the old one. 2: keep the old one in case of a recovery. 3: in case of error.
     */
    int set_plan(int type);


    /*!
     *  \brief Update the plan in case of recovery
     *
     * \return -1 for error, 1 for success
     *
     */
    int set_plan_recouvrement();


    /*!
     *  \brief Update the next action
     *
     *  Return the next action and update the index in the plan.
     *
     *  \return Next action to be done.
     */
    action next_action();


    /*!
     *  \brief Assessor on the current action id.
     *
     *  \return Current action id.
     * 
     */
    int get_id_action_courante();

    /*!
     *  \brief Action assessor
     * 
     * return the action corresponding to the id sent in parameter.
     * 
     *  \param int: Action id
     *  \return Action matching the id.
     * 
     */
    action get_action(int);

    /*!
     *  \brief Mutator id_action_suivante
     *
     * Change the value of id_action_suivante.
     * 
     *  \param int: new value of id_action_suivante
     * 
     */

    void set_id_action_courante(int);

    /*!
     *  \brief Assessor taille_plan
     * 
     * 
     *  \return taille_plan
     * 
     */
    int get_taille();

    /*!
     *  \brief Mutator taille_plan 
     * 
     *  \param int: new value of taille_plan
     * 
     */
    void set_taille(int);


    /*!
     *  \brief Mutatr plan_courant
     *
     * Change the name of the plan currently used.
     * 
     * 
     *  \param char*: new name of plan
     * 
     */

    void set_plan_courant(char*);

    /*!
     *  \brief Assessor plan_courant
     *
     *  return the name of the current plan
     * 
     *  \return name of the plan
     * 
     */

    char* get_plan_courant();

    /*!
     *  \brief Modify an action in the list 
     * 
     *  \param action: new action
     *  \param int: index in the plan
     * 
     */

    void set_action(action, int);


    /*!
     *  \brief Mutator liste_actions
     *
     *  Modify the whole list of action
     * 
     *  \param action*: pointer to the new list
     * 
     */

    void set_liste_actions(action*);

    /*!
     *  \brief Assessor liste_actions
     *
     * 
     *  \return A pointer to a list of action
     * 
     */

    action* get_liste_actions();

    /*!
     *  \brief Display function
     *
     *  Display all the members of an action
     * 
     *  \param action: the action to be displayed
     * 
     */

    void afficher_action(action);

};


#endif
