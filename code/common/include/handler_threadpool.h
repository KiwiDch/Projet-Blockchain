#ifndef HANDLER_THREAD
#define HANDLER_THREAD

#include <glib.h>
#include "threadpool.h"
#include "handler.h"
#include "crypto.h"

typedef struct {
    ThreadPool pool;
    Arg arguments;
    Future* futures;
} ThreadHandler;

bool init_ThreadHandler(ThreadHandler* handler, int pool_size, int queuesize, char* db_path,ProducterConsummer* entree, ProducterConsummer* sortie); //initialise les arg etc..

bool spawn_handler(ThreadHandler* handler); // fait spawner un handler

void freeElement(gpointer key, gpointer value, gpointer user_data);

void destroy_ThreadHandler(ThreadHandler* handler); // detruit proprement

#endif