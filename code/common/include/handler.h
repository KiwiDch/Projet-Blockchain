#ifndef HANDLER_H
#define HANDLER_H
#include "messages.h"
#include <stdio.h>
#include "producteur-consommateur.h"


void* run(void *arg);

void handle(Commande cmd);

void handle_ajout_electeur(AjoutElecteurCmd* cmd);
void handle_supression_electeur(SupprimeElecteurCmd* cmd);
void handle_est_present(EstPresentCmd* cmd);
void handle_mise_a_jour_electeur(MiseAJourElecteurCmd* cmd);
void handle_lire_electeur(LireElecteurCmd* cmd);

void handle_ajout_election(AjoutElectionCmd* cmd);
void handle_supression_election(SupprimeElectionCmd* cmd);
void handle_lire_election(LireElectionCmd* cmd);
void handle_mise_a_jour_election(MiseAJourElectionCmd* cmd);

void handle_ajout_vote(AjoutVoteCmd* cmd);
void handle_supression_vote(SupprimeVoteCmd* cmd);
void handle_lire_vote(LireVoteCmd* cmd);
void handle_mise_a_jour_vote(MiseAJourVoteCmd* cmd);



#endif