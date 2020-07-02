#include "uchat.h"

void mx_client_thread_manager(char *ip, int port, t_glade *glade) {
    pthread_t thread_id;
    t_thread_data *data = malloc(sizeof(t_thread_data));

    data->ip = ip;
    data->port = port;
    data->glade = glade;

    pthread_create(&thread_id, NULL, mx_start_client, (void *)data);
}
