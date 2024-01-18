#ifndef MESSAGE_H
#define MESSAGE_H

#include "protocol.h"


//Election
typedef struct {
    char identifiant[ENTITY_ID_SIZE];
    char question[256];
    char dateDebut[256];
    char dateFin[256];
    StatusElection status;
}  AjoutElectionCmd;

typedef struct {
    char identifiant[ENTITY_ID_SIZE];
}  SupprimeElectionCmd;

typedef struct {
    char identifiant[ENTITY_ID_SIZE];
}  LireElectionCmd;

typedef struct {
    char identifiant[ENTITY_ID_SIZE];
    char question[256];
    char dateDebut[256];
    char dateFin[256];
    StatusElection status;
}  MiseAJourElectionCmd;

//Vote
typedef struct {
    char idVotant[ENTITY_ID_SIZE];
    char idElection[ENTITY_ID_SIZE];
    char timestamp[256];
    char bulletin[256];
    char hashValidation[256];
}  AjoutVoteCmd;

typedef struct {
    char identifiant[ENTITY_ID_SIZE];
}  SupprimeVoteCmd;

typedef struct {
    char identifiant[ENTITY_ID_SIZE];
}  LireVoteCmd;

typedef struct {
    char idVotant[ENTITY_ID_SIZE];
    char idElection[ENTITY_ID_SIZE];
    char timestamp[256];
    char bulletin[256];
    char hashValidation[256];
}  MiseAJourVoteCmd;

//Electeur

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} AjoutElecteurCmd;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} SupprimeElecteurCmd;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} EstPresentCmd;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} LireElecteurCmd;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} MiseAJourElecteurCmd;

typedef enum
{
    NOP = 0,
    AJOUT_ELECTEUR,
    SUPPRIME_ELECTEUR,
    EST_PRESENT,
    LIRE_ELECTEUR,
    MISE_A_JOUR_ELECTEUR,
    AJOUT_ELECTION,
    SUPPRIME_ELECTION,
    LIRE_ELECTION,
    MISE_A_JOUR_ELECTION,
    AJOUT_VOTE,
    SUPPRIME_VOTE,
    LIRE_VOTE,
    MISE_A_JOUR_VOTE,
} CommandType;

//--
typedef struct
{
    CommandType type;
    char signature[256]; // la signature de la commande
    union
    {
        AjoutElecteurCmd ajoutElecteur;
        SupprimeElecteurCmd supprimeElecteur;
        EstPresentCmd estPresent;
        MiseAJourElecteurCmd miseAJourElecteur;
        LireElecteurCmd lireElecteur;
        AjoutElectionCmd ajoutElection;
        SupprimeElectionCmd supprimeElection;
        LireElectionCmd lireElection;
        MiseAJourElectionCmd miseAJourElection;
        AjoutVoteCmd ajoutVote;
        SupprimeVoteCmd supprimeVote;
        LireVoteCmd lireVote;
        MiseAJourVoteCmd miseAJourVote;
    } commande;
} Commande;

#endif