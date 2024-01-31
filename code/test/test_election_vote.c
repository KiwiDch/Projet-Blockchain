#include "../common/include/threadpool.h"
#include "../common/include/handler.h"
#include "../common/include/messages.h"
#include "../common/include/producteur-consommateur.h"
#include "../common/include/bd.h"
#include "../common/include/handler_threadpool.h"
#include "../common/include/protocol.h"
#include "../common/include/crypto.h"


#include <unistd.h>

//Ce test simule une election, il faut que la base de donnée soit supprimé

ProducterConsummer pc;
ProducterConsummer message;
ThreadHandler handler;

Commande cmdelection;
Commande cmdelecteur;
Commande cmdvote;
Commande cmdresultat;
Commande cmdstop;

Commande* cmd[5];

void* emission(void * arg) {

    for(int i = 0; i< 5; i++){
        if(spawn_handler(&handler) != true){
            printf("probleme\n");
            fflush(stdout);
        }
        else {
            product(&pc, (char *)cmd[i], sizeof(Commande));
            printf("command submited\n");
            fflush(stdout);
        }
        sleep(5); //pour le test c'est pour s'assurer que ca s'execute dans l'ordre
    }
}

void debug_print(Message* m) {
    switch(m->type) {
        case String:
            printf("Message recu: %s\n", m->message.msg);
            fflush(stdout);
            break;
        case AjoutElectionReponse:
            printf("clé recu: %Zd", m->message.key.g);
            fflush(stdout);

            mpz_t c;
            encrypt_vote(&m->message.key, 1,c);
            mpz_get_str(cmdvote.commande.voterElection.bulletin,10,c);

            printf("vote: %sd", cmdvote.commande.voterElection.bulletin);
            fflush(stdout);

            break;
        case ResultatElection:
            printf("nb de choix 1: %ld sur %i votes\n", m->message.resultat.choix1, m->message.resultat.total);
            break;
        case Stop:
            printf("STOP");
            fflush(stdout);
    }
}

void* reception(void * arg) {

    printf("Ecoute");
    fflush(stdout);
    while(!handler.pool.shutdown) {
        Message m;
        consume(&message, (char*) &m, sizeof(Message));

        debug_print(&m);

        if(m.type == Stop){
            printf("Arret en cours...");
            fflush(stdout);

            destroy_ThreadHandler(&handler);
        }
    }
}

int main(){

    printf("init\n");
    fflush(stdout);

    AjoutElecteurCmd ajout;
    strcpy(ajout.identifiant, "Elian");

    cmdelecteur.type = AJOUT_ELECTEUR;
    strcpy(cmdelecteur.signature, "coucou");
    cmdelecteur.commande.ajoutElecteur = ajout;

    cmdelection.type = AJOUT_ELECTION;
    strcpy(cmdelection.signature, "coucou");
    AjoutElectionCmd election;
    strcpy(election.identifiant, "coucou");
    strcpy(election.question, "auugh ?");
    strcpy(election.dateDebut, "27/01/2023");
    strcpy(election.dateFin, "28/01/2025");
    cmdelection.commande.ajoutElection = election;

    cmdvote.type = VOTE;
    strcpy(cmdvote.signature, "coucou");
    VoteCmd vote;
    strcpy(vote.identifiant_election, "coucou");
    strcpy(vote.identifiant_votant, "Elian");
    strcpy(vote.hash_validation, "hash");
    cmdvote.commande.voterElection = vote;

    cmdresultat.type = RESULTAT_ELECTION;
    strcpy(cmdresultat.signature, "coucou");
    ResultatElectionCmd rslt;
    strcpy(rslt.identifiant_election, "coucou");
    cmdresultat.commande.resultatElection = rslt;

    cmdstop.type = NOP;

    cmd[0] = &cmdelecteur;
    cmd[1] = &cmdelection;
    cmd[2] = &cmdvote;
    cmd[3] = &cmdresultat;
    cmd[4] = &cmdstop;

    if(init_ThreadHandler(&handler,5,10,":memory:",&pc,&message) == false){
        printf("probleme de thread\n");
        fflush(stdout);
        return -1;
    }

    pthread_t thread,thread_messages;
    
    if(pthread_create(&thread_messages, NULL, reception, NULL) != 0){
        printf("erreur");
        return -1;
    };

    if(pthread_create(&thread, NULL, emission, NULL) != 0){
        printf("erreur");
        return -1;
    };

    pthread_join(thread, NULL);
    pthread_join(thread_messages, NULL);

    printf("fini");
    fflush(stdout);
    return 0;
}