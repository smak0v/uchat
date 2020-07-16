#include "client.h"

void mx_load_groups(char *response, t_glade *g) {
    mx_parse_load_groups_response(response, g);
}
