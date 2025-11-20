#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "barrera.h"

typedef struct {
    int tid;
    int etapas;
    barrier_t *bar;
} thread_arg_t;

void* trabajador(void *arg) {
    thread_arg_t *a = (thread_arg_t*)arg;

    for (int e = 0; e < a->etapas; e++) {
        usleep(100000 + rand() % 200000);

        printf("[H%d] esperando en etapa %d\n", a->tid, e);

        barrier_wait(a->bar);

        printf("[H%d] paso barrera en etapa %d\n", a->tid, e);
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    int N = (argc > 1) ? atoi(argv[1]) : 5;
    int E = (argc > 2) ? atoi(argv[2]) : 4;

    pthread_t hilos[N];
    thread_arg_t args[N];

    barrier_t barrera;
    barrier_init(&barrera, N);

    printf("Creando %d hebras, %d etapas...\n", N, E);

    for (int i = 0; i < N; i++) {
        args[i].tid = i;
        args[i].etapas = E;
        args[i].bar = &barrera;
        pthread_create(&hilos[i], NULL, trabajador, &args[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(hilos[i], NULL);
    }

    barrier_destroy(&barrera);
    return 0;
}
