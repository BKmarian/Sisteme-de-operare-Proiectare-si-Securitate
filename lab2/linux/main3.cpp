#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <math.h>

/* this function is run by the second thread */
int global = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void *increment(void *param)
{
    for(int i = 0 ;i  < 1000000 ; i++) {
        pthread_mutex_lock(&mutex);
        global++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;

}

int main()
{

    void* result;
    pthread_t handle1,handle2;

    if(pthread_create(&handle1, NULL, increment, NULL)) {

        printf("Eroare creare thread\n");
        return -1;
    }

    if(pthread_create(&handle2, NULL, increment, NULL)) {

        printf("Eroare creare thread\n");
        return -1;
    }

    if(pthread_join(handle1, &result)) {

        printf("Eroare join\n");
        return -1;

    }

    if(pthread_join(handle2, &result)) {

        printf("Eroare join\n");
        return -1;

    }

    printf("%d",global);

    pthread_mutex_destroy(&mutex);
}