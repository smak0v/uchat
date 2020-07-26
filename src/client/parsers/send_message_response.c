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

static void add_dialog_to_dialogues(t_glade *g, json_object *j_id,
    json_object *j_uid2) {
    t_dialogue *dialogue = mx_memalloc(sizeof(t_dialogue));

    dialogue->did = json_object_get_int(j_id);
    dialogue->uid2 = json_object_get_int(j_uid2);

    mx_push_back(&g->dialogues, dialogue);
}

static void parse_message_response(json_object *jobj, t_glade *g) {
    json_object *j_msg = NULL;
    json_object *j_id = NULL;
    json_object *j_uid2 = NULL;
    json_object *j_name = NULL;

    json_object_object_get_ex(jobj, "msg", &j_msg);
    json_object_object_get_ex(j_msg, "did", &j_id);
    json_object_object_get_ex(j_msg, "uid2", &j_uid2);
    json_object_object_get_ex(j_msg, "nme", &j_name);

    if (!check_did_in_dialogues(g->dialogues, json_object_get_int(j_id))) {
        add_dialog_to_dialogues(g, j_id, j_uid2);
        add_dialog_to_gui(g, j_uid2, j_id, j_name);
    }
}

int mx_parse_send_message_response(char *response, t_glade *g) {
    json_object *jobj = json_tokener_parse(response);
    json_object *j_code = NULL;

    json_object_object_get_ex(jobj, "code", &j_code);

    if (j_code && json_object_get_type(j_code) == json_type_int) {
        if (json_object_get_int(j_code) == 200) {
            parse_message_response(jobj, g);
            return mx_clear_jobj(&jobj, MX_SUCCESS);
        }
        else
            return mx_clear_jobj(&jobj, MX_FAILURE);
    }

    return mx_clear_jobj(&jobj, MX_FAILURE);
}
