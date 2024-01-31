#ifndef BD__
#define BD__

#include <sqlite3.h>
#include <string.h>
#include "protocol.h"
#include <glib.h>
#include <gmp.h>
#include <stdbool.h>

sqlite3 *database_open(const char *path);
int database_close(sqlite3 *db);
int database_init(sqlite3 *db);
int database_exists(const char *path);

// usecases electeur
void createElecteur(sqlite3 *db, const char *numeroID, int size);
int electeurExists(sqlite3 *db, const char *numeroID, int size);
void deleteElecteur(sqlite3 *db, const char *numeroID, int size);
void updateElecteur(sqlite3 *db, const char *oldNumeroID, int size1, const char *newNumeroID, int size2);
Electeur readElecteur(sqlite3 *db, const char *numeroID, int size);
int getIdFromNumeroID(sqlite3 *db, const char *numeroID, int size);

// usecases election
void createElection(sqlite3 *db, const char *identifiant, int sizeId, const char *question, const char *dateDebut, const char *dateFin, const char *status);
void updateElection(sqlite3 *db, int id, const char *question, const char* status);
int Election_getIdFromNumeroID(sqlite3 *db, const char *numeroID, int size);
Election readElection(sqlite3 *db, int id);
void deleteElection(sqlite3 *db, int id);
void Election_castVote(sqlite3 *db, int idVotant, int idElection, const void *ballot, int ballotSize, const char *hashValidation);
// int Election_resultat(sqlite3 *db, int localid, int *numberOption0, int *numberOption1, int *totalVotes);
void Election_processVotes(sqlite3 *db, int electionId, mpz_t resultat_chiffre, int *p_totalvotes);
#endif
