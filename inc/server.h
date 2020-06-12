#pragma once

#include "uchat.h"
#define MX_MAX_THREADS 50

//STRUCTURES
typedef struct communication t_comm;
typedef struct threads t_threads;
typedef struct connected_clients t_cli;
typedef char *(*api_function)(void *, t_list **) ;

struct communication {
    int connection_fd;
    char *status;
    t_list **clients;
};

struct threads {
    pthread_t *threads;
    char *status;
};

struct connected_clients {
    int connection_fd;
    int user_id;
};

//FUNCTIONS
t_threads *mx_init_threads(void);
void mx_thread_manager(pthread_t **threads_ptr, char **status_ptr, 
                       int connection_fd, t_list **clients);
void *mx_communicate(void *data);
char *mx_process_request(char *request, t_list **clients);
char *mx_register_user(void *jobj, t_list **clients);
