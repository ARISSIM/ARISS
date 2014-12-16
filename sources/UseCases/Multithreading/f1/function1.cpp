#include "CBasefunction.h"
#include <iostream>
#include <stdlib.h>

#define SEMKEY_1 50000



static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * fonction_thread(void * arg) {

    long numero = (long) arg;
    std::cout << "numéro vaut : " << numero << std::endl;

    //usleep(random());
    std::cout << "Before while " << std::endl;

    while (1) {
        fprintf(stderr, "[%d] demande le mutexn\n", numero);
        if (numero != 2)
            while (pthread_mutex_trylock(& mutex) != 0)
                ;
        else
            pthread_mutex_lock(& mutex);
        fprintf(stderr, "        [%d] tient le mutexn\n", numero);
        sleep(1);
        fprintf(stderr, "[%d] lache le mutexn\n", numero);
        pthread_mutex_unlock(& mutex);
        usleep(100);
    }
    std::cout << "exit thread " << std::endl;


    return NULL;
}


#define NB_THREADS 5

int main(int argc, char *argv[]) {

    int redemarrage = atoi(argv[7]);
    int position = atoi(argv[6]);
    GUI_ARINC_partition("Partition1", position, redemarrage);
    long i;
    pthread_t thread[NB_THREADS];

    pthread_mutex_lock(& mutex);
    for (i = 0; i < NB_THREADS; i++) {
        long argument = i + 1;
        std::cout << "argument vaut : " << argument << std::endl;

        pthread_create(& thread[i], NULL, fonction_thread, (void *) argument);
        usleep(10000);
    }
    sleep(1);
    fprintf(stderr, "Liberation initiale du mutex\n");
    pthread_mutex_unlock(& mutex);
    for (i = 0; i < NB_THREADS; i++)
        pthread_join(thread[i], NULL);
    return EXIT_SUCCESS;
}
