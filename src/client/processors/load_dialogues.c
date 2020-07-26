#include "client.h"

void mx_load_dialogues(char *response, t_glade *g) {
    gdk_threads_add_idle(mx_parse_load_dialogs_response,
        mx_create_main_thread_struct(response, g));
}
