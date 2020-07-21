#include "client.h"

void mx_s_msg(char *response, t_glade *g) {
    if (!mx_parse_send_message_response(response))
        mx_add_message_to_gui(g, response);
}
