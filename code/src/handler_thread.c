#include "../common/include/handler_threadpool.h"

//mutex, peut etre les mettres dans init
pthread_mutex_t mutex_bd = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_keys = PTHREAD_MUTEX_INITIALIZER;

bool init_ThreadHandler(ThreadHandler* handler, int pool_size, int queuesize, char* db_path,ProducterConsummer* entree, ProducterConsummer* sortie){
    //arg
    Arg arg;

    //Arg - bd
    sqlite3* bd = database_open(db_path);
    database_init(bd);
 
    //Arg - keys

    GHashTable* keys = g_hash_table_new(g_str_hash, g_str_equal);

    //Arg - PC
    sem_t mutex_entree, vide_entree, plein_entree;
    sem_t mutex_sortie, vide_sortie, plein_sortie;
    init_sems(&plein_entree, 1, &vide_entree, 1, &mutex_entree);
    init_sems(&plein_sortie, 1, &vide_sortie, 1, &mutex_sortie);

    entree->plein = plein_entree;
    entree->mutex = mutex_entree;
    entree->vide = vide_entree;

    sortie->plein = plein_sortie;
    sortie->mutex = mutex_sortie;
    sortie->vide = vide_sortie;

    arg.db = bd;
    arg.db_mutex = &mutex_bd;
    arg.keys_mutex = &mutex_keys;
    arg.pc = entree;
    arg.messages = sortie;
    arg.keys = keys;

    handler->arguments = arg;

    //pool
    ThreadPool pool;
    handler->pool = pool;

    if(initThreadPool(&handler->pool, pool_size, queuesize) == false){
        return false;
    }

    //futures
    handler->futures = malloc(pool_size * sizeof(Future));

    for(int i = 0; i < pool_size; i++){
        Future f;
        f.completed = true; //pour dire qu'elle est disponible
        handler->futures[i] = f;
    }
    return true;
}

Future* find_available_future(Future* array, int size){
    for(int i = 0; i < size; i ++){
        if(array[i].completed == true){
            return &array[i];
        }
    }
    return NULL;
}

bool spawn_handler(ThreadHandler* handler){
    Future* future = find_available_future(handler->futures,handler->pool.thread_count);
    if(future == NULL) {
        return false;
    }
    
    futureInit(future, run, &handler->arguments);
    return submitTaskToPool(&handler->pool, future);
}

void freeElement(gpointer key, gpointer value, gpointer user_data) { // detruit chaque element
    KeyPair *myInstance = (KeyPair *)value;
    g_free(myInstance);
}


void destroy_ThreadHandler(ThreadHandler* handler) {
    destroyThreadPool(&handler->pool);
    g_hash_table_foreach(handler->arguments.keys, freeElement, NULL);
    g_hash_table_destroy(handler->arguments.keys);
    free(handler->futures);
}

