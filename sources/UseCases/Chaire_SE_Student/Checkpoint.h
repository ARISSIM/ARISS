#ifndef CHECKPOINT_H
#define CHECKPOINT_H 

/*!
* \file Checkpoint.h
* \brief Backup structure to save the states of each images
* \version 2.0
* \author Lucie BEAUSSART Thomas BETOUS Abdelkader BOUARFA William EXCOFFON
*/

#include <stdlib.h>     /* srand, rand */
#include <time.h>   
#include <unistd.h>
#include <iostream>

  /*! \class Checkpoint
   * \brief Creation and management of the checkpoints.
   */

using namespace std; 

class Checkpoint{

	protected :
	
	int id_image_sauvegarde;	/*!<Image id*/
	int id_action;			/*!<Index in the plan of the corresponding action*/
	bool etat[5];			/*!<States of the image: took, analysed, validated, stored, transmitted*/
	int taille_image;		/*!<Weight of the image */
	int pointeur_mem;		/*!<Pointer in the memory */

	public :
	
	Checkpoint();
	

	//int comparer(Checkpoint);
	
    /*!
     *  \brief State assessor
     *
     *  Return the i-th state of an image.
     *
     *  \param int: State index (from 0 to 4).
     *  \return Image's state.
     */
	bool get_etat(int);
	
    /*!
     *  \brief Assessor id_image_sauvegarde
     *
     * Return the image's id stored in the checkpoint.
     * 
     *  \return Image id.
     */
	int get_id_image_sauvegarde();
	
    /*!
     *  \brief id_action & id_image_sauvegarde mutator.
     *
     * \param int: New id_image_sauvegarde.
     * \param int: New id_action.
     */	
	void set_checkpoint(int,int);
	
    /*!
     *  \brief Assessor id_action.
     *  \return id_action.
     */
	int get_id_action();
	
	//mutateurs
	
    /*!
     *  \brief Single state mutator.
     *
     * Method to change a specific state in the checkpoint.
     * 
     *  \param int: State index (from 0 to 4).
     *  \param bool: New value of the state.
     */

	void set_etat(int,bool);
	
    /*!
     *  \brief Single state and image id mutator.
     *
     *  Change the image id and a state.
     *
     *  \param int: New image id.
     *  \param  int: State index (from 0 to 4).
     *  \param bool: New value of the state.
     */
	void set_etat(int, int, bool);
	
	
	//int get_action_from_id(int);
     /*!
     *  \brief All-states mutator 
     *
     * Change all the values of a checkpoint with the values of another one.
     * 
     *  \param Checkpoint: Checkpoint from wich the values are taken.
     */
     
	void set_tous_etats(Checkpoint);
	
    /*!
     *  \brief State mutator 
     *
     * Change all the states with the values of another one.
     * 
     *  \param bool*: New states.
     */
	void set_tous_etats(bool[5]); //Surcharge
	
     /*!
     *  \brief Image weight mutator 
     *
     * Change the values of taille_image.
     * 
     *  \param int: New value of taille_image.
     */
	void set_taille_image(int);

     /*!
     *  \brief Image weight mutator 
     *
     * Change the values of taille_image with the one of an existing checkpoint.
     * 
     *  \param Checkpoint: Checkpoint contening the new value of taille_image.
     */	
	void set_taille_image(Checkpoint); /* version surchargée */
	
     /*!
     *  \brief Memory pointer mutator 
     *
     * Change the values of pointeur_mem.
     * 
     *  \param int: New value of pointeur_mem.
     */	
	void set_pointeur_mem(int);
	
     /*!
     *  \brief Memory pointer mutator 
     *
     * Change the values of pointeur_mem with the one of an existing checkpoint.
     * 
     *  \param Checkpoint: Checkpoint contening the new value of pointeur_mem.
     */	
	void set_pointeur_mem(Checkpoint);
	
     /*!
     *  \brief taille_image assessor
     * 
     *  \return taille_image.
     */	
	int get_taille_image();
	
     /*!
     *  \brief pointeur_mem assessor
     * 
     *  \return pointeur_mem.
     */	
	int get_pointeur_mem();

};

#endif
