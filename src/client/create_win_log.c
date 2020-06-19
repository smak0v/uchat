#include "uchat.h"

static void clicked_log() {
   printf("Log in button clicked\n");
}

static void entry_changed(GtkEntry *s) {
    char *tmp = (char *)gtk_entry_get_text(s);
   
    mx_printstr_endl(tmp);    
}


static void create_win_reg(t_glade *g) {
    g->w_reg = GTK_WIDGET(gtk_builder_get_object(g->bd, "window1"));
    g->button1 = GTK_WIDGET(gtk_builder_get_object(g->bd, "button1"));
    
    g_signal_connect(g->button1, "clicked", G_CALLBACK(clicked_log), NULL);    
}

void mx_create_win_log(t_glade *g) {
    create_win_reg(g);
    g->w_log = GTK_WIDGET(gtk_builder_get_object(g->bd, "win_log"));
    g->e_name = GTK_WIDGET(gtk_builder_get_object(g->bd, "name_entry"));
    g->e_pass = GTK_WIDGET(gtk_builder_get_object(g->bd, "pass_entry"));
    g->b_log_in = GTK_WIDGET(gtk_builder_get_object(g->bd, "b_login"));
    g->b_reg = GTK_WIDGET(gtk_builder_get_object(g->bd, "b_reg"));
    g->button1 = GTK_WIDGET(gtk_builder_get_object(g->bd, "button1"));

    g_signal_connect(g->w_log, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(g->e_name, "changed", G_CALLBACK(entry_changed), NULL);
    g_signal_connect(g->e_pass, "changed", G_CALLBACK(entry_changed), NULL);
    g_signal_connect(g->b_log_in, "clicked", G_CALLBACK(clicked_log), NULL);
    g_signal_connect(g->b_reg, "clicked", G_CALLBACK(mx_hide_win), g->w_reg);
}

