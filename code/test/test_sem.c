#include "../common/include/handler.h"
#include "../common/include/messages.h"
#include "../common/include/producteur-consommateur.h"
#include "../common/include/handler.h"
#include "../common/include/bd.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <pthread.h>

ProducterConsummer pc;
ProducterConsummer message;
sqlite3* bd;
pthread_mutex_t mutex_bd = PTHREAD_MUTEX_INITIALIZER;

Arg arg;

void* listen_messages(void* mess){
    ProducterConsummer* pc_message = (ProducterConsummer*) mess;
    printf("Ecoute\n");
    fflush(stdout);
    int arret = 0;
    while(!arret) {
        Message m;
        consume(pc_message, (char*) &m, sizeof(Message));
        arret = m.arret;
        printf("Message recu: %s\n", m.msg);
        fflush(stdout);
    }
}

int main () {
    bd = database_open("./bd.sqlite");
    database_init(bd);

    sem_t mutex, vide, plein;
    pthread_t thread,thread_messages;
    printf("init\n");
    fflush(stdout);

    init_sems(&plein, 1, &vide, 1, &mutex);
    pc.plein = plein;
    pc.vide = vide;
    pc.mutex = mutex;


    sem_t mutex2, vide2, plein2;
    init_sems(&plein2, 1, &vide2, 1, &mutex2);
    message.plein = plein2;
    message.vide = vide2;
    message.mutex = mutex2;

    printf("create thread\n");
    fflush(stdout);

    AjoutElecteurCmd ajout;
    strcpy(ajout.identifiant, "coucou");

    Commande cmd;
    cmd.type = AJOUT_ELECTEUR;
    strcpy(cmd.signature, "coucou");
    cmd.commande.ajoutElecteur = ajout;

    AjoutElectionCmd election;
    strcpy(election.identifiant, "coucou");
    strcpy(election.question, "auugh ?");
    strcpy(election.dateDebut, "27/01/2023");
    strcpy(election.dateFin, "28/01/2023");

    Commande cmd2;
    cmd2.type = NOP;
    strcpy(cmd2.signature, "election");
    cmd2.commande.ajoutElection = election;
    

    product(&pc, (char*) &cmd,sizeof(Commande));
    


    arg.pc = &pc;
    arg.messages = &message;
    arg.db_mutex = &mutex_bd;
    arg.db = bd;

    if(pthread_create(&thread_messages, NULL, listen_messages, &message) != 0){
        printf("saduuqsdddddsdddqsddddgdsdddh");
        return -1;
    }; 

    if(pthread_create(&thread, NULL, run, &arg) != 0){
        printf("sauuqsdddsddqsdddgsdddh");
        return -1;
    };  
    //product(&pc, cmd);
    //product(&pc, cmd);

    product(&pc, (char *) &cmd2,sizeof(Commande));
    pthread_join(thread, NULL);
}
