#include "server.h"

static void *mx_receive_file(void *void_data) {
    t_ft_data *data = (t_ft_data *)void_data;
    struct sockaddr_in cli_addr;
    unsigned int len = sizeof(cli_addr);
    int connect = accept(data->sock, (MX_SA *)&cli_addr, &len);

    if (connect < 0)
        pthread_exit(NULL);

    mx_recv_file(data->name, connect);

    *(data->status) = 0;
    pthread_exit(NULL);
}

// TODO: REFACTOR
char *mx_file_transfer(t_comm *connect, char *file, char *res, int msg_id) {
    int index = -1;
    int port = -1;
    int socket = -2;
    pthread_t *thr = NULL;
    t_ft_data *data = NULL;

    while (socket == -2) {
        while (mx_get_free_thread(connect->status, &index) != 0)
            usleep(1);
        port = index + 7200;
        socket = mx_open_listener(port);
    }

    json_object *jobj = json_object_new_object();

    mx_j_o_o_a(jobj, "code", json_object_new_int(200));
    mx_j_o_o_a(jobj, "type", json_object_new_int(FILE_TRANSFER));
    mx_j_o_o_a(jobj, "mid", json_object_new_int(msg_id));
    mx_j_o_o_a(jobj, "port", json_object_new_int(port));
    mx_j_o_o_a(jobj, "path", json_object_new_string(file));

    char *j_str = (char *)json_object_to_json_string(jobj);
    SSL *ssl = mx_find_ssl(connect->ssl_list, connect->fd);
    SSL_write(ssl, j_str, (sizeof(char) * strlen(j_str)));

    if (socket > 0) {
        thr = malloc(sizeof(pthread_t));
        data = malloc(sizeof(t_ft_data));
        data->name = mx_strdup(file);
        data->sock = socket;
        data->status = &((connect->status)[index]);
        if (pthread_create(thr, NULL, mx_receive_file, (void *)data) != 0)
            return "{\"code\": 666}";
        return res;
    }
    else
        return "{\"code\": 666}";
}

