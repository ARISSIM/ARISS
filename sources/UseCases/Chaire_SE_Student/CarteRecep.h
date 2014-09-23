#ifndef CARTERECEP_H
#define CARTERECEP_H 

/*!
 * \file CarteRecep.h
 * \brief Plan reception device
 * \version 2.0
 * \author Lucie BEAUSSART Thomas BETOUS Abdelkader BOUARFA William EXCOFFON
 */


#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

/*! \class CarteComm
   * \brief Emulates a TC controller.
   *
   *  This device is used to recieve mission plan from earth using low speed communications.
   */
class CarteRecep {

protected :
	 bool etat; /*!< Status of the device. True = Activated */
	

public :

		CarteRecep();
		
		/*!
		*  \brief Activation protocol
		*
		*  \return Status of the device (activated or not).
		*/
		
		bool activer_carte();
		
		/*!
		*  \brief Plan reception protocol
		*
		* This function uses scp protocol and rsa cryptage to recieve plans from a distant machine.
		*  \return 0 for an error, else 1.
		*/
		
		int recevoir_plan();
		
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
