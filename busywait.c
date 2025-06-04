#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_ITEMS 10

int buffer_busy[BUFFER_SIZE];
int in_busy = 0, out_busy = 0;
volatile int count_busy = 0;

pthread_mutex_t mutex_busy;

void* productor_busy(void* arg) {
    for (int i = 0; i < NUM_ITEMS; i++) {
        int item = rand() % 100;
        while (count_busy == BUFFER_SIZE);

        pthread_mutex_lock(&mutex_busy);
        buffer_busy[in_busy] = item;
        printf("🛠 [busy] Productor produjo: %d\n", item);
        in_busy = (in_busy + 1) % BUFFER_SIZE;
        count_busy++;
        pthread_mutex_unlock(&mutex_busy);

        sleep(1);
    }
    pthread_exit(NULL);
}

void* consumidor_busy(void* arg) {
    for (int i = 0; i < NUM_ITEMS; i++) {
        while (count_busy == 0);

        pthread_mutex_lock(&mutex_busy);
        int item = buffer_busy[out_busy];
        printf("🍽 [busy] Consumidor consumió: %d\n", item);
        out_busy = (out_busy + 1) % BUFFER_SIZE;
        count_busy--;
        pthread_mutex_unlock(&mutex_busy);

        sleep(2);
    }
    pthread_exit(NULL);
}

void version_busy_wait() {
    pthread_t prod_thread, cons_thread;

    pthread_mutex_init(&mutex_busy, NULL);

    pthread_create(&prod_thread, NULL, productor_busy, NULL);
    pthread_create(&cons_thread, NULL, consumidor_busy, NULL);

    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    pthread_mutex_destroy(&mutex_busy);
}