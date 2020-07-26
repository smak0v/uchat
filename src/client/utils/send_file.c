#include "client.h"

void mx_cli_file_transfer(char *response, t_glade *g) {
    json_object *jobj = json_tokener_parse(response);
    json_object *j_value = NULL;
    char *path = NULL;
    int port = -1;
    bool mode = -1;

    json_object_object_get_ex(jobj, "path", &j_value);
    if (j_value)
        path = (char *)json_object_get_string(j_value);
    json_object_object_get_ex(jobj, "port", &j_value);
    if (json_object_get_type(j_value) == json_type_int)
        port = json_object_get_int(j_value);
    json_object_object_get_ex(jobj, "mode", &j_value);
    if (json_object_get_type(j_value) == json_type_boolean)
        mode = json_object_get_boolean(j_value);

    mx_process_send_file(g->ip, path, port, mode);
}

// TODO REFACTOR
void mx_process_send_file(char *ip, char *path, int port, bool mode) {
    int connection_fd = -1;
    pthread_t *thr = NULL;
    t_ft_data *data = NULL;

    connection_fd = mx_open_connection(ip, port);
    if (connection_fd < 0)
        return;

    if (mode)
        path = mx_strjoin("~/Downloads", path);

    thr = malloc(sizeof(pthread_t));
    data = malloc(sizeof(t_ft_data));
    data->name = path;
    data->sock = connection_fd;

    if (!mode) {
        if (pthread_create(thr, NULL, mx_send_file, (void *)data) != 0)
            printf("Thread creation error in process_send_file\n");
    }
    else
        if (pthread_create(thr, NULL, mx_recv_file, (void *)data) != 0)
            printf("Thread creation error in process_send_file\n");
}

void *mx_send_file(void *data) {
    FILE *file;
    char buffer[1024];
    int b = 1;
    char *json_str;
    int pack_num = 1;

    if (!(file = fopen(((t_ft_data *)(data))->name, "r")))
        pthread_exit(NULL);

    while ((b = fread(buffer, sizeof(char), sizeof(buffer), file)) > 0) {
        json_str = mx_json_string_s_file(1, pack_num++, buffer, b);
        write(((t_ft_data *)(data))->sock, json_str, strlen(json_str));
        mx_strdel(&json_str);
        usleep(1000);
        bzero(buffer, sizeof(buffer));
    }

    pthread_exit(NULL);
}
