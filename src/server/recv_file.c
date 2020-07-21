#include "server.h"

static const char *get_pack_data(char *buffer) {
    json_object *jobj = json_tokener_parse(buffer);
    json_object *json_data;
    const char *data = NULL;

    if (json_object_get_type(jobj) == json_type_object) {
        json_object_object_get_ex(jobj, "data", &json_data);
        if (json_data && json_object_get_type(json_data) == json_type_string)
            data = json_object_get_string(json_data);
    }

    return data;
}

static int get_pack_num(char *buffer) {
    json_object *jobj = json_tokener_parse(buffer);;
    json_object *json_data;
    int package_num = 0;

    if (json_object_get_type(jobj) == json_type_object) {
        json_object_object_get_ex(jobj, "num", &json_data);
        if (json_data && json_object_get_type(json_data) == json_type_int)
            package_num = json_object_get_int(json_data);
    }

    return package_num;
}

static bool is_package_last(char *buffer) {
    json_object *jobj = json_tokener_parse(buffer);
    json_object *json_data;
    bool is_last = false;

    if (json_object_get_type(jobj) == json_type_object) {
        json_object_object_get_ex(jobj, "is_last", &json_data);
        if (json_data && json_object_get_type(json_data) == json_type_boolean)
            is_last = json_object_get_boolean(json_data);
    }

    return is_last;
}

static int get_data_len(char *buffer) {
    json_object *jobj = json_tokener_parse(buffer);
    json_object *json_data;
    int len = 0;

    if (json_object_get_type(jobj) == json_type_object) {
        json_object_object_get_ex(jobj, "data", &json_data);
        if (json_data && json_object_get_type(json_data) == json_type_string)
            len = json_object_get_string_len(json_data);
    }

    return len;
}

void mx_recv_file(char *filename, int fd) {
    FILE *file;
    char buffer[10000];
    int b = 1;
    const char *data;
    int pack_num = 0;

    char *path = mx_memrchr(filename, '/', strlen(filename));
    if (!(file = fopen(mx_strjoin("files/", path + 1), "w+")))
        mx_terminate("open");

    while ((b = read(fd, buffer, sizeof(buffer) - 1)) >= 0) {
        data = get_pack_data(buffer);
        if (!data)
            mx_terminate("broken json");
        fwrite(data, 1, get_data_len(buffer), file);
        if (is_package_last(buffer))
            break;
        if (++pack_num != get_pack_num(buffer))
            mx_terminate("broken file");
        bzero(buffer, sizeof(buffer));
    }

    if (fclose(file) < 0)
        mx_terminate("close");
}
