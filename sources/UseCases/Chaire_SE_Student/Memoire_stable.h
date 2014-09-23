#ifndef MEMOIRE_H
#define MEMOIRE_H 

/*!
 * \file Memoire_stable.h
 * \brief Emulate the memory used for image storage
 * \version 2.0
 * \author Lucie BEAUSSART Thomas BETOUS Abdelkader BOUARFA William EXCOFFON
 */


#include <sys/types.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include "Camera.h"

using namespace std;

/*! \class Memoire_stable
 * \brief Image memory
 *
 *  This class emulate the hardware memory used to store the image took by the camera. It could be considered as a descriptor table for the storage of the images. In the simulator this class use bash command "cp" to send the images to an other station (connected by ethernet or wireless connection).
 */


class Memoire_stable {
protected:

public:

    Memoire_stable();

    /*!
     * \brief Copy the images from the temporary memory to the stable memory.
     * 
     * \param image: Image to store.
     */
    int stocker(image);

    /*!
     * \brief Delete an image in the memory
     * 
     * \param: Image to be deleted.
     */
    void vider(image);

    /*!
     * \brief Delete all the images stored
     */
    void purger();
};


#endif
