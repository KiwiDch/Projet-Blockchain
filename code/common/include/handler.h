#ifndef HANDLER_H
#define HANDLER_H


#include "messages.h"
#include <stdio.h>
#include "bd.h"
#include "producteur-consommateur.h"
#include <glib.h>
#include <stdbool.h>


typedef struct {
	sqlite3* db;
	pthread_mutex_t* db_mutex;
    pthread_mutex_t* keys_mutex;
    GHashTable* keys;
} HandlerContext;

typedef struct {
	ProducterConsummer* pc;
	ProducterConsummer* messages;
	pthread_mutex_t* db_mutex;
    pthread_mutex_t* keys_mutex;
    GHashTable* keys;
	sqlite3* db;
} Arg;

Message write_message(char * msg);

bool isInDateRange(const char *startDateStr, const char *endDateStr);

void* run(void *arg);

Message handle(HandlerContext* context,Commande cmd);

Message handle_ajout_electeur(HandlerContext* context, AjoutElecteurCmd* cmd);
void handle_supression_electeur(HandlerContext* context, SupprimeElecteurCmd* cmd);
Message handle_est_present(HandlerContext* context, EstPresentCmd* cmd);
void handle_mise_a_jour_electeur(HandlerContext* context, MiseAJourElecteurCmd* cmd);
Message handle_lire_electeur(HandlerContext* context, LireElecteurCmd* cmd);

Message handle_ajout_election(HandlerContext* context, AjoutElectionCmd* cmd);
void handle_supression_election(HandlerContext* context, SupprimeElectionCmd* cmd);
Message handle_lire_election(HandlerContext* context, LireElectionCmd* cmd);
void handle_mise_a_jour_election(HandlerContext* context, MiseAJourElectionCmd* cmd);

Message handle_voter_election(HandlerContext* context, VoteCmd* cmd);
Message handler_resultat_election(HandlerContext* context, ResultatElectionCmd* cmd);
/*void handle_ajout_vote(HandlerContext* context, AjoutVoteCmd* cmd);
void handle_supression_vote(HandlerContext* context, SupprimeVoteCmd* cmd);
void handle_lire_vote(HandlerContext* context, LireVoteCmd* cmd);
void handle_mise_a_jour_vote(HandlerContext* context, MiseAJourVoteCmd* cmd);*/


#endif
