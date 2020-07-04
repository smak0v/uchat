#include "client.h"

static void login(t_glade *g, GtkWidget *w) {
    char *request = mx_json_string_login_signup(S_IN, g->log, g->pass);
    char *response = NULL;

    SSL_write(g->ssl, request, strlen(request));
    response = mx_read_server_response(g);

    if (!mx_parse_login_response(response, g))
        mx_open_win_chat(w, g);

    mx_clear_login_inputs(g);

    mx_strdel(&request);
    mx_strdel(&response);
}

void mx_b_reg_log(GtkButton *b, t_glade *g) {
    char *repeat = (char *)gtk_entry_get_text(GTK_ENTRY(g->r_repass));
    char *request = NULL;
    char *response = NULL;

    (void)b;

    g->log = (char *)gtk_entry_get_text(GTK_ENTRY(g->r_ename));
    g->pass = (char *)gtk_entry_get_text(GTK_ENTRY(g->r_epass));

    if (!mx_validate_signup_data(g, repeat)) {
        request = mx_json_string_login_signup(REG, g->log, g->pass);
        SSL_write(g->ssl, request, strlen(request));
        response = mx_read_server_response(g);
        if (!mx_parse_signup_response(response, g))
            login(g, g->w_reg);

        mx_strdel(&request);
        mx_strdel(&response);
    }
    mx_clear_signup_inputs(g);
}

void mx_b_log(GtkButton *b, t_glade *g) {
    g->log = (char *)gtk_entry_get_text(GTK_ENTRY(g->e_name));
    g->pass = (char *)gtk_entry_get_text(GTK_ENTRY(g->e_pass));

    (void)b;

    if (!mx_validate_login_data(g))
       login(g, g->w_log);
}

void mx_b_logout(GtkButton *b, t_glade *g) {
    char *request = mx_json_string_logout(g->token, g->uid);
    char *response = NULL;

    (void)b;

    SSL_write(g->ssl, request, strlen(request));
    response = mx_read_server_response(g);

    mx_parse_logout_response(response, g);

    mx_strdel(&request);
    mx_strdel(&response);
}
