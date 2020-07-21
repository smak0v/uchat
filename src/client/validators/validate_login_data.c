#include "client.h"

int mx_validate_login_data(t_glade *g) {
    char *username = g->log;
    char *password = g->pass;

    if ((!username || !password) || (!strlen(username) || !strlen(password))) {
        gtk_label_set_text(GTK_LABEL(g->l_login_error),
             "Fields can`t be empty!\n" \
             "One of the fields or both fields is empty. Try again!");
        gdk_threads_add_idle(mx_show_widget, g->l_login_error);
        return -1;
    }

    return MX_SUCCESS;
}
