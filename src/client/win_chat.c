#include "uchat.h"

void go(GtkButton *b, t_glade *g) {
    char tmp[128];
    sprintf(tmp, "entry=%s", gtk_entry_get_text(GTK_ENTRY(g->c_entry)));
    gtk_label_set_text(GTK_LABEL(g->c_label), (const gchar *)tmp);
    (void)b;
    mx_printstr_endl("Chat button clicked");
    
}

void mx_create_win_chat(t_glade *g) {
    g->w_chat = GTK_WIDGET(gtk_builder_get_object(g->bd, "win_chat"));
    g->c_label = GTK_WIDGET(gtk_builder_get_object(g->bd, "label1"));
    g->c_entry = GTK_WIDGET(gtk_builder_get_object(g->bd, "entry1"));
    g->c_b_go = GTK_WIDGET(gtk_builder_get_object(g->bd, "go"));
    mx_printstr_endl("Chat window");
    
    g_signal_connect(g->c_b_go, "clicked", G_CALLBACK(go), g);
}

void mx_show_win_chat(GtkWidget *v, t_glade *g) {
    int w;
    int h;
    int x;
    int y;
    
    gtk_window_get_position(GTK_WINDOW(v), &x, &y);
    gtk_window_get_size(GTK_WINDOW(v), &w, &h);
    gtk_widget_hide(GTK_WIDGET(v));

    gtk_window_resize(GTK_WINDOW(g->w_chat), w, h);
    gtk_window_move(GTK_WINDOW(g->w_chat), x, y + 22.35);
    gtk_widget_show_all(GTK_WIDGET(g->w_chat));

    printf("==========Enter in chat!==========\n");
}

void mx_open_win_chat(GtkWidget *w, t_glade *g) {
    mx_show_win_chat(w, g);

}
