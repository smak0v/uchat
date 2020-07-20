#include "client.h"

void mx_s_out(char *response, t_glade *g) {
    mx_parse_logout_response(response, g);
}
