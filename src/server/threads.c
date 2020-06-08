#include "uchat.h"
#include "server.h"

static int get_free_thread(char *status, int *counter) {
    for (int j = 0; j < MX_MAX_THREADS; j++)
        if (status[j] == 0) {
            *counter = j;
            return 0;
        }
    return 1;
}

t_comm *init_data(int socket_fd, int connection_fd, char *status) {
    t_comm *data = malloc(sizeof(t_comm));

    data->socket_fd = socket_fd;
    data->connection_fd = connection_fd;
    data->status = status;

    return data;
}

void mx_thread_manager(pthread_t **threads_ptr, char **status_ptr,
                       int socket_fd, int connection_fd) {
    t_comm *data = NULL;
    int counter = 0;
    int free_thread = -1;
    char *status = *status_ptr;
    pthread_t *thr = *threads_ptr;

    while ((free_thread = get_free_thread(*status_ptr, &counter)) != 0)
        printf("%d\n", free_thread);

    data = init_data(socket_fd, connection_fd, &status[counter]);
    status[counter] = 1;
    if (pthread_create(&thr[counter], NULL, mx_communicate, (void *)data) == 0)
        printf("Connected to client!\n");
    else
        printf("Connection failed :(\n");
}
