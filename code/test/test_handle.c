#include "../common/include/handler.h"
#include "../common/include/messages.h"
#include <string.h>
#include <stdio.h>

int main () {
    AjoutElecteurCmd ajout;
    strcpy(ajout.identifiant, "coucou");

    Commande cmd;
    cmd.type = AJOUT_ELECTEUR;
    strcpy(cmd.signature, "coucou");
    cmd.commande.ajoutElecteur = ajout;

    handle(cmd);
}