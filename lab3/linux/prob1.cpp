
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <queue>
#include <semaphore.h>
#include <pthread.h>
#include <string>
std::queue<std::string> coada;
sem_t semaphore;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void destroy() {
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&conditionMutex);
    pthread_cond_destroy(&cond);
}
const char slash[2] = "/";
void *afisare(void *param) {
    int threadId = *(int *)param;
	while(true) {
		sem_wait(&semaphore);
		pthread_mutex_lock( &mutex );
		std::string numeFisier = coada.front();
		if (numeFisier == "abc") {
            coada.pop();
            pthread_mutex_unlock(&mutex);
            free(param);
			break;
		}
		printf("%s:   processed by thread %d\n",numeFisier.c_str(),threadId);
		coada.pop();
		pthread_mutex_unlock( &mutex );
	}
	return NULL;
}
void readInDepth(int depth , char * path) {
    struct dirent *entry;
    DIR * dir;
    if (!(dir = opendir(path)))
        return;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        if (entry->d_type == DT_DIR) {
            pthread_mutex_lock( &mutex );
            std::string numeFisier(entry->d_name);
            coada.push(numeFisier);
            pthread_mutex_unlock( &mutex );
            sem_post(&semaphore);
            strcat(path,slash);
            strcat(path,entry->d_name);
            readInDepth(depth + 1,path);
        }
        else {
            pthread_mutex_lock( &mutex );
            std::string numeFisier(entry->d_name);
            coada.push(numeFisier);
            pthread_mutex_unlock( &mutex );
            sem_post(&semaphore);
        }
    }

}
int main() {
    void* result;
    pthread_t handle[4];
    sem_init(&semaphore , 0 , 0);
    for(int i = 0 ; i <=3 ; i++) {
        int *arg = (int *)malloc(sizeof(*arg));
        *arg = i;
	    pthread_create(&handle[i], NULL, afisare, arg);
    }
    char path[500] = "..";
    readInDepth(0,path);

    coada.push("abc");
    sem_post(&semaphore);
    coada.push("abc");
    sem_post(&semaphore);
    coada.push("abc");
    sem_post(&semaphore);
    coada.push("abc");
    sem_post(&semaphore);

    pthread_join(handle[0], &result);
    pthread_join(handle[1], &result);
    pthread_join(handle[2], &result);
    pthread_join(handle[3], &result);

    destroy();
}
 
