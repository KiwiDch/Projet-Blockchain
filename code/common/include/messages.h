#ifndef MESSAGE_H
#define MESSAGE_H

#include "protocol.h"
#include "crypto.h"

//Election
typedef struct { // a simplfiier, status peut etre determiné
    char identifiant[ENTITY_ID_SIZE];
    char question[256];
    char dateDebut[256];
    char dateFin[256];
    //StatusElection status;
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
}  MiseAJourElectionCmd;

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
    char nouveau_identifiant[ENTITY_ID_SIZE];
} MiseAJourElecteurCmd;

typedef struct {
    char identifiant_votant[ENTITY_ID_SIZE];
    char identifiant_election[256];
    mpz_t bulletin;
    char hash_validation[256];
} VoteCmd;

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
    VOTE,
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
        VoteCmd voterElection;
    } commande;
} Commande;


typedef enum {
    String,
    AjoutElectionReponse,
    Stop
} MessageType;

typedef struct { //message vers l'exterieur
    MessageType type;
    union {
        char msg[256];
        PublicKey key;
    } message;
} Message;



#endif
