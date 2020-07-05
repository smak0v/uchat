#include "client.h"

void mx_open_logwin(GtkWidget *sender, t_glade *g) {
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;

    (void)sender;
    mx_clear_signup_inputs(g);

    gtk_window_get_position(GTK_WINDOW(g->w_reg), &x, &y);
    gtk_window_get_size(GTK_WINDOW(g->w_reg), &w, &h);
    gtk_widget_hide(g->w_reg);
    gtk_widget_hide(g->w_chat);

    gtk_window_resize(GTK_WINDOW(g->w_log), w, h);
    gtk_window_move(GTK_WINDOW(g->w_log), x, y + 22.35);
    gtk_widget_show_all(GTK_WIDGET(g->w_log));
}

void mx_open_regwin(GtkWidget *sender, t_glade *g) {
    int w = 0;
    int h = 0;
    int x = 0;
    int y = 0;

    (void)sender;
    mx_clear_login_inputs(g);

    gtk_window_get_position(GTK_WINDOW(g->w_log), &x, &y);
    gtk_window_get_size(GTK_WINDOW(g->w_log), &w, &h);
    gtk_widget_hide(g->w_log);
    gtk_widget_hide(g->w_chat);

    gtk_window_resize(GTK_WINDOW(g->w_reg), w, h);
    gtk_window_move(GTK_WINDOW(g->w_reg), x, y + 22.35);
    gtk_widget_show_all(GTK_WIDGET(g->w_reg));

    mx_widget_set_visibility(g->bd, "r_pass_err", FALSE);
    mx_widget_set_visibility(g->bd, "r_pass_err1", FALSE);
    mx_widget_set_visibility(g->bd, "r_name_err", FALSE);
}

void mx_create_win_reg(t_glade *g) {
    g->w_reg = mx_get_gtk_obj(g, "win_reg");
    g->b_reg_login = mx_get_gtk_obj(g, "b_reg_login");
    g->r_ename = mx_get_gtk_obj(g, "reg_elogin");
    g->r_epass = mx_get_gtk_obj(g, "reg_epass");
    g->r_repass = mx_get_gtk_obj(g, "reg_erepeat");
    g->b_reg_login = mx_get_gtk_obj(g, "b_reg_login");
    g->b_reg_back = mx_get_gtk_obj(g, "b_reg_back");

    g_signal_connect(g->w_reg, "destroy", G_CALLBACK(mx_gtk_quit), NULL);
    g_signal_connect(g->b_reg_back, "clicked", G_CALLBACK(mx_open_logwin), g);
    g_signal_connect(g->b_reg_login, "clicked", G_CALLBACK(mx_b_reg_log), g);
}

void mx_create_win_log(t_glade *g) {
    mx_create_win_reg(g);

    g->w_log = mx_get_gtk_obj(g, "win_log");
    g->e_name = mx_get_gtk_obj(g, "name_entry");
    g->e_pass = mx_get_gtk_obj(g, "pass_entry");
    g->b_log_in = mx_get_gtk_obj(g, "b_login");
    g->b_reg = mx_get_gtk_obj(g, "b_reg");

    g_signal_connect(g->w_log, "destroy", G_CALLBACK(mx_gtk_quit), NULL);
    g_signal_connect(g->b_reg, "clicked", G_CALLBACK(mx_open_regwin), g);
    g_signal_connect(g->b_log_in, "clicked", G_CALLBACK(mx_b_log), g);
}
