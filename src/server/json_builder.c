#include "server.h"

char *mx_json_string_s_file(int id, int num, char *buff, int buf_size) {
    json_object *jobj = json_object_new_object();

    json_object_object_add(jobj, "id", json_object_new_int(id));
    json_object_object_add(jobj, "num", json_object_new_int(num));
    json_object_object_add(jobj, "data", json_object_new_string(buff));
    json_object_object_add(jobj, "is_last", json_object_new_boolean(buf_size < 1024 ? true : false));


    return (char *)json_object_to_json_string(jobj);
}


