#include "uchat.h"

static int get_free_thread(char *status, int *counter) {
    for (int j = 0; j < MX_MAX_THREADS; j++)
        if (status[j] == 0) {
            *counter = j;
            return 0;
        }

    return 1;
}

static t_comm *init_data(int connection_fd, char *status, t_meta **metadata) {
    t_comm *data = malloc(sizeof(t_comm));

    data->fd = connection_fd;
    data->status = status;
    data->db = (*metadata)->db;

    return data;
}

t_meta *mx_init_threads(sqlite3 *db) {
    pthread_t *threads = malloc(sizeof(pthread_t) * MX_MAX_THREADS);
    char *status = malloc(sizeof(char) * MX_MAX_THREADS);
    t_meta *data = malloc(sizeof(t_meta));

    mx_memset(status, 0, MX_MAX_THREADS);
    data->threads = threads;
    data->status = status;
    data->db = db;

    return data;
}

void mx_thread_manager(int connection_fd, t_meta **metadata) {
    t_comm *data = NULL;
    int counter = 0;
    int free_thread = -1;
    char *status = (*metadata)->status;
    pthread_t *thr = (*metadata)->threads;

    while ((free_thread = get_free_thread((*metadata)->status, &counter)) != 0)
        printf("%d\n", free_thread);

    data = init_data(connection_fd, &status[counter], metadata);
    status[counter] = 1;
    if (pthread_create(&thr[counter], NULL, mx_communicate, (void *)data) == 0)
        printf("Connected to client!\n");
    else
        printf("Connection failed :(\n");
}
