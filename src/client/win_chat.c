#include "uchat.h"

void mx_create_box(t_glade *g, char *tmp) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *label_text = gtk_label_new(NULL);

    mx_printstr_endl("Chat window1");
    gtk_box_pack_start(GTK_BOX(box), label_text, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(g->c_box), box, TRUE, TRUE, 0);
    gtk_label_set_text(GTK_LABEL(label_text), tmp);
    gtk_widget_show_all(box);
}

void go(GtkButton *b, t_glade *g) {
    char *tmp = (char *)gtk_entry_get_text(GTK_ENTRY(g->c_entry));
    // gtk_label_set_text(GTK_LABEL(g->c_label), tmp);
    (void)b;
    mx_printstr_endl("Chat button clicked");
    mx_create_box(g, tmp);
}

void mx_create_win_chat(t_glade *g) {
    g->w_chat = GTK_WIDGET(gtk_builder_get_object(g->bd, "win_chat"));
    g->c_box = GTK_WIDGET(gtk_builder_get_object(g->bd, "box3"));
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
