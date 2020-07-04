#include "server.h"

static char *get_token(json_object *jobj) {
    json_object *j_tok = NULL;

    json_object_object_get_ex(jobj, "tok", &j_tok);

    if (!j_tok || json_object_get_type(j_tok) != json_type_string)
        return NULL;

    return (char *)json_object_get_string(j_tok);
}

int mx_validate_token(sqlite3 *db, int id, void *v_jobj) {
    json_object *jobj = (json_object *)v_jobj;
    int code = 1;
    char *true_tok = NULL;
    char *incoming_tok = get_token(jobj);

    if (!incoming_tok)
        return code;

    true_tok = mx_get_token_by_user_id(db, id);

    if (!mx_strcmp(incoming_tok, true_tok))
        code = 0;
    else {
        printf("%d\n%s\n%s\n", id, true_tok, incoming_tok);
        mx_print_db(db, "SOCKETS");
    }
    mx_strdel(&true_tok);
    return code;
}
