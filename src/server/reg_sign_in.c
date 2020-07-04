#include "server.h"

static int validate_sign_in(sqlite3 *db, const char *name, const char *passw) {
    t_user *user = mx_get_user_by_login(db, (char *)name);

    if (!user || mx_strcmp((char *)passw, user->user_pass))
        return -1;
    else
        return user->user_id;
}

static char *add_to_db(sqlite3 *db, char *name, char *passw) {
    int code = mx_add_user(db, (char *)name, (char *)passw);

    if (code > 0)
       return "{\"code\": 200}";
    else if (code == -1)
       return "{\"code\": 500}";
    else //  code == -2, username already exists
       return "{\"code\": 409}";
}

static int extract_name_passw(json_object *json, const char **name,
                              const char **passw) {
    json_object *j_name = NULL;
    json_object *j_passw = NULL;

    json_object_object_get_ex(json, "name", &j_name);
    json_object_object_get_ex(json, "passw", &j_passw);

    if (j_name && j_passw && json_object_get_type(j_name) == json_type_string
        && json_object_get_type(j_passw) == json_type_string) {
        *name = json_object_get_string(j_name);
        *passw = json_object_get_string(j_passw);
    }

    if (*name == NULL || *passw == NULL)
        return 1;
    else
        return 0;
}

char *mx_register_user(void *jobj, t_comm *connect) {
    const char *name = NULL;
    const char *pass = NULL;
    int code = 0;

    if ((code = extract_name_passw((json_object *)jobj, &name, &pass)) != 0)
        return mx_bad_request(NULL, NULL);

    return add_to_db(connect->db, (char *)name, (char *)pass);
}

// TODO: AUDITOR && REFACGTOR && LEAKS
char *mx_sign_in(void *jobj, t_comm *connect) {
    const char *name = NULL;
    const char *pass = NULL;
    int uid = -1;
    unsigned char *token = malloc(sizeof(unsigned char *) * 257);

    if (extract_name_passw((json_object *)jobj, &name, &pass) != 0)
        return mx_bad_request(NULL, NULL);

    if ((uid = validate_sign_in(connect->db, name, pass)) == -1)
        return "{\"code\": 401}";

    if (RAND_bytes(token, 256) != 1)
        return "{\"code\": 500}";
    token[256] = '\0';

    // TEMPORARY KOSTYL
    for (int i = 0; i < 256; i++) {
        if (token[i] < 33 || token[i] == 34 || token[i] > 126 || token[i] == '{' || token[i] == '}')
            token[i] = 75;
    }

    if (mx_add_sock_user(connect->db, uid, connect->fd, (char *)token) == -1)
        return "{\"code\": 500}";

    return mx_json_string_s_in(uid, (char *)token);
}
