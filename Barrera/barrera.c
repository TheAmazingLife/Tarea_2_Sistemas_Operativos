#include "barrera.h"

void barrier_init(barrier_t *b, int N) {
    b->count = 0;
    b->N = N;
    b->etapa = 0;
    pthread_mutex_init(&b->lock, NULL);
    pthread_cond_init(&b->cond, NULL);
}

void barrier_wait(barrier_t *b) {
    pthread_mutex_lock(&b->lock);

    int etapa_local = b->etapa;

    b->count++;

    if (b->count == b->N) {
        b->etapa++;
        b->count = 0;
        pthread_cond_broadcast(&b->cond);
    } else {
        while (etapa_local == b->etapa) {
            pthread_cond_wait(&b->cond, &b->lock);
        }
    }

    pthread_mutex_unlock(&b->lock);
}

void barrier_destroy(barrier_t *b) {
    pthread_mutex_destroy(&b->lock);
    pthread_cond_destroy(&b->cond);
}
