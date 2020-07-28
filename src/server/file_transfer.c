#include "server.h"

static void *mx_init_transfer(void *void_data) {
    t_ft_data *data = (t_ft_data *)void_data;
    struct sockaddr_in cli_addr;
    unsigned int len = sizeof(cli_addr);
    int connect = accept(data->sock, (MX_SA *)&cli_addr, &len);
    int sock = data->sock;

    if (connect < 0)
        pthread_exit(NULL);
    data->sock = connect;
    if (!data->type) {
        mx_mkdir("files");
        mx_recv_file(void_data);
    }
    else
        mx_send_file_serv(void_data);

    *(data->status) = 0;
    close(connect);
    close(sock);
    pthread_exit(NULL);
}

char *mx_download(void *jobj, t_comm *connect) {
    json_object *j_uid = NULL;
    json_object *j_mid = NULL;
    int mid = -1;
    t_msg *msg = NULL;

    json_object_object_get_ex(jobj, "uid", &j_uid);
    json_object_object_get_ex(jobj, "mid", &j_mid);
    if (j_mid && j_uid && json_object_get_type(j_uid) == json_type_int
        && json_object_get_type(j_mid) == json_type_int)
        mid = json_object_get_int(j_mid);
    else
        return mx_bad_request(NULL, NULL);
    if (mx_validate_token(connect->db, json_object_get_int(j_uid), jobj))
        return mx_json_string_code_type(401, DOWNLOAD);

    msg = mx_get_msg_by_id(connect->db, mid);
    if (!msg->file)
        return mx_json_string_code_type(404, DOWNLOAD);
    return mx_file_transfer(connect, msg->file,
                            mx_j_s_c_t(200, DOWNLOAD), msg->id, 1);
}

static void send_data_to_client(t_comm *connect, t_auditor *data) {
    json_object *jobj = json_object_new_object();
    SSL *ssl = NULL;

    mx_j_o_o_a(jobj, "code", json_object_new_int(200));
    mx_j_o_o_a(jobj, "type", json_object_new_int(FILE_TRANSFER));
    mx_j_o_o_a(jobj, "mid", json_object_new_int(data->mid));
    mx_j_o_o_a(jobj, "port", json_object_new_int(data->port));
    mx_j_o_o_a(jobj, "path", json_object_new_string(data->file));
    mx_j_o_o_a(jobj, "mode", json_object_new_boolean(data->mode));

    char *j_str = (char *)json_object_to_json_string(jobj);
    ssl = mx_find_ssl(connect->ssl_list, connect->fd);
    SSL_write(ssl, j_str, (sizeof(char) * strlen(j_str)));

    free(data);
}

char *mx_file_transfer(t_comm *connect, char *file, char *res, int msg_id,
                       bool type) {
    int port = -1;
    int socket = -2;
    t_ft_data *data = NULL;
    pthread_t *thr = malloc(sizeof(pthread_t));;

    while (socket == -2) {
        while (mx_get_free_thread(connect->status, &port) != 0)
            usleep(1);
        port += 7200;
        socket = mx_open_listener(port);
    }
    send_data_to_client(connect, mx_auditor_kostyl(msg_id, port, file, type));
    if (socket > 0) {
        data = mx_init_ft_data(connect, socket,
                               mx_auditor_kostyl(msg_id, port, file, type));
        if (pthread_create(thr, NULL, mx_init_transfer, (void *)data) != 0)
            return "{\"code\": 666}";
        return res;
    }
    return "{\"code\": 666}";
}

