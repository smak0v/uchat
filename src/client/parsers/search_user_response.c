#include "client.h"

static void parse_users(json_object *jobj, t_glade *g) {
    json_object *j_users = json_object_object_get(jobj, "users");
    GtkWidget *l_not_users_found = NULL;

    if (json_object_get_type(j_users) != json_type_null) {
        (void)g;
    }
    else {
        l_not_users_found = gtk_label_new("No users found");
        gtk_label_set_yalign(GTK_LABEL(l_not_users_found), 0.5);
        gtk_container_add(GTK_CONTAINER(g->box3), l_not_users_found);
        gtk_widget_show(l_not_users_found);
    }
}

static int check_response_code(int code) {
    if (code == 400 || code == 401 || code == 500)
        return MX_FAILURE;
    else
        return MX_SUCCESS;
}

void mx_parse_serach_user_response(char *response, t_glade *g) {
    json_object *jobj = json_tokener_parse(response);
    json_object *j_code = NULL;

    if (json_object_get_type(jobj) == json_type_object) {
        json_object_object_get_ex(jobj, "code", &j_code);
        if (j_code && json_object_get_type(j_code) == json_type_int) {
            if (!check_response_code(json_object_get_int(j_code))) {
                mx_printstr_endl(response);
                parse_users(jobj, g);
                mx_clear_jobj(&jobj, MX_SUCCESS);
            }
            else
                mx_clear_jobj(&jobj, MX_FAILURE);
        }

        mx_clear_jobj(&jobj, MX_FAILURE);
    }

    mx_clear_jobj(&jobj, MX_FAILURE);
}
