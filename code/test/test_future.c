#include "../common/include/threadpool.h"

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

// Exemple de fonction à exécuter par le Future
void *maFonction(void *arg)
{
    // Votre code ici
    sleep(5);
    return "test is ok\n";
}

void *maFonctionErreur(void *arg)
{
    // Votre code ici
    sleep(3);
    return "test is not ok\n";
}

// Programme principal
int main()
{
    ThreadPool pool;
    if (!initThreadPool(&pool, 4, 10))
    {
        fprintf(stderr, "Erreur lors de l'initialisation du pool de threads\n");
        return 1;
    }

    // Créer et configurer une tâche Future
    Future myTask1;
    futureInit(&myTask1, maFonction, NULL);

    Future myTask2;
    futureInit(&myTask2, maFonctionErreur, NULL);

    // Soumettre la tâche au pool
    if (!submitTaskToPool(&pool, &myTask1))
    {
        fprintf(stderr, "Erreur lors de la soumission de la tâche\n");
    }

    // Soumettre la tâche au pool
    if (!submitTaskToPool(&pool, &myTask2))
    {
        fprintf(stderr, "Erreur lors de la soumission de la tâche\n");
    }

    // Attendre et récupérer le résultat
    void *result = futureGet(&myTask1);
    // Utiliser le résultat...
    g_print("result %s", (char *)result);

    result = futureGet(&myTask2);
    // Utiliser le résultat...
    g_print("result %s", (char *)result);

    destroyThreadPool(&pool);
    return 0;
}
