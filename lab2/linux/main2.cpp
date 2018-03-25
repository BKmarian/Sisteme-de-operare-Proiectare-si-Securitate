#include <pthread.h>
#include <stdio.h>
#include <math.h>

/* this function is run by the second thread */
void *rad_x(void *param)
{

    int x = *(int *)param;
    double *radical = new double();
    *radical = sqrt(x);

    return (void*)radical;

}

int main()
{

    int x = 100;
    void* result;
    pthread_t handle;

    if(pthread_create(&handle, NULL, rad_x, &x)) {

        printf("Eroare creare thread\n");
        return -1;
    }

    if(pthread_join(handle, &result)) {

        printf("Eroare join\n");
        return -1;

    }

    double val = *(double *)result;
    printf("%f",val);

    delete result;
    return 0;

}