#include "../common/include/producteur-consommateur.h"
#include <stdio.h>

void* init_sems(sem_t *plein, int nb_consummers, sem_t *vide, int nb_producter, sem_t *mutex) {
    sem_init(plein, 0, nb_consummers);
    sem_init(vide, 0, nb_producter);
    sem_init(mutex, 0, 1);
}

void* product(ProducterConsummer* pc, Commande transaction) {
    printf("product\n");
    fflush(stdout);
    sem_wait(&(pc->vide));
    sem_wait(&(pc->mutex));
    pc->buffer[pc->index_in] = transaction;
    pc->index_in = (pc->index_in + 1) % SIZE;
    printf("producted\n");
    fflush(stdout);
    sem_post(&(pc->mutex));
    sem_post(&(pc->plein));
}

Commande consume(ProducterConsummer* pc) {
    printf("consume \n");
    fflush(stdout);
    sem_wait(&(pc->plein));
    sem_wait(&(pc->mutex));
    pc->index_out = (pc->index_out + 1) % SIZE;
    Commande value = pc->buffer[pc->index_out];
    printf("consumed \n");
    fflush(stdout);
    sem_post(&(pc->mutex));
    sem_post(&(pc->vide));
    return value;
}
