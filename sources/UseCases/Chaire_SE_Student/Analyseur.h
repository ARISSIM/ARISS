#ifndef ANALYSEUR_H
#define ANALYSEUR_H 

/*!
* \file Analyseur.h
* \brief Analyse and compress the images from the camera.
* \version 2.0
* \author Lucie BEAUSSART Thomas BETOUS Abdelkader BOUARFA William EXCOFFON
*/


#include "Camera.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <iostream>
#include <stdio.h>

using namespace std;

/*!
 * \class Analyseur
 * \brief Analyse and compress the images from the camera.
 *
 *  It analyse the image and compare the analysis to a minimal value of quality (written in the plan). It also compress the file.
 * 
 */
class Analyseur{

	public :
	
	/*!
	*  \brief Constructor
	*/
	Analyseur();
	
	/*!
	*  \brief Image analysis
	*
	*  Analyse an image regarding the cloud cover, (10% of chance to be rejected for simulate this phenomenon).
	*
	*  \param char* : name of the file.
	*  \return 0 for an error, 1 if completed.
	*/
	int traiter_image(char*); // TODO : prévoir argument image; pour l'instant sortie aléatoire
	
	/*!
	*  \brief Compress an image
	*
	*  Create a .zip from an image file.
	*
	*  \param char* : name of the file.
	*  \return 0 for an error, 1 if completed.
	*/
	int compresser_image(char*);
	
};

#endif
