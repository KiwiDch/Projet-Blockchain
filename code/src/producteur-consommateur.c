#include "../common/include/producteur-consommateur.h"
#include <stdio.h>

void* init_sems(sem_t *plein, int nb_consummers, sem_t *vide, int nb_producter, sem_t *mutex) {
    sem_init(plein, 0, nb_consummers);
    sem_init(vide, 0, nb_producter);
    sem_init(mutex, 0, 1);

    for(int i= 0; i < nb_consummers; i++){
	    sem_wait(plein); //pour ne pas consommer quand le buf est vide
    }
}

void* product(ProducterConsummer* pc, char* transaction, size_t size) {
    sem_wait(&(pc->vide));
    sem_wait(&(pc->mutex));
    for(int i = 0; i < size; i++){
        pc->buffer[(pc->index_in + i) % SIZE] = transaction[i];
    }
    pc->index_in = (pc->index_in + size) % SIZE;
    sem_post(&(pc->mutex));
    sem_post(&(pc->plein));
}

void* consume(ProducterConsummer* pc, char* dest, size_t size) {
    sem_wait(&(pc->plein));
    sem_wait(&(pc->mutex));
    for(int i = 0; i < size; i++){
        dest[i] = pc->buffer[(pc->index_out + i) % SIZE];
    }
    pc->index_out = (pc->index_out + size) % SIZE;
    sem_post(&(pc->mutex));
    sem_post(&(pc->vide));
}
