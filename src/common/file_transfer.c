#include "uchat.h"

static char *new_package(int message_id, int package_number, char *buffer) {
    char *id = mx_itoa(message_id);
    char *pack_num = mx_itoa(package_number);
    char *json_string = mx_json_builder(6, "\"id\":", id, "\"num\":", pack_num,
                                        "\"data:\":", buffer);

    mx_strdel(&id);
    mx_strdel(&pack_num);
    return json_string;
}

// from cient
void mx_send_file(int socket_fd, char *path) {
    FILE *file;
    char buffer[1024];
    int b = 1;
    char *json_str;
    int pack_num = 1;

    if (!(file = fopen(path, "r")))
        mx_terminate("open");

    while ((b = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        json_str = new_package(1, pack_num++, buffer);
        send(socket_fd, json_str, b, 0);
        mx_strdel(&json_str);
        bzero(buffer, sizeof(buffer));
    }

    if (fclose(file) < 0)
        mx_terminate("close");
}

// from server
void mx_recv_file(int connection_fd, char *filename) {
    FILE *file;
    char buffer[1024];
    int b = 1;
    json_object *jobj;
    json_object *json_data;
    const char *data;

    if (!(file = fopen(filename, "w")))
        mx_terminate("open");

    while ((b = recv(connection_fd, buffer, sizeof(buffer), 0)) > 0) {
        jobj = json_tokener_parse(buffer);
        json_object_object_get_ex(jobj, "data", &json_data);
        data = json_object_get_string(json_data);
        fwrite(data, 1, b, file);
    }

    if (fclose(file) < 0)
        mx_terminate("close");
}
// recv принимает json, парсит его и записывает в файл данные из массива файл
// в jsone айди месседжа, номер пакета и буффер с куском файла
