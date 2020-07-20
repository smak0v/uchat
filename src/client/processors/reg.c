#include "client.h"

void mx_reg(char *response, t_glade *g) {
    if (!mx_parse_signup_response(response, g))
        mx_login(g);
}
