#include "server.h"

static void check_null(char *name, char *var, json_object *jobj) {
    if (var)
        mx_j_o_o_a(jobj, name, json_object_new_string(var));
    else
        mx_j_o_o_a(jobj, name, json_object_new_null());
}


json_object *mx_create_profile_object(t_profile *prof) {
    json_object *j_prof = json_object_new_object();

    mx_j_o_o_a(j_prof, "uid", json_object_new_int(prof->user_id));
    mx_j_o_o_a(j_prof, "name", json_object_new_string(prof->name));

    check_null("dob", prof->birth, j_prof);
    check_null("mail", prof->email, j_prof);
    check_null("status", prof->status, j_prof);
    check_null("country", prof->country, j_prof);

    return j_prof;
}
