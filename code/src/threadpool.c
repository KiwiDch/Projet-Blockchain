#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "../common/include/threadpool.h"

// Fonction exécutée par chaque thread du pool
void *thread_do(void *threadpool)
{
    ThreadPool *pool = (ThreadPool *)threadpool;

    while (true)
    {
        pthread_mutex_lock(&pool->lock);

        // Attendre une tâche
        while ((pool->count == 0) && (!pool->shutdown))
        {
            pthread_cond_wait(&pool->notify, &pool->lock);
        }

        if (pool->shutdown)
        {
            break;
        }
        // Récupérer la tâche
        Future *task = pool->tasks[pool->head];
        pool->head = (pool->head + 1) % pool->queue_size;
        pool->count--;

        pthread_mutex_unlock(&pool->lock);
        // Exécuter la tâche
        task->result = task->function(task->arg);
        pthread_mutex_lock(&task->mutex);
        task->completed = true;
        pthread_cond_signal(&task->cond);
        pthread_mutex_unlock(&task->mutex);
    }

    pthread_mutex_unlock(&pool->lock);
    pthread_exit(NULL);
    return NULL;
}

// Initialisation du pool de threads
bool initThreadPool(ThreadPool *pool, int numThreads, int queueSize)
{
    if (numThreads <= 0 || queueSize <= 0)
    {
        return false;
    }

    pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * numThreads);
    pool->tasks = (Future **)malloc(sizeof(Future *) * queueSize);

    pool->thread_count = numThreads;
    pool->queue_size = queueSize;
    pool->head = pool->tail = pool->count = 0;
    pool->shutdown = false;

    pthread_mutex_init(&(pool->lock), NULL);
    pthread_cond_init(&(pool->notify), NULL);

    for (int i = 0; i < numThreads; i++)
    {
        if (pthread_create(&(pool->threads[i]), NULL, thread_do, (void *)pool) != 0)
        {
            // En cas d'erreur, nettoyer et retourner false
            free(pool->threads);
            free(pool->tasks);
            return false;
        }
    }

    return true;
}

// Destruction du pool de threads
void destroyThreadPool(ThreadPool *pool)
{
    pool->shutdown = true;
    pthread_cond_broadcast(&(pool->notify));

    for (int i = 0; i < pool->thread_count; i++)
    {
        pthread_join(pool->threads[i], NULL);
    }
    free(pool->threads);
    free(pool->tasks);
    pthread_mutex_destroy(&(pool->lock));
    pthread_cond_destroy(&(pool->notify));
}

// Soumettre une tâche au pool de threads
bool submitTaskToPool(ThreadPool *pool, Future *task)
{

    pthread_mutex_lock(&pool->lock);

    if (pool->count == pool->queue_size || pool->shutdown)
    {
        pthread_mutex_unlock(&pool->lock);
        return false;
    }

    pool->tasks[pool->tail] = task;
    pool->tail = (pool->tail + 1) % pool->queue_size;
    pool->count++;

    pthread_cond_signal(&pool->notify);
    pthread_mutex_unlock(&pool->lock);

    return true;
}

// Fonction pour récupérer le résultat d'une tâche Future
void *futureGet(Future *future)
{
    pthread_mutex_lock(&future->mutex);
    while (!future->completed)
    {
        pthread_cond_wait(&future->cond, &future->mutex);
    }
    void *result = future->result;
    pthread_mutex_unlock(&future->mutex);
    return result;
}

// Fonction pour initialiser un Future
void futureInit(Future *future, void *(*function)(void *), void *arg)
{
    future->function = function;
    future->arg = arg;
    future->completed = false;
    future->result = NULL;
    pthread_mutex_init(&future->mutex, NULL);
    pthread_cond_init(&future->cond, NULL);
}
