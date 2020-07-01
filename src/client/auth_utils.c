#include "uchat.h"

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

    (void)b;
    g->log = (char *)gtk_entry_get_text(GTK_ENTRY(g->r_ename));
    g->pass = (char *)gtk_entry_get_text(GTK_ENTRY(g->r_epass));

    if (!mx_validate_signup_data(g, repeat)) {
        mx_open_win_chat(g->w_reg, g);
    }
}

 void mx_b_log(GtkButton *b, t_glade *g) {
    g->log = (char *)gtk_entry_get_text(GTK_ENTRY(g->e_name));
    g->pass = (char *)gtk_entry_get_text(GTK_ENTRY(g->e_pass));

    (void)b;
    if (!mx_validate_login_data(g)) {
        mx_open_win_chat(g->w_log, g);
    }
}
