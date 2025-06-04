#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_ITEMS 10

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t lleno;
sem_t vacio;
pthread_mutex_t mutex;

void* productor(void* arg) {
    for (int i = 0; i < NUM_ITEMS; i++) {
        int item = rand() % 100;

        sem_wait(&vacio);              
        pthread_mutex_lock(&mutex);    

        buffer[in] = item;
        printf("Productor produjo: %d\n", item);
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex); 

        sleep(1); 
    }

    pthread_exit(NULL);
}

void* consumidor(void* arg) {
    for (int i = 0; i < NUM_ITEMS; i++) {
        sem_wait(&lleno);              
        pthread_mutex_lock(&mutex);    
        int item = buffer[out];
        printf("Consumidor consumió: %d\n", item);
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex); 
        sem_post(&vacio);              

        sleep(2); 
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t prod_thread, cons_thread;

 
    sem_init(&lleno, 0, 0);
    sem_init(&vacio, 0, BUFFER_SIZE);
    pthread_mutex_init(&mutex, NULL);

  
    pthread_create(&prod_thread, NULL, productor, NULL);
    pthread_create(&cons_thread, NULL, consumidor, NULL);

    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

   
    sem_destroy(&lleno);
    sem_destroy(&vacio);
    pthread_mutex_destroy(&mutex);

    return 0;
}
