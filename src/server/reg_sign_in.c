#include "server.h"

static int validate_sign_in(sqlite3 *db, const char *name, const char *passw) {
    t_user *user = mx_get_user_by_login(db, (char *)name);
    char *encrypted_passw = mx_hmac_sha_256((char *)name, (char *)passw);

    if (!user || mx_strcmp(encrypted_passw, user->user_pass))
        return -1;
    else
        return user->user_id;
}

static const char *add_to_db(sqlite3 *db, char *name, char *passw, int *uid) {
    *uid = mx_add_user(db, (char *)name, (char *)passw);

    if (*uid > 0)
       return "{\"code\": 200}";
    else if (*uid == -1)
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
    char *encrypted_pass = NULL;
    t_profile *prof = NULL;
    int code = 0;

    if ((code = extract_name_passw((json_object *)jobj, &name, &pass)) != 0)
        return mx_bad_request(NULL, NULL);
    if ((encrypted_pass = mx_hmac_sha_256((char *)name, (char *)pass)) == NULL)
        return "{\"code\": 500}";
    pass = add_to_db(connect->db, (char *)name, encrypted_pass, &code);
    if (code > 0) {
        prof = mx_memalloc(sizeof(t_profile));
        prof->name = (char *)name;
        prof->user_id = code;
        if (mx_add_profile(connect->db, prof) == -1)
            return "{\"code\": 500}";
    }
    free(prof);
    return (char *)pass;
}

// TODO: REFACTOR
char *mx_sign_in(void *jobj, t_comm *connect) {
    const char *name = NULL;
    const char *pass = NULL;
    int uid = -1;
    char *tk = NULL;
    char *res = NULL;

    char *sock = NULL;
    char *tmp = NULL;

    if (extract_name_passw((json_object *)jobj, &name, &pass) != 0)
        return mx_bad_request(NULL, NULL);
    if ((uid = validate_sign_in(connect->db, name, pass)) == -1)
        return "{\"code\": 404}";
    if ((token = mx_get_token_by_user_id(connect->db, uid)) == NULL)
        if ((token = (char *)mx_generate_token()) == NULL)
            return "{\"code\": 500}";

    sock = mx_get_sock_by_user_id(connect->db, uid);
    if (!sock)
        if (mx_add_sock_user(connect->db, uid, mx_itoa(connect->fd), tk) == -1)
            res = "{\"code\": 500}";
    else if (sock[0] == '\0') {
        res = "{\"code\": 500}";
        mx_strdel(&sock);
    }
    else {
        tmp = sock;
        sock = mx_add_socket(sock, connect->fd);
        mx_strdel(&tmp);
    }

    if (mx_add_sock_user(connect->db, uid, sock, tk) == -1)
        res = "{\"code\": 500}";
    else
        res = mx_json_string_s_in(uid, tk);
    mx_strdel(&tk);
    return res;
}
