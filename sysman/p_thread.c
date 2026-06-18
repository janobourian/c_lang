#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *safeThread(void *arg){
    (void)arg;
    printf("Hello from our thread\n");
    return NULL;
}

void *safeOperation(void *arg){
    int *time = (int *)arg;
    sleep((unsigned int)*time);
    printf("Inside %d\n", *time);
    return NULL;
}

int main(){
    pthread_t thread;
    pthread_create(&thread, NULL, safeThread, NULL);
    pthread_join(thread, NULL);

    int NUM_THREADS = 10;
    pthread_t threads[NUM_THREADS];
    int args[NUM_THREADS];
    for(int i = 0; i < NUM_THREADS; i++){
        args[i] = i;
        pthread_create(&threads[i], NULL, safeOperation, &args[i]);
    }

    for( int j = 0; j < NUM_THREADS; j++){
        pthread_join(threads[j], NULL);
    }

    return 0;
}