#include "client.h"

static int check_uid2_in_dialogues(t_list *dialogues, char *uid2) {
    t_dialogue *dialogue = NULL;

    while (dialogues) {
        dialogue = (t_dialogue *)dialogues->data;

        if (dialogue->uid2 == mx_atoi(uid2))
            return dialogue->did;

        dialogues = dialogues->next;
    }

    return -1;
}

static void hide_show_dialog_widgets(t_glade *g) {
    gdk_threads_add_idle(mx_show_widget, g->messages_area);
    gdk_threads_add_idle(mx_show_widget, g->box_message);
    gdk_threads_add_idle(mx_show_widget, g->e_search);
    gdk_threads_add_idle(mx_show_widget, g->box9);
    gdk_threads_add_idle(mx_hide_widget, g->l_select_chat);
    gdk_threads_add_idle(mx_hide_widget, g->profile_area);
}

static void setup_g_data(t_glade *g, GtkWidget *l_uid2) {
    int did = check_uid2_in_dialogues(g->dialogues,
        (char *)gtk_label_get_text(GTK_LABEL(l_uid2)));

    g->group = false;
    g->uid2 = mx_atoi((char *)gtk_label_get_text(GTK_LABEL(l_uid2)));

    if (did >= 0)
        g->dgid = did;
    else
        g->dgid = -2;
}

void mx_open_dialog(GtkWidget *w, t_glade *g) {
    GtkWidget *parent = gtk_widget_get_parent(w);
    GList *childs = gtk_container_get_children(GTK_CONTAINER(parent));
    GtkWidget *l_uid2 = GTK_WIDGET(g_list_nth_data(childs, 0));

    mx_delete_childs(g->messages_area, true);
    mx_clear_input_text(g);

    gtk_label_set_text(GTK_LABEL(g->l_chat_name),
        gtk_label_get_text(GTK_LABEL(g_list_nth_data(childs, 1))));
    hide_show_dialog_widgets(g);
    setup_g_data(g, l_uid2);

    mx_load_messages_request(g, time(NULL));

    gdk_threads_add_idle(mx_hide_widget, g->d_add_chat);

    g_list_free(childs);
    childs = NULL;

    (void)g;
}
