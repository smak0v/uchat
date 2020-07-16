#include "client.h"

void mx_s_in(char *response, t_glade *g) {
    if (!mx_parse_login_response(response, g))
        mx_show_win_chat(g->w_log, g);

    mx_clear_login_inputs(g);
}
