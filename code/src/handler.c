#include "../common/include/handler.h"

Message write_message(char * msg){
    Message m;
    m.type = String;
    strcpy(m.message.msg, msg);
    return m;
}

bool isInDateRange(const char *startDateStr, const char *endDateStr) {
    // Obtenir la date actuelle
    time_t currentTime;
    time(&currentTime);
    struct tm currentDate;
    localtime_r(&currentTime, &currentDate);

    // Convertir les chaînes de caractères en structures tm
    struct tm startDate, endDate;
    memset(&startDate, 0, sizeof(struct tm));
    memset(&endDate, 0, sizeof(struct tm));

    if (sscanf(startDateStr, "%d/%d/%d", &startDate.tm_mday, &startDate.tm_mon, &startDate.tm_year) != 3 ||
        sscanf(endDateStr, "%d/%d/%d", &endDate.tm_mday,&endDate.tm_mon,&endDate.tm_year) != 3) {
        // Échec de la conversion des chaînes de caractères en dates
        return false;
    }

    // Les années dans la structure tm sont comptées à partir de 1900
    startDate.tm_year -= 1900;
    endDate.tm_year -= 1900;

    // Les mois dans la structure tm sont comptés de 0 à 11
    startDate.tm_mon -= 1;
    endDate.tm_mon -= 1;

    // Convertir les structures tm en timestamps
    time_t startTimestamp = mktime(&startDate);
    time_t endTimestamp = mktime(&endDate);
    time_t currentTimestamp = mktime(&currentDate);

    // Vérifier si la date actuelle est dans l'intervalle
    return (currentTimestamp >= startTimestamp && currentTimestamp <= endTimestamp);
}

Message handle_lire_electeur(HandlerContext* context, LireElecteurCmd* cmd) {
    pthread_mutex_lock(context->db_mutex);
    Electeur e = readElecteur(context->db,cmd->identifiant,strlen(cmd->identifiant));
    pthread_mutex_unlock(context->db_mutex);
    Message m;
    m.type = String;
    sprintf(m.message.msg,"Id de l'electeur: %s", e.numeroID);
    //printf("Lecture");
    return m;
}

void handle_mise_a_jour_electeur(HandlerContext* context, MiseAJourElecteurCmd* cmd){
    pthread_mutex_lock(context->db_mutex);
    updateElecteur(context->db,cmd->identifiant,strlen(cmd->identifiant), cmd->nouveau_identifiant, strlen(cmd->identifiant));
    pthread_mutex_unlock(context->db_mutex);

    printf("Mise a jour");
}

Message handle_ajout_electeur(HandlerContext* context, AjoutElecteurCmd* cmd){
    pthread_mutex_lock(context->db_mutex);
    if(electeurExists(context->db,cmd->identifiant,strlen(cmd->identifiant)) != 0){ //Si electeur est deja la, erreur
        return write_message("Existe déja");
    }
    createElecteur(context->db,cmd->identifiant,strlen(cmd->identifiant));
    pthread_mutex_unlock(context->db_mutex);

    return write_message("Ajouté");
}

void handle_supression_electeur(HandlerContext* context, SupprimeElecteurCmd* cmd){
    pthread_mutex_lock(context->db_mutex);
    deleteElecteur(context->db,cmd->identifiant,strlen(cmd->identifiant));
    pthread_mutex_unlock(context->db_mutex);

    printf("Suppression");
}

Message handle_est_present(HandlerContext* context, EstPresentCmd* cmd){
    pthread_mutex_lock(context->db_mutex);
    int estPresent = electeurExists(context->db,cmd->identifiant,strlen(cmd->identifiant));
    pthread_mutex_unlock(context->db_mutex);

    Message m;
    m.type = String;
    strcpy(m.message.msg, estPresent ? "Oui": "Non");
    return m;
}

Message handle_ajout_election(HandlerContext* context, AjoutElectionCmd* cmd){

    pthread_mutex_lock(context->db_mutex);
    if(Election_getIdFromNumeroID(context->db, cmd->identifiant, strlen(cmd->identifiant)) != -1) { // s'il existe deja, erreur
        return write_message("Existe déja");
    }

    createElection(context->db,cmd->identifiant,strlen(cmd->identifiant),cmd->question,cmd->dateDebut,cmd->dateFin,"active");
    pthread_mutex_unlock(context->db_mutex);

    //generation clé
    KeyPair* keypair = malloc(sizeof(KeyPair));
    generate_KeyPair(keypair, 2048);

    pthread_mutex_lock(context->keys_mutex);
    g_hash_table_insert(context->keys,g_strdup(cmd->identifiant), keypair);
    pthread_mutex_unlock(context->keys_mutex);

    Message m;
    m.type = AjoutElectionReponse;
    m.message.key = keypair->public;

    return m;
}

void handle_supression_election(HandlerContext* context, SupprimeElectionCmd* cmd){
    pthread_mutex_lock(context->db_mutex);
    int id = Election_getIdFromNumeroID(context->db, cmd->identifiant, strlen(cmd->identifiant));
    deleteElection(context->db, id);
    pthread_mutex_unlock(context->db_mutex);

    printf("supression election");
}

Message handle_lire_election(HandlerContext* context, LireElectionCmd* cmd){
    pthread_mutex_lock(context->db_mutex);
    int id = Election_getIdFromNumeroID(context->db, cmd->identifiant, strlen(cmd->identifiant));
    Election election = readElection(context->db, id);
    pthread_mutex_unlock(context->db_mutex);
    Message m;
    m.type = String;

    char status[10];
    Status_into_chars(election.status, status);

    sprintf(m.message.msg, "id: %s, question: %s, dateDebut: %s, dateFin: %s, status: %s", election.identifiant, election.question, election.dateDebut, election.dateFin, status);
    printf("lire election");
    

    return m;
}

void handle_mise_a_jour_election(HandlerContext* context, MiseAJourElectionCmd* cmd){
    pthread_mutex_lock(context->db_mutex);
    int id = Election_getIdFromNumeroID(context->db, cmd->identifiant, strlen(cmd->identifiant));
    updateElection(context->db, id, cmd->question);
    pthread_mutex_unlock(context->db_mutex);

    printf("mise a jour election");
}


Message handle_voter_election(HandlerContext* context, VoteCmd* cmd){
    pthread_mutex_lock(context->db_mutex);
    int id = getIdFromNumeroID(context->db, cmd->identifiant_votant, strlen(cmd->identifiant_votant));
    int id_election = Election_getIdFromNumeroID(context->db, cmd->identifiant_election, strlen(cmd->identifiant_election));
    if(id  == -1 || id_election == -1){
        return write_message("Election ou Utilisateur inexistant");
    }


    Election e = readElection(context->db, id_election);

    if(e.status != active){
        return write_message("Election non active");
    }

    if(isInDateRange(e.dateDebut,e.dateFin) == false){
        return write_message("Date dépassé");
    }

    const char* hash;
    hash = cmd->hash_validation;

    Election_castVote(context->db, id, id_election, (void *) &cmd->bulletin, sizeof(mpz_t), hash);
    pthread_mutex_unlock(context->db_mutex);
    return write_message("a voter");
}

Message handle(HandlerContext* context, Commande cmd) {

    switch (cmd.type) {
        case NOP: {
            printf("Arret demandé\n");
            fflush(stdout);
            Message m;
            m.type = Stop;
            return m;
        }
        case AJOUT_ELECTEUR: {
            printf("Ajout electeur\n");
            fflush(stdout);
            return handle_ajout_electeur(context, &cmd.commande.ajoutElecteur);
        }
        case SUPPRIME_ELECTEUR: {
            handle_supression_electeur(context, &cmd.commande.supprimeElecteur);
            break;
        }
        case EST_PRESENT: {
            return handle_est_present(context, &cmd.commande.estPresent);
            break;
        }
        case LIRE_ELECTEUR: {
            return handle_lire_electeur(context, &cmd.commande.lireElecteur);
        }
        case MISE_A_JOUR_ELECTEUR: {
            handle_mise_a_jour_electeur(context, &cmd.commande.miseAJourElecteur);
	    break;
        }
        case AJOUT_ELECTION: {
            return handle_ajout_election(context, &cmd.commande.ajoutElection);
        }
        case SUPPRIME_ELECTION: {
            handle_supression_election(context, &cmd.commande.supprimeElection);
	    break;
        }
        case MISE_A_JOUR_ELECTION: {
            handle_mise_a_jour_election(context, &cmd.commande.miseAJourElection);
	    break;
        }
        case LIRE_ELECTION: {
            return handle_lire_election(context, &cmd.commande.lireElection);
        }
        case VOTE: {
            return handle_voter_election(context, &cmd.commande.voterElection);
        }
        default: {
	        printf("unknown\n");
            break;
        }
    }
    Message m;
    m.type = String;
    strcpy(m.message.msg,"Ok !");
    return m;
}

void* run(void *arg){

    Arg* config = (Arg *) arg;
    //context init
    HandlerContext context;
    context.db = config->db;
    context.db_mutex = config->db_mutex;
    context.keys_mutex = config->keys_mutex;
    context.keys = config->keys;

    ProducterConsummer *pc = (ProducterConsummer *) config->pc;
    ProducterConsummer *messages = (ProducterConsummer *) config->messages;
    //traitement
    Commande cmd;
    consume(pc, (char*) &cmd, sizeof(Commande));
    
    Message m = handle(&context, cmd);
    product(messages, (char*)&m, sizeof(Message));

    return NULL;
}

