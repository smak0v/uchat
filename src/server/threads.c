#include "uchat.h"

static int get_free_thread(char *status, int *counter) {
    for (int j = 0; j < MX_MAX_THREADS; j++)
        if (status[j] == 0) {
            *counter = j;
            return 0;
        }
    return 1;
}

static t_comm *init_data(int connection_fd, char *status, t_list **clients,
                         sqlite3 *db) {
    t_comm *data = malloc(sizeof(t_comm));

    data->connection_fd = connection_fd;
    data->status = status;
    data->clients = clients;
    data->db = db;

    return data;
}

t_threads *mx_init_threads(void) {
    pthread_t *threads = malloc(sizeof(pthread_t) * MX_MAX_THREADS);
    char *status = malloc(sizeof(char) * MX_MAX_THREADS);
    t_threads *data = malloc(sizeof(t_threads));

    mx_memset(status, 0, MX_MAX_THREADS);
    data->threads = threads;
    data->status = status;

    return data;
}

void mx_thread_manager(pthread_t **threads_ptr, char **status_ptr, 
                       int connection_fd, t_list **clients, sqlite3 *db) {
    t_comm *data = NULL;
    int counter = 0;
    int free_thread = -1;
    char *status = *status_ptr;
    pthread_t *thr = *threads_ptr;

    while ((free_thread = get_free_thread(*status_ptr, &counter)) != 0)
        printf("%d\n", free_thread);

    data = init_data(connection_fd, &status[counter], clients, db);
    status[counter] = 1;
    if (pthread_create(&thr[counter], NULL, mx_communicate, (void *)data) == 0)
        printf("Connected to client!\n");
    else
        printf("Connection failed :(\n");
}
