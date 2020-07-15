#include "client.h"

void mx_open_dialog(GtkWidget *w, t_glade *g) {
    GtkWidget *parent = gtk_widget_get_parent(w);
    GList *childs = gtk_container_get_children(GTK_CONTAINER(parent));
    GtkWidget *l_id = GTK_WIDGET(g_list_nth_data(childs, 0));

    // mx_delete_childs(g->box8);
    // gtk_entry_set_text(GTK_ENTRY(g->e_chat_search), "");
    // gtk_widget_hide(g->d_add_chat);

    mx_delete_childs(g->messages_area);
    mx_clear_input_text(g);
    gtk_label_set_text(GTK_LABEL(g->l_chat_name),
        gtk_label_get_text(GTK_LABEL(g_list_nth_data(childs, 1))));

    gtk_widget_show(GTK_WIDGET(g->messages_area));
    gtk_widget_show(GTK_WIDGET(g->box_message));
    gtk_widget_show(GTK_WIDGET(g->e_search));
    gtk_widget_show(GTK_WIDGET(g->box5));
    gtk_widget_hide(GTK_WIDGET(g->l_select_chat));
    gtk_widget_hide(GTK_WIDGET(g->profile_area));

    g->group = false;
    g->dgid = mx_atoi((char *)gtk_label_get_text(GTK_LABEL(l_id)));

    mx_load_messages(g, time(NULL));

    g_list_free(childs);
    childs = NULL;
    (void)g;
}
