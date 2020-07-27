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
                            mx_json_string_code_type(200, DOWNLOAD), msg->id, 1);
}

// TODO: REFACTOR !!!!!!!!!!!!!!!!!
char *mx_file_transfer(t_comm *connect, char *file, char *res, int msg_id, bool type) {
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
    mx_j_o_o_a(jobj, "mode", json_object_new_boolean(type));

    char *j_str = (char *)json_object_to_json_string(jobj);
    SSL *ssl = mx_find_ssl(connect->ssl_list, connect->fd);
    SSL_write(ssl, j_str, (sizeof(char) * strlen(j_str)));

    char *itoa_str = NULL;
    char *tmp = NULL;
    char *path = NULL;

    itoa_str = mx_itoa(msg_id);
    tmp = mx_strjoin(itoa_str, "_");
    mx_strdel(&itoa_str);
    if (type == 0) {
        itoa_str = mx_memrchr(file, '/', (size_t)mx_strlen(file));
        itoa_str += 1;
        path = mx_strjoin(tmp, itoa_str);
    }
    else
        path = mx_strjoin(tmp, file);

    mx_strdel(&tmp);
    tmp = mx_strjoin("files/", path);
    mx_strdel(&path);

    if (socket > 0) {
        thr = malloc(sizeof(pthread_t));
        data = malloc(sizeof(t_ft_data));
        data->name = tmp;
        data->sock = socket;
        data->status = &((connect->status)[index]);
        data->type = type;

        if (pthread_create(thr, NULL, mx_init_transfer, (void *)data) != 0)
            return "{\"code\": 666}";
        return res;
    }
    else
        return "{\"code\": 666}";
}

