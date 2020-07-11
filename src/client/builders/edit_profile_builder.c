#include "client.h"

char *mx_json_string_edit_profile(t_glade *g, t_profile *profile) {
    json_object *jobj = json_object_new_object();

    json_object_object_add(jobj, "type",
        json_object_new_int((int)EDIT_PROFILE));
    json_object_object_add(jobj, "tok", json_object_new_string(g->token));
    json_object_object_add(jobj, "id", json_object_new_int(g->uid));
    json_object_object_add(jobj, "name",
        json_object_new_string(profile->name));
    json_object_object_add(jobj, "dob", json_object_new_string(profile->dob));
    json_object_object_add(jobj, "mail",
        json_object_new_string(profile->email));
    json_object_object_add(jobj, "status",
        json_object_new_string(profile->status));
    json_object_object_add(jobj, "country",
        json_object_new_string(profile->country));

    return (char *)json_object_to_json_string(jobj);
}
