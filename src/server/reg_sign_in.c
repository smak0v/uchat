#include "server.h"

static int validate_sign_in(sqlite3 *db, const char *name, const char *passw) {
    t_user *user = mx_get_user_by_login(db, (char *)name);

    if (!mx_strcmp((char *)name, user->user_login) 
        && !mx_strcmp((char *)passw, user->user_pass))
        return user->user_id;
    else
        return -1;
}

static char *add_to_db(sqlite3 *db, char *name, char *passw) {
    // int code = mx_add_user(db, (char *)name, (char *)passw);
    mx_add_user(db, (char *)name, (char *)passw);

    // if (code > 0)
       return "{\"code\": 200}";
    // else if (code == -1)
    //    return "{\"code\": 500}";
    // else //  code == -2, username already exists
    //    return "{\"code\": 409}";
}

static int extract_name_passw(json_object *json, const char **name,
                              const char **passw) {
    json_object_object_foreach(json, key, val) {
        if (json_object_get_type(val) != json_type_string)
            return 1;
        if (!mx_strcmp(key, "name"))
            *name = json_object_get_string(val);
        else if (!mx_strcmp(key, "passw"))
            *passw = json_object_get_string(val);
    }

    if (*name == NULL || *passw == NULL)
        return 1;
    else
        return 0;
}

char *mx_register_user(void *jobj, t_list **clients, sqlite3 *db, int fd) {
    const char *name = NULL;
    const char *pass = NULL;
    int code = 0;

    clients = NULL;
    fd = 0;

    if ((code = extract_name_passw((json_object *)jobj, &name, &pass)) != 0)
        return mx_bad_request(NULL, NULL, NULL, 0);

    return add_to_db(db, (char *)name, (char *)pass);
}

char *mx_sign_in(void *jobj, t_list **clients, sqlite3 *db, int fd) {
    const char *name = NULL;
    const char *pass = NULL;
    int code = 0;
    int uid = -1;

    if ((code = extract_name_passw((json_object *)jobj, &name, &pass)) != 0)
        return mx_bad_request(NULL, NULL, NULL, 0);

    if ((uid = validate_sign_in(db, name, pass)) != -1)
        return "{\"code\": 401}";

    mx_add_client(clients, fd, (char *)name, uid);
    return "{\"code\": 200}";
}
