#include "uchat.h"

void mx_open_regwin(GtkWidget *sender, t_glade *g) {
    int w;
    int h;
    int x;
    int y;

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
    printf("==========Hide on click!==========\n");
}

void mx_b_reg_log(GtkButton *b, t_glade *g) {
    char *repeat = (char *)gtk_entry_get_text(GTK_ENTRY(g->r_repass));

    g->log = (char *)gtk_entry_get_text(GTK_ENTRY(g->r_ename));
    g->pass = (char *)gtk_entry_get_text(GTK_ENTRY(g->r_epass));
    if (strlen(g->log) < 3)
        mx_widget_set_visibility(g->bd, "r_name_err", TRUE);
    if (repeat) { // create fun to valid log and pass and repeat
        if (mx_strcmp(repeat, g->pass) != 0) {
            mx_printstr_endl("Incorrect pass");
            mx_widget_set_visibility(g->bd, "r_pass_err", TRUE);
            mx_widget_set_visibility(g->bd, "r_pass_err1", TRUE);
        }
    }
    // after valid user data - register into db and log into chat
    mx_open_win_chat(g->w_reg, g);

    mx_printstr_endl(g->log);
    mx_printstr_endl(g->pass);
    (void)b;
    printf("Reg button clicked\n"); // del
}

 void mx_b_log(GtkButton *b, t_glade *g) {
    g->log = (char *)gtk_entry_get_text(GTK_ENTRY(g->e_name));
    g->pass = (char *)gtk_entry_get_text(GTK_ENTRY(g->e_pass));

    // create valid func! and then send data to db and goto to open_win_chat
    if (strlen(g->log) < 3)
        mx_printstr_endl("Write username");
    if (strlen(g->pass) < 3)
        mx_printstr_endl("Write password");

    mx_printstr_endl(g->log);
    mx_printstr_endl(g->pass);
    (void)b;
    printf("Log in button clicked\n");

    // log into chat!
    mx_open_win_chat(g->w_log, g);
}
