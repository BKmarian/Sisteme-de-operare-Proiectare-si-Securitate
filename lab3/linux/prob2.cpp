
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <queue>
#include <pthread.h>
#include <string>
std::queue<std::string> coada;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t conditionMutex = PTHREAD_MUTEX_INITIALIZER;
const char slash[2] = "/";
void destroy() {
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&conditionMutex);
    pthread_cond_destroy(&cond);
}
void *afisare(void *param) {
    int threadId = *(int *)param;
    while(true) {

        pthread_mutex_lock( &conditionMutex );
        while(coada.size() == 0) {
            pthread_cond_wait( &cond , &mutex);
        }
        pthread_mutex_unlock( & conditionMutex );

        pthread_mutex_lock(&mutex);
        std::string numeFisier = coada.front();
        if (numeFisier == "abc") {
            pthread_mutex_unlock( & mutex );
            pthread_mutex_unlock( & conditionMutex );
            free(param);
            break;
        }
        coada.pop();
        pthread_mutex_unlock( & mutex );
        printf("%s:   processed by thread %d\n",numeFisier.c_str(),threadId);
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
            std::string numeFisier(entry->d_name);

            pthread_mutex_lock( &mutex );
            coada.push(numeFisier);
            pthread_cond_signal( &cond );
            pthread_mutex_unlock( &mutex );
            strcat(path,slash);
            strcat(path,entry->d_name);
            readInDepth(depth + 1,path);
        }
        else {
            std::string numeFisier(entry->d_name);
            pthread_mutex_lock( &mutex );
            coada.push(numeFisier);
            pthread_cond_signal( &cond );
            pthread_mutex_unlock( &mutex );
        }
    }

}
int main() {
    void* result;
    pthread_t handle[4];
    char path[500] = ".";
    readInDepth(0,path);
    for(int i = 0 ; i <=3 ; i++) {
        int *arg = (int *)malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&handle[i], NULL, afisare, arg);
    }

    coada.push("abc");
    coada.push("abc");
    coada.push("abc");
    coada.push("abc");
    pthread_cond_broadcast( &cond );
    pthread_join(handle[0], &result);
    pthread_join(handle[1], &result);
    pthread_join(handle[2], &result);
    pthread_join(handle[3], &result);

    destroy();
    
}
