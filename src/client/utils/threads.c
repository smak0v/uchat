#include "client.h"

void mx_client_thread_manager(t_glade *glade, SSL *ssl) {
    pthread_t thread_id;
    t_thread_data *data = malloc(sizeof(t_thread_data));

    data->ssl = ssl;
    data->glade = glade;

    pthread_create(&thread_id, NULL, mx_listen_server, (void *)data);

    glade->listener = thread_id;
}
