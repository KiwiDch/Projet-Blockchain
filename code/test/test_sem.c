#include "../common/include/handler.h"
#include "../common/include/messages.h"
#include "../common/include/producteur-consommateur.h"
#include "../common/include/handler.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>

ProducterConsummer pc;

int main () {
    sem_t mutex, vide, plein;
    pthread_t thread;
    printf("init\n");
    fflush(stdout);
    init_sems(&plein, 1, &vide, 1, &mutex);

    pc.plein = plein;
    pc.vide = vide;
    pc.mutex = mutex;

    printf("create thread\n");
    fflush(stdout);

    AjoutElecteurCmd ajout;
    strcpy(ajout.identifiant, "coucou");

    Commande cmd;
    cmd.type = AJOUT_ELECTEUR;
    strcpy(cmd.signature, "coucou");
    cmd.commande.ajoutElecteur = ajout;
    

    product(&pc, cmd);
    if(pthread_create(&thread, NULL, run, &pc) != 0){
        printf("sauuqsdddgdddh");
        return -1;
    };  
    product(&pc, cmd);
    product(&pc, cmd);


    pthread_join(thread, NULL);
}
