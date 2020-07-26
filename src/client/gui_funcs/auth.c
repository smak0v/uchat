#include "client.h"

void mx_login(t_glade *g) {
    char *request = mx_json_string_login_signup(S_IN, g->log, g->pass);

    SSL_write(g->ssl, request, strlen(request));

    mx_strdel(&request);
}

void mx_b_reg_log(GtkButton *b, t_glade *g) {
    char *repeat = (char *)gtk_entry_get_text(GTK_ENTRY(g->r_repass));
    char *request = NULL;

    (void)b;

    g->log = mx_strdup((char *)gtk_entry_get_text(GTK_ENTRY(g->r_ename)));
    g->pass = mx_strdup((char *)gtk_entry_get_text(GTK_ENTRY(g->r_epass)));

    if (!mx_validate_signup_data(g, repeat)) {
        request = mx_json_string_login_signup(REG, g->log, g->pass);

        SSL_write(g->ssl, request, strlen(request));

        mx_strdel(&request);
    }

    mx_clear_signup_inputs(g);
}

void mx_b_log(GtkButton *b, t_glade *g) {
    g->log = mx_strdup((char *)gtk_entry_get_text(GTK_ENTRY(g->e_name)));
    g->pass = mx_strdup((char *)gtk_entry_get_text(GTK_ENTRY(g->e_pass)));

    (void)b;

    if (!mx_validate_login_data(g))
       mx_login(g);
}

void mx_logout(t_glade *g) {
    char *request = mx_json_string_logout(g->token, g->uid);

    SSL_write(g->ssl, request, strlen(request));

    mx_strdel(&request);
}

void mx_b_logout(GtkButton *b, t_glade *g) {
    (void)b;

    mx_logout(g);
}
