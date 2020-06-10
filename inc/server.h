#pragma once

#define MX_MAX_THREADS 50

//STRUCTURES
typedef struct communication t_comm;

struct communication {
    int connection_fd;
    char *status;
};

//FUNCTIONS
void mx_thread_manager(pthread_t **threads_ptr, char **status_ptr,
                       int connection_fd);
void *mx_communicate(void *data);
