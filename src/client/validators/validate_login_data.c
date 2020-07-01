#include "uchat.h"

int mx_validate_login_data(t_glade *g) {
    char *username = g->log;
    char *password = g->pass;

    if ((!username || !password) || (!strlen(username) || !strlen(password))) {
        mx_create_error_modal_window("Fields can not be empty!",
            "One of the fields or both fields is empty. Try again!", g->w_log);
        return -1;
    }

    return MX_SUCCESS;
}
