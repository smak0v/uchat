#include "server.h"

static const char *add_to_db(sqlite3 *db, char *name, char *passw, int *uid) {
    *uid = mx_add_user(db, (char *)name, (char *)passw);

    if (*uid > 0)
       return "{\"code\": 200}";
    else if (*uid == -1)
       return "{\"code\": 500}";
    else //  code == -2, username already exists
       return "{\"code\": 409}";
}

char *mx_register_user(void *jobj, t_comm *connect) {
    const char *name = NULL;
    const char *pass = NULL;
    char *encrypted_pass = NULL;
    t_profile *prof = NULL;
    int code = 0;

    if ((code = mx_extract_name_passw((json_object *)jobj, &name, &pass)) != 0)
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
