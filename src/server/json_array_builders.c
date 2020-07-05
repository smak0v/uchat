#include "server.h"

void mx_fill_array_int(json_object *jobj, int *arr, int len) {
    json_object *j_int = NULL;

    for (int i = 0; i < len; i++) {
        j_int = json_object_new_int(arr[i]);
        json_object_array_add(jobj, j_int);
    }
}

void mx_fill_array_str(json_object *jobj, char **arr, int len) {
    json_object *j_int = NULL;

    for (int i = 0; i < len; i++) {
        j_int = json_object_new_string(arr[i]);
        json_object_array_add(jobj, j_int);
    }
}
