#include "client.h"

void mx_find_user(char *response, t_glade *g) {
    gdk_threads_add_idle(mx_parse_get_profile_response,
        mx_create_main_thread_struct(response, g));
}
