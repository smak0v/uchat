#include "server.h"

static char *json_string_s_file(int id, int num, char *buff, int buf_size) {
    json_object *jobj = json_object_new_object();

    json_object_object_add(jobj, "id", json_object_new_int(id));
    json_object_object_add(jobj, "num", json_object_new_int(num));
    json_object_object_add(jobj, "data",
        json_object_new_string_len(buff, buf_size));
    json_object_object_add(jobj, "is_last",
        json_object_new_boolean(buf_size < 1024 ? true : false));

    return (char *)json_object_to_json_string(jobj);
}

void *mx_send_file_serv(void *data) {
    FILE *file;
    char buffer[1024];
    int b = 1;
    char *json_str;
    int pack_num = 1;

    if (!(file = fopen(((t_ft_data *)(data))->name, "r")))
        pthread_exit(NULL);

    while ((b = fread(buffer, sizeof(char), sizeof(buffer), file)) > 0) {
        json_str = json_string_s_file(1, pack_num++, buffer, b);
        write(((t_ft_data *)(data))->sock, json_str, strlen(json_str));
        mx_strdel(&json_str);
        usleep(1000);
        bzero(buffer, sizeof(buffer));
    }

    pthread_exit(NULL);
}
