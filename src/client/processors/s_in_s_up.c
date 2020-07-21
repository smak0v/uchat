#include "client.h"

void mx_s_in_s_up(char *response, t_glade *g, int type) {
    if (type == S_IN) {
        if (!mx_parse_login_response(response, g))
            mx_show_win_chat(g);

        mx_clear_login_inputs(g);
    }

    else if (type == REG) {
        if (!mx_parse_signup_response(response, g))
            mx_show_win_chat(g);

        mx_clear_signup_inputs(g);
    }
}
