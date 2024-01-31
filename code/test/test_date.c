#include "../common/include/handler.h"
#include <stdbool.h>

/*int main(){
    bool i = isInDateRange("14/01/2023", "10/08/2025");
    printf("%i", i);
}*/

int main() {
    // Exemple d'utilisation
    const char *startDate = "10/10/2003";
    const char *endDate = "21/02/2025";

    if (isInDateRange(startDate, endDate)) {
        printf("La date d'aujourd'hui est dans l'intervalle spécifié.\n");
    } else {
        printf("La date d'aujourd'hui n'est pas dans l'intervalle spécifié.\n");
    }

    return 0;
}