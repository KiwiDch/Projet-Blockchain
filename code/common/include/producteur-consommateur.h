#ifndef PRODUCTEURCONSOMMATEUR
#define PRODUCTEURCONSOMMATEUR
#include <pthread.h>
#include <semaphore.h>
#include "messages.h"

#define SIZE 2048

typedef struct {
    sem_t plein;
    sem_t vide;
    sem_t mutex;

    char buffer[SIZE]; //Pourrait utiliser le tas/un pointeur pour faciliter le partage entre thread
    int index_in;
    int index_out;
} ProducterConsummer;

void* init_sems(sem_t *plein, int nb_consummers, sem_t *vide, int nb_producter, sem_t *mutex);

void* product(ProducterConsummer* pc, char* transaction, size_t size);

void* consume(ProducterConsummer* pc, char* dest, size_t size);


#endif
