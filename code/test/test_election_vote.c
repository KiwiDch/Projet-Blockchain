#include "../common/include/threadpool.h"
#include "../common/include/handler.h"
#include "../common/include/messages.h"
#include "../common/include/producteur-consommateur.h"
#include "../common/include/bd.h"
#include "../common/include/handler_threadpool.h"
#include "../common/include/protocol.h"
#include "../common/include/crypto.h"


#include <unistd.h>

ProducterConsummer pc;
ProducterConsummer message;
ThreadHandler handler;

Commande cmdelection;
Commande cmdelecteur;
Commande cmdvote;

Commande* cmd[3];

void* emission(void * arg) {

    for(int i = 0; i< 3; i++){
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
            memcpy(cmdvote.commande.voterElection.bulletin, c, sizeof(mpz_t));

            printf("vote: %Zd", cmdvote.commande.voterElection.bulletin);
            fflush(stdout);

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

    cmd[0] = &cmdelecteur;
    cmd[1] = &cmdelection;
    cmd[2] = &cmdvote;

    if(init_ThreadHandler(&handler,5,10,"bd.sqlite",&pc,&message) == false){
        printf("probleme de thread\n");
        fflush(stdout);
        return -1;
    }

    pthread_t thread,thread_messages;
    
    if(pthread_create(&thread_messages, NULL, reception, NULL) != 0){
        printf("saduuqsddddddsdddqsddddgdsdddh");
        return -1;
    };

    if(pthread_create(&thread, NULL, emission, NULL) != 0){
        printf("sauuqsdddsddqsdddgsdddh");
        return -1;
    };

    pthread_join(thread, NULL);
    pthread_join(thread_messages, NULL);

    printf("fini");
    fflush(stdout);
    return 0;
}