#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_ITEMS 10

int buffer_cond[BUFFER_SIZE];
int in_cond = 0, out_cond = 0;
int count = 0;

pthread_mutex_t mutex_cond;
pthread_cond_t cond_lleno;
pthread_cond_t cond_vacio;

void* productor_cond(void* arg) {
    for (int i = 0; i < NUM_ITEMS; i++) {
        int item = rand() % 100;

        pthread_mutex_lock(&mutex_cond);
        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&cond_vacio, &mutex_cond);
        }

        buffer_cond[in_cond] = item;
        printf("[cond] Productor produjo: %d\n", item);
        in_cond = (in_cond + 1) % BUFFER_SIZE;
        count++;

        pthread_cond_signal(&cond_lleno);
        pthread_mutex_unlock(&mutex_cond);

        sleep(1);
    }
    pthread_exit(NULL);
}

void* consumidor_cond(void* arg) {
    for (int i = 0; i < NUM_ITEMS; i++) {
        pthread_mutex_lock(&mutex_cond);
        while (count == 0) {
            pthread_cond_wait(&cond_lleno, &mutex_cond);
        }

        int item = buffer_cond[out_cond];
        printf("[cond] Consumidor consumió: %d\n", item);
        out_cond = (out_cond + 1) % BUFFER_SIZE;
        count--;

        pthread_cond_signal(&cond_vacio);
        pthread_mutex_unlock(&mutex_cond);

        sleep(2);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t prod_thread, cons_thread;

    pthread_mutex_init(&mutex_cond, NULL);
    pthread_cond_init(&cond_lleno, NULL);
    pthread_cond_init(&cond_vacio, NULL);

    pthread_create(&prod_thread, NULL, productor_cond, NULL);
    pthread_create(&cons_thread, NULL, consumidor_cond, NULL);

    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    pthread_mutex_destroy(&mutex_cond);
    pthread_cond_destroy(&cond_lleno);
    pthread_cond_destroy(&cond_vacio);

    return 0;
}
