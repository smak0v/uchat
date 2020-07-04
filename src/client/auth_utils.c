#include "client.h"

static void login(t_glade *g, GtkWidget *w) {
    char *request = mx_json_string_login_signup(S_IN, g->log, g->pass);
    char *response = NULL;

    SSL_write(g->ssl, request, strlen(request));
    response = mx_read_server_response(g);

    if (!mx_parse_login_response(response, g))
        mx_open_win_chat(w, g);

    mx_strdel(&request);
    mx_strdel(&response);
}

void mx_open_regwin(GtkWidget *sender, t_glade *g) {
    int w = 0;
    int h = 0;
    int x = 0;
    int y = 0;

    (void)sender;
    gtk_window_get_position(GTK_WINDOW(g->w_log), &x, &y);
    gtk_window_get_size(GTK_WINDOW(g->w_log), &w, &h);
    gtk_widget_hide(g->w_log);

    gtk_window_resize(GTK_WINDOW(g->w_reg), w, h);
    gtk_window_move(GTK_WINDOW(g->w_reg), x, y + 22.35);
    gtk_widget_show_all(GTK_WIDGET(g->w_reg));

    mx_widget_set_visibility(g->bd, "r_pass_err", FALSE);
    mx_widget_set_visibility(g->bd, "r_pass_err1", FALSE);
    mx_widget_set_visibility(g->bd, "r_name_err", FALSE);
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
}

 void mx_b_log(GtkButton *b, t_glade *g) {
    g->log = (char *)gtk_entry_get_text(GTK_ENTRY(g->e_name));
    g->pass = (char *)gtk_entry_get_text(GTK_ENTRY(g->e_pass));

    (void)b;

    if (!mx_validate_login_data(g))
       login(g, g->w_log);
}
