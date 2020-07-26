#include "client.h"

void mx_load_groups(char *response, t_glade *g) {
    gdk_threads_add_idle(mx_parse_load_groups_response,
        mx_create_main_thread_struct(response, g));
}
