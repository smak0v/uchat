#include "client.h"

static char *get_string(json_object *jobj) {
    if (jobj)
        return (char *)json_object_get_string(jobj);
    else
        return "";
}

static void save_response_data(json_object *j_prof, t_glade *g) {
    json_object *j_name = NULL;
    json_object *j_dob = NULL;
    json_object *j_mail = NULL;
    json_object *j_status = NULL;
    json_object *j_country = NULL;

    json_object_object_get_ex(j_prof, "name", &j_name);
    json_object_object_get_ex(j_prof, "dob", &j_dob);
    json_object_object_get_ex(j_prof, "mail", &j_mail);
    json_object_object_get_ex(j_prof, "status", &j_status);
    json_object_object_get_ex(j_prof, "country", &j_country);

    gtk_entry_set_text(GTK_ENTRY(g->e_name), get_string(j_name));
    gtk_entry_set_text(GTK_ENTRY(g->e_birthday), get_string(j_dob));
    gtk_entry_set_text(GTK_ENTRY(g->e_email), get_string(j_mail));
    gtk_entry_set_text(GTK_ENTRY(g->e_status), get_string(j_status));
    gtk_entry_set_text(GTK_ENTRY(g->e_country), get_string(j_country));
}

static int check_response_code(int code, json_object *jobj, t_glade *g,
    bool current) {
    json_object *j_prof = NULL;

    if (code == 400 || code == 401 || code == 404 || code == 500)
        return MX_FAILURE;
    else {
        if (current) {
            json_object_object_get_ex(jobj, "prof", &j_prof);
            save_response_data(j_prof, g);
        }
        return MX_SUCCESS;
    }
}

void mx_parse_get_profile_response(char *response, t_glade *g, bool current) {
    json_object *jobj = json_tokener_parse(response);
    json_object *j_code = NULL;

    if (json_object_get_type(jobj) == json_type_object) {
        json_object_object_get_ex(jobj, "code", &j_code);
        if (j_code && json_object_get_type(j_code) == json_type_int) {
            if (!check_response_code(json_object_get_int(j_code), jobj, g,
                current))
                mx_clear_jobj(&jobj, MX_SUCCESS);
            else
                mx_clear_jobj(&jobj, MX_FAILURE);
        }

        mx_clear_jobj(&jobj, MX_FAILURE);
    }

    mx_clear_jobj(&jobj, MX_FAILURE);
}
