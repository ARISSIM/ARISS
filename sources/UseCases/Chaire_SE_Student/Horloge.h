#ifndef HORLOGE_H
#define HORLOGE_H 
/*!
* \file Horloge.h
* \brief Emulate the clock used by a partition
* \version 2.0
* \author Lucie BEAUSSART Thomas BETOUS Abdelkader BOUARFA William EXCOFFON
*/

#include <sys/types.h>
#include <stdio.h>
#include <iostream>
#include <time.h>

using namespace std;

  /*! \class Horloge
   * \brief Emulate the clock used by a partition
   *
   */

class Horloge {

protected :
	time_t big_bang; /*!< Origin of time for the simulator */
	

public :
		Horloge();
		
		/*!
		 * \brief Return the time since the big_bang value in seconds.
		 * \return Relative date in second.
		 */
		double get_temps();
		
		/*!
		 * \brief Transform a tm structure to a date in seconds.
		 * \param tm: timer containing the date.
		 * \return Relative date in seconds.
		 */
		double date2seconds( struct tm p_temps );
		
		//void affichage();
};


#endif
