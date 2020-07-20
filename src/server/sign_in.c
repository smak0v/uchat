#include "server.h"

static int validate_sign_in(sqlite3 *db, const char *name, const char *passw) {
    t_user *user = mx_get_user_by_login(db, (char *)name);
    char *encrypted_passw = mx_hmac_sha_256((char *)name, (char *)passw);

    if (!user || mx_strcmp(encrypted_passw, user->user_pass))
        return -1;
    else
        return user->user_id;
}

int mx_extract_name_passw(json_object *json, const char **name,
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

static char *process_sockets(sqlite3 *db, int fd, int uid, char *tk) {
    char *sock = mx_get_sock_by_user_id(db, uid);
    char *tmp = NULL;

    if (!sock) {
        if (mx_add_sock_user(db, uid, mx_itoa(fd), tk) == -1)
            return NULL;
    }
    else if (sock[0] == '\0') {
        mx_strdel(&sock);
        return NULL;
    }
    else {
        tmp = sock;
        sock = mx_add_socket(sock, fd);
    }

    if (mx_add_sock_user(db, uid, sock, tk) == -1)
        return NULL;

    return "ok";
}

char *mx_post_reg_sign_in(t_comm *connect, char *name, char *passw) {
    json_object *jobj = json_object_new_object();

    mx_j_o_o_a(jobj, "name", json_object_new_string(name));
    mx_j_o_o_a(jobj, "passw", json_object_new_string(passw));

    return mx_sign_in((void *)jobj, connect);
}

char *mx_sign_in(void *jobj, t_comm *connect) {
    const char *name = NULL;
    const char *pass = NULL;
    int uid = -1;
    char *tk = NULL;
    char *res = NULL;

    if (mx_extract_name_passw((json_object *)jobj, &name, &pass) != 0)
        return mx_bad_request(NULL, NULL);
    if ((uid = validate_sign_in(connect->db, name, pass)) == -1)
        return mx_json_string_code_type(404, S_IN);
    if ((tk = mx_get_token_by_user_id(connect->db, uid)) == NULL)
        if ((tk = (char *)mx_generate_token()) == NULL)
            return mx_json_string_code_type(500, S_IN);
    if ((res = process_sockets(connect->db, connect->fd, uid, tk)) == NULL)
        return mx_json_string_code_type(500, S_IN);
    else
        res = mx_json_string_s_in(uid, tk);
    mx_strdel(&tk);

    return res;
}
