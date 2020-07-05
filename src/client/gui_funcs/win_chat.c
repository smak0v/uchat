#include "client.h"

// void mx_send_msg(GtkWidget *top, t_glade *g) {
//     GtkWidget *row = gtk_list_box_row_new();
//     gtk_list_box_row_set_selectable (GTK_LIST_BOX_ROW(row), TRUE);
//     gtk_list_box_insert (GTK_LIST_BOX(g->listbox), row, g->n);
//     // g->message_num++;
//     (void)top;
//     GtkWidget *gridmenu = gtk_grid_new();
//     gtk_container_add(GTK_CONTAINER(row), gridmenu);

//     char *tmp = (char *)gtk_entry_get_text(GTK_ENTRY(g->entry1));
//     GtkWidget *label = gtk_label_new("");
//     mx_printstr_endl(tmp);
//     gtk_label_set_text(GTK_LABEL(label), tmp);

//     GtkWidget *time = gtk_label_new("21:45:34 ");
//     GtkWidget *name = gtk_label_new("name ");

//     gtk_grid_attach(GTK_GRID(gridmenu), name, 0, 0, 1, 1);
//     gtk_grid_attach(GTK_GRID(gridmenu), time, 0, 1, 1, 1);
//     gtk_grid_attach(GTK_GRID(gridmenu), label, 1, 1, 1, 1);

//     gtk_widget_show_all(row);
// }

// void mx_create_box(t_glade *g, char *tmp) {
//     GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
//     GtkWidget *label_text = gtk_label_new(NULL);

//     mx_printstr_endl("Chat window1");
//     gtk_box_pack_start(GTK_BOX(box), label_text, FALSE, FALSE, 0);
//     gtk_box_pack_end(GTK_BOX(g->c_box), box, TRUE, TRUE, 0);
//     gtk_label_set_text(GTK_LABEL(label_text), tmp);
//     gtk_widget_show_all(box);
// }

// void go(GtkButton *b, t_glade *g) {
    // char *tmp = (char *)gtk_entry_get_text(GTK_ENTRY(g->c_entry));
//     // gtk_label_set_text(GTK_LABEL(g->c_label), tmp);
//     (void)b;
//     mx_printstr_endl("Chat button clicked");
//     mx_create_box(g, tmp);
// }

void mx_create_win_chat(t_glade *g) {
    g->w_chat = GTK_WIDGET(gtk_builder_get_object(g->bd, "win_chat"));
    g->b_logout = GTK_WIDGET(gtk_builder_get_object(g->bd, "b_logout"));
    // g->b_send_msg = GTK_WIDGET(gtk_builder_get_object(g->bd, "b_send_msg"));

    // g->box1 = GTK_WIDGET(gtk_builder_get_object(g->bd, "box1"));
    // g->button1 = GTK_WIDGET(gtk_builder_get_object(g->bd, "button1"));
    // g->box2 = GTK_WIDGET(gtk_builder_get_object(g->bd, "box2"));
    // g->box3 = GTK_WIDGET(gtk_builder_get_object(g->bd, "box3"));
    // g->box4 = GTK_WIDGET(gtk_builder_get_object(g->bd, "box4"));
    // g->box5 = GTK_WIDGET(gtk_builder_get_object(g->bd, "box5"));
    // g->label1 = GTK_WIDGET(gtk_builder_get_object(g->bd, "label1"));
    // g->label2 = GTK_WIDGET(gtk_builder_get_object(g->bd, "label2"));
    // g->box3 = GTK_WIDGET(gtk_builder_get_object(g->bd, "box3"));
    // g->entry1 = GTK_WIDGET(gtk_builder_get_object(g->bd, "entry1"));
    // GtkWidget *l = gtk_label_new("Group name");
    // GtkWidget *l1 = gtk_label_new("->> name");
    // g->b1 = gtk_button_new();
    // g->box_new = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
    // g->scroll = gtk_scrolled_window_new(0,0);
    // gtk_widget_set_name(g->scroll, "scroll");
    // g->listbox = gtk_list_box_new();
    // g->msg_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    // g->entry = gtk_entry_new();
    // g->send = gtk_button_new();
    // g->n = 0;
    // g->message_num = 0;
    // gtk_widget_set_size_request(g->scroll,30,30);
    // gtk_box_pack_start(GTK_BOX(g->box2), l, FALSE, FALSE, 0);
    // gtk_box_pack_start(GTK_BOX(g->box), g->b1, TRUE, TRUE, 0);
    // gtk_box_pack_start(GTK_BOX(g->box_new), b, TRUE, TRUE, 0);
    // gtk_box_pack_end(GTK_BOX(g->box), g->box_new, TRUE, TRUE, 0);
    // gtk_box_pack_start(GTK_BOX(g->box2), g->scroll, TRUE, TRUE, 0);
    // gtk_box_pack_end(GTK_BOX(g->box_new), g->msg_box, TRUE, TRUE, 0);
    // gtk_box_pack_end(GTK_BOX(g->msg_box), g->send, TRUE, TRUE, 0);
    // gtk_box_pack_start(GTK_BOX(g->msg_box), g->entry, TRUE, TRUE, 0);
    // gtk_widget_set_name(listbox,"listboxleft");
    // gtk_container_add(GTK_CONTAINER(g->scroll), g->listbox);
    // gtk_widget_show_all(g->w_chat);



    // gtk_entry_set_placeholder_text(GTK_ENTRY(g->entry1),"Write a message...");

    g_signal_connect(g->w_chat, "destroy", G_CALLBACK(mx_gtk_quit), NULL);
    // g_signal_connect(g->b_send_msg, "clicked", G_CALLBACK(mx_send_msg), g);
    g_signal_connect(g->b_logout, "clicked", G_CALLBACK(mx_b_logout), g);
}

void mx_show_win_chat(GtkWidget *v, t_glade *g) {
    int w = 0;
    int h = 0;
    int x = 0;
    int y= 0;

    gtk_window_get_position(GTK_WINDOW(v), &x, &y);
    gtk_window_get_size(GTK_WINDOW(v), &w, &h);
    gtk_widget_hide(GTK_WIDGET(v));

    gtk_window_resize(GTK_WINDOW(g->w_chat), w, h);
    gtk_window_move(GTK_WINDOW(g->w_chat), x, y + 22.35);
    gtk_widget_show_all(GTK_WIDGET(g->w_chat));
}

void mx_open_win_chat(GtkWidget *w, t_glade *g) {
    mx_show_win_chat(w, g);
}
