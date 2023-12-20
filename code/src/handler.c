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
        default: {
            break;
        }
    }
}

