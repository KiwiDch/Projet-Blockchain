#ifndef THREADPOOL__
#define THREADPOOL__

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

// Définition de la structure Future
typedef struct
{
    void *(*function)(void *); // Fonction à exécuter
    void *arg;                 // Argument de la fonction
    void *result;              // Résultat de la fonction
    bool completed;            // Indique si la tâche est terminée
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Future;

// Définition de la structure ThreadPool
typedef struct
{
    pthread_mutex_t lock;
    pthread_cond_t notify;
    pthread_t *threads;
    Future **tasks;
    int thread_count;
    int queue_size;
    int head;
    int tail;
    int count;
    bool shutdown;
} ThreadPool;

bool initThreadPool(ThreadPool *pool, int numThreads, int queueSize);

void destroyThreadPool(ThreadPool *pool);

bool submitTaskToPool(ThreadPool *pool, Future *task);

void *futureGet(Future *future);

void futureInit(Future *future, void *(*function)(void *), void *arg);

#endif