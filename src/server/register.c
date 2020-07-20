#include "server.h"

static int add_to_db(sqlite3 *db, char *name, char *passw, int *uid) {
    *uid = mx_add_user(db, (char *)name, (char *)passw);

    if (*uid > 0)
        return 0;
    else if (*uid == -1)
        return -1;
    else //  code == -2, username already exists
        return -2;
}

static int create_profile(t_comm *connect, char *name, int uid) {
    t_profile *prof = mx_memalloc(sizeof(t_profile));
    int code = 0;

    prof->name = name;
    prof->user_id = uid;
    if (mx_add_profile(connect->db, prof) == -1)
        code = -1;
    free(prof);

    return code;
}

char *mx_register_user(void *jobj, t_comm *connect) {
    const char *name = NULL;
    const char *pass = NULL;
    char *encrypted_pass = NULL;
    int code = 0;
    int uid = -1;

    if (mx_extract_name_passw((json_object *)jobj, &name, &pass) != 0)
        return mx_bad_request(NULL, NULL);
    if ((encrypted_pass = mx_hmac_sha_256((char *)name, (char *)pass)) == NULL)
        return mx_json_string_code_type(500, REG);
    code = add_to_db(connect->db, (char *)name, encrypted_pass, &uid);
    if (code == -1)
        return mx_json_string_code_type(500, REG);
    else if (code == -2)
        return mx_json_string_code_type(409, REG);

    if (create_profile(connect, (char *)name, uid) == -1)
        return mx_json_string_code_type(500, REG);

    return mx_post_reg_sign_in(connect, (char *)name, (char *)pass);
}
