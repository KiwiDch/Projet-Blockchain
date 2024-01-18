#include "../common/include/handler.h"

void handle_lire_electeur(LireElecteurCmd* cmd) {
    printf("Lecture");
}

void handle_mise_a_jour_electeur(MiseAJourElecteurCmd* cmd) {
    printf("Mise a jour");
}

void handle_ajout_electeur(AjoutElecteurCmd* cmd) {
    printf("Ajout");
}

void handle_supression_electeur(SupprimeElecteurCmd* cmd) {
    printf("Suppression");
}

void handle_est_present(EstPresentCmd* cmd) {
    printf("est present");
}

void handle_ajout_election(AjoutElectionCmd* cmd) {
    printf("ajout election");
}

void handle_supression_election(SupprimeElectionCmd* cmd) {
    printf("supression election");
}

void handle_lire_election(LireElectionCmd* cmd){
    printf("lire election");
}

void handle_mise_a_jour_election(MiseAJourElectionCmd* cmd){
    printf("mise a jour election");
}

void handle_ajout_vote(AjoutVoteCmd* cmd){
    printf("ajout vote");
}
void handle_supression_vote(SupprimeVoteCmd* cmd){
    printf("supression vote");
}
void handle_lire_vote(LireVoteCmd* cmd){
    printf("lire vote");
}
void handle_mise_a_jour_vote(MiseAJourVoteCmd* cmd){
    printf("mise a jour vote");
}

void handle(Commande cmd) {
    switch (cmd.type) {
        case AJOUT_ELECTEUR: {
            handle_ajout_electeur(&cmd.commande.ajoutElecteur);
            break;
        }
        case SUPPRIME_ELECTEUR: {
            handle_supression_electeur(&cmd.commande.supprimeElecteur);
            break;
        }
        case EST_PRESENT: {
            handle_est_present(&cmd.commande.estPresent);
            break;
        }
        case LIRE_ELECTEUR: {
            handle_lire_electeur(&cmd.commande.lireElecteur);
        }
        case MISE_A_JOUR_ELECTEUR: {
            handle_mise_a_jour_electeur(&cmd.commande.miseAJourElecteur);
        }
        case AJOUT_ELECTION: {
            handle_ajout_election(&cmd.commande.ajoutElection);
        }
        case SUPPRIME_ELECTION: {
            handle_supression_election(&cmd.commande.supprimeElection);
        }
        case MISE_A_JOUR_ELECTION: {
            handle_mise_a_jour_election(&cmd.commande.miseAJourElection);
        }
        case LIRE_ELECTION: {
            handle_lire_election(&cmd.commande.lireElection);
        }
        case AJOUT_VOTE: {
            handle_ajout_vote(&cmd.commande.ajoutVote);
        }
        case SUPPRIME_VOTE: {
            handle_supression_vote(&cmd.commande.supprimeVote);
        }
        case MISE_A_JOUR_VOTE: {
            handle_mise_a_jour_vote(&cmd.commande.miseAJourVote);
        }
        case LIRE_VOTE: {
            handle_lire_vote(&cmd.commande.lireVote);
        }
        default: {
            break;
        }
    }
}

void* run(void *arg){
    ProducterConsummer *pc = (ProducterConsummer *) arg;
    for(int i = 0; i < 10; i++){
        Commande cmd = consume(pc);
        handle(cmd);
    }
    return NULL;
}

