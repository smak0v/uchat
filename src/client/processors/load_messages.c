#include "client.h"

void mx_load_messages(char *response, t_glade *g) {
    mx_parse_load_messages_response(response, g);
}
