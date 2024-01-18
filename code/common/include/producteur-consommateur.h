#ifndef PRODUCTEURCONSOMMATEUR
#define PRODUCTEURCONSOMMATEUR
#include <pthread.h>
#include <semaphore.h>
#include "messages.h"

#define SIZE 50

typedef struct {
    sem_t plein;
    sem_t vide;
    sem_t mutex;

    Commande buffer[SIZE];
    int index_in;
    int index_out;
} ProducterConsummer;

void* init_sems(sem_t *plein, int nb_consummers, sem_t *vide, int nb_producter, sem_t *mutex);

void* product(ProducterConsummer* pc, Commande transaction);

Commande consume(ProducterConsummer* pc);


#endif
