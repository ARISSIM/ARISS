#include "include/CBasefunction.h"

#define SEMKEY_1 50000



static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * fonction_thread(void * arg) {
    int numero = *((int*) (&arg));
    usleep(10000 * numero);
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
        usleep(10000);
    }
    return NULL;
}


#define NB_THREADS 5

int main(int argc, char *argv[]) {

    int redemarrage = atoi(argv[7]);
    int position = atoi(argv[6]);
    GUI_ARINC_partition("Partition1", position, redemarrage);
    int i;
    pthread_t thread[NB_THREADS];



    /*pid_to_send = prec[partitionNumber];
    arguments.pid_to_watch = &pid_to_send;
    arguments.myCManager = this;
    arguments.pid_result = &pid_result;
    thread_attributes = (pthread_attr_t *) malloc(sizeof (pthread_attr_t));
    thread_watchdogM = (pthread_t *) malloc(sizeof (pthread_t));
    pthread_attr_init(thread_attributes);
    if (pthread_create(thread_watchdogM, thread_attributes, (void*(*)(void*)) & CManager::f_thread_watchdogM, (void *) &arguments) != 0)
        perror(" Monitoring thread creation failed !");
*/


    pthread_mutex_lock(& mutex);
    for (i = 0; i < NB_THREADS; i++) {
        int argument =  i + 1;
        pthread_create(& thread[i], NULL, fonction_thread, (void *) &argument);
        usleep(10000);
    }
    sleep(1);
    fprintf(stderr, "Liberation initiale du mutex\n");
    pthread_mutex_unlock(& mutex);
    for (i = 0; i < NB_THREADS; i++)
        pthread_join(thread[i], NULL);
    return EXIT_SUCCESS;
}
