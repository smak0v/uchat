#include "client.h"

char *mx_json_string_download_file(char *token, int uid, int mid) {
    json_object *jobj = json_object_new_object();

    json_object_object_add(jobj, "type", json_object_new_int((int)DOWNLOAD));
    json_object_object_add(jobj, "tok", json_object_new_string(token));
    json_object_object_add(jobj, "uid", json_object_new_int(uid));
    json_object_object_add(jobj, "mid", json_object_new_int(mid));

    return (char *)json_object_to_json_string(jobj);
}
