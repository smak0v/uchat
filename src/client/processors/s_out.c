#include "client.h"

void mx_s_out(char *response, t_glade *g) {
    gdk_threads_add_idle(mx_parse_logout_response,
        mx_create_main_thread_struct(response, g));
}
