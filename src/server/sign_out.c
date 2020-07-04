#include "server.h"

char *mx_sign_out(void *jobj, t_comm *connect) {
    json_object *j_id = NULL;
    int id = -1;

    json_object_object_get_ex(jobj, "id", &j_id);

    if (j_id && json_object_get_type(j_id) == json_type_int)
        id = json_object_get_int(j_id);
    else
        return mx_bad_request(NULL, NULL);

    if (mx_validate_token(connect->db, id, (json_object *)jobj))
        return "{\"code\": 401}";

    if (mx_delete_sock_by_user_id(connect->db, id) == -1)
        printf("I'm not sure how to handle this mistake in mx_sign_out\n");

    return "{\"code\": 200}";
}
