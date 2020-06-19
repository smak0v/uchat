#include "uchat.h"

static void clicked_log() {
   printf("Log in button clicked\n");
}

static void entry_changed(GtkEntry *s) {
    char *tmp = (char *)gtk_entry_get_text(s);
   
    mx_printstr_endl(tmp);    
}

static void back(GtkWidget *sender, t_glade *g) {
    printf("==========Go back!!!!==========\n");
    gtk_widget_hide(gtk_widget_get_toplevel(sender));
    mx_create_win_log(g);
    gtk_widget_show_all(g->w_log);
}

static void create_win_reg(t_glade *g) {
    g->w_reg = GTK_WIDGET(gtk_builder_get_object(g->bd, "win_reg"));
    g->b_reg_login = GTK_WIDGET(gtk_builder_get_object(g->bd, "b_reg_login"));
    
    g->r_ename = GTK_WIDGET(gtk_builder_get_object(g->bd, "reg_elogin"));
    g->r_epass = GTK_WIDGET(gtk_builder_get_object(g->bd, "reg_epass"));
    g->r_epass = GTK_WIDGET(gtk_builder_get_object(g->bd, "reg_erepeat"));
    g->b_reg_login = GTK_WIDGET(gtk_builder_get_object(g->bd, "b_reg_login"));
    g->b_reg_back = GTK_WIDGET(gtk_builder_get_object(g->bd, "b_reg_back"));

    g_signal_connect(g->b_reg_login, "clicked", G_CALLBACK(clicked_log), NULL);
    g_signal_connect(g->b_reg_back, "clicked", G_CALLBACK(back), g);
}

void mx_create_win_log(t_glade *g) {
    create_win_reg(g);
    g->w_log = GTK_WIDGET(gtk_builder_get_object(g->bd, "win_log"));
    g->e_name = GTK_WIDGET(gtk_builder_get_object(g->bd, "name_entry"));
    g->e_pass = GTK_WIDGET(gtk_builder_get_object(g->bd, "pass_entry"));
    g->b_log_in = GTK_WIDGET(gtk_builder_get_object(g->bd, "b_login"));
    g->b_reg = GTK_WIDGET(gtk_builder_get_object(g->bd, "b_reg"));

    g_signal_connect(g->w_log, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(g->e_name, "changed", G_CALLBACK(entry_changed), NULL);
    g_signal_connect(g->e_pass, "changed", G_CALLBACK(entry_changed), NULL);
    g_signal_connect(g->b_log_in, "clicked", G_CALLBACK(clicked_log), NULL);
    g_signal_connect(g->b_reg, "clicked", G_CALLBACK(mx_hide_win), g->w_reg);
}

