#include "client.h"


void mx_pilin_pilin(t_glade *g, json_object *j_msg) {
    json_object *j_uid = json_object_object_get(j_msg, "uid");

    if (g->uid != mx_atoi((char *)json_object_get_string(j_uid)))
        mx_play_audio_thread(g, MX_DILIN_PATH);
}
