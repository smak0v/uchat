#include "server.h"

char *mx_register_user(void *jobj, t_list **clients, sqlite3 *db) {
    json_object *json = (json_object *)jobj;
    json = NULL;
    clients = NULL;
    db = NULL;
    return "working";

}

