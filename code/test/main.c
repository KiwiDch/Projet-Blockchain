#include "../common/include/threadpool.h"
#include "../common/include/handler.h"
#include "../common/include/messages.h"
#include "../common/include/producteur-consommateur.h"
#include "../common/include/bd.h"
#include "../common/include/handler_threadpool.h"
#include "../common/include/protocol.h"
#include "../common/include/crypto.h"


ProducterConsummer pc;
ProducterConsummer message;
ThreadHandler handler;

void* emission(void * arg) {
    //en attandant les sockets

    Commande cmd;
    cmd.type = AJOUT_ELECTION;
    strcpy(cmd.signature, "coucou");
    AjoutElectionCmd election;
    strcpy(election.identifiant, "coucou");
    strcpy(election.question, "auugh ?");
    strcpy(election.dateDebut, "27/01/2023");
    strcpy(election.dateFin, "28/01/2023");

    cmd.commande.ajoutElection = election;
    //while(!handler.pool.shutdown){
    //for(int i = 0; i< 2; i++){
        if(spawn_handler(&handler) != true){
            printf("probleme\n");
            fflush(stdout);
        }
        else {
            printf("type %i", cmd.type);
            product(&pc, (char *)&cmd, sizeof(Commande));
            printf("command submited\n");
            fflush(stdout);
        }



    //}
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