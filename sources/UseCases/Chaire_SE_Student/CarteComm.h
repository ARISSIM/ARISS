#ifndef CARTECOMM_H
#define CARTECOMM_H 

/*!
 * \file CarteComm.h
 * \brief Image transmission device
 * \version 2.0
 * \author Lucie BEAUSSART Thomas BETOUS Abdelkader BOUARFA William EXCOFFON
 */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

using namespace std;

/*! \class CarteComm
   * \brief Emulates a TM controller.
   *
   *  This device is used to transmit images from satellite to earth using high speed communications.
   */
class CarteComm {

protected :
	 bool active;/*!< Status of the device. True = Activated */
	

public :

		CarteComm();
		
		/*!
		*  \brief Activation protocol
		*
		*  \return Status of the device (activated or not).
		*/
		
		bool activer_carte();
		
		/*!
		*  \brief Transmission protocol
		* 
		* This function uses scp protocol and rsa cryptage to send images to a distant machine.
		*
		*  \param char*: Image's id
		*  \param char*: Name of the machine which will recieve the data.
		*  \return true in case of succes, else false.
		*/
				
		bool envoyer(char*, char*); // envoi d'une image désignée par son identifiant
		
		/*!
		*  \brief Desactivation protocol
		*
		*  \return Status of the device (activated or not).
		*/		
		bool desactiver_carte();
		
		
		/*!
		*  \brief Accessor: active.
		*
		*  \return Status of the device
		*/
				
		bool get_etat();
};


#endif
