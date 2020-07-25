#include "client.h"

static bool check_did_in_dialogues(t_list *dialogues, int did) {
    t_dialogue *dialogue = NULL;

    while (dialogues) {
        dialogue = (t_dialogue *)dialogues->data;

        if (dialogue->did == did)
            return true;

        dialogues = dialogues->next;
    }

    return false;
}

static void add_dialog_to_gui(t_glade *g, json_object *j_uid2,
    json_object *j_did, json_object *j_name) {
    g->group = false;
    g->uid2 = json_object_get_int(j_uid2);
    g->dgid = json_object_get_int(j_did);

    mx_add_dialogue_to_gui(g, g->dgid, g->uid2,
        (char *)json_object_get_string(j_name));
}

static int check_response_code(int code, json_object *jobj, t_glade *g) {
    json_object *j_msg = NULL;
    json_object *j_did = NULL;
    json_object *j_uid2 = NULL;
    json_object *j_name = NULL;

    if (code == 400 || code == 401 || code == 500)
        return MX_FAILURE;
    else {
        json_object_object_get_ex(jobj, "msg", &j_msg);
        json_object_object_get_ex(j_msg, "did", &j_did);
        json_object_object_get_ex(j_msg, "uid2", &j_uid2);
        json_object_object_get_ex(j_msg, "nme", &j_name);

        if (check_did_in_dialogues(g->dialogues, json_object_get_int(j_did)))
            return MX_SUCCESS;
        else
            add_dialog_to_gui(g, j_uid2, j_did, j_name);

        return MX_SUCCESS;
    }
}

int mx_parse_send_message_response(char *response, t_glade *g) {
    json_object *jobj = json_tokener_parse(response);
    json_object *j_code = NULL;

    if (json_object_get_type(jobj) == json_type_object) {
        json_object_object_get_ex(jobj, "code", &j_code);
        if (j_code && json_object_get_type(j_code) == json_type_int) {
            if (!check_response_code(json_object_get_int(j_code), jobj, g)) {
                return mx_clear_jobj(&jobj, MX_SUCCESS);
            }
            else
                return mx_clear_jobj(&jobj, MX_FAILURE);
        }

        return mx_clear_jobj(&jobj, MX_FAILURE);
    }

    return mx_clear_jobj(&jobj, MX_FAILURE);
}
