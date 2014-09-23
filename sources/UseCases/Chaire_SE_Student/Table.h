#ifndef TABLE_H
#define TABLE_H 

/*!
* \file Table.h
* \brief Backup structure saving the checkpoints
* \version 2.0
* \author Lucie BEAUSSART Thomas BETOUS Abdelkader BOUARFA William EXCOFFON
*/

#include <stdlib.h>     /* srand, rand */
#include <time.h>   
#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdio.h>
#include <string.h>   
#include "Checkpoint.h"


#define TAILLE_TABLE 75 
/*! \def TAILLE_TABLE 
* \brief Size of the Checkpoint buffer
*/

/*! \class Table
* \brief Buffer of checkpoints 
* 
*/

using namespace std;




class Table{

	private :
	
	int num_action_dernier_checkpoint;	/*!<Action id used in the last checkpoint*/
	char plan_courant[30]; 			/*!<Name of the current plan use*/
	
	Checkpoint liste_checkpoints[TAILLE_TABLE];	/*!<List of the 75 last checkpoints*/
	int etat_courant_table;			/*!<Position index in the list of checkpoints*/

	public :
	
	Table();
	    /*!
    *  \brief Print the checkpoint table
    *
    *  This method is used to print the checkpoint list
    */
	void afficher();
	
    /*!
    *  \brief Add a checkpoint to the list
    *  \param checkpoint: checkpoint to be added
    *  \return 1 if operation was successful, 0 if not
    */
	int maj(Checkpoint);
	
	    /*!
    *  \brief Modify a checkpoint with the image id and the state to be modified
    *  \param int: id of the image 
    *  \param int: id of state of the image to be modified
    *  \param bool: new state
    *  \return 1 if operation was successful
    * 		0 if not
    */
	int maj(int,int,bool);
	
    /*!
    * \brief Assessor : id of the last cherckpoint
    * 
    * \return id of the last checkpoint
    */
	
	
	int get_num_check();
	
	    /*!
    *  \brief Assessor : last checkpoint stored
    *
    *  \return last checkpoint stored
    */
	Checkpoint get_dernier_check();

    /*!
    * \brief Get the index of an image in the checkpoint table
    * 
    * \param int: image id
    * \return index of the image in the table
    */	
	
	int id_image_to_position (int);
    
	/*!
    * \brief Get the id of an image from its position in the table
    * 
    * \param int: index
    * \return image id
    */	
		
	
	
	int position_to_id_image (int); 

    /*!
    * \brief Get the checkpoint of an image from its id
    * 
    * \param int: image id
    * \return checkpoint of the image
    */		
	Checkpoint get_check (int);

};

#endif
