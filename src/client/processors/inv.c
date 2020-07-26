#include "client.h"

void mx_inv(char *response, t_glade *g) {
    gdk_threads_add_idle(mx_invite_user_to_group_response,
        mx_create_main_thread_struct(response, g));
}
