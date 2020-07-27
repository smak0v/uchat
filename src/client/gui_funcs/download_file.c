#include "client.h"

static void download_file(t_glade *g, int mid) {
    char *request = mx_json_string_download_file(g->token, g->uid, mid);

    SSL_write(g->ssl, request, strlen(request));

    mx_strdel(&request);
}

void mx_download(GtkWidget *w, t_glade *g) {
    GtkWidget *file_box = gtk_widget_get_parent(w);
    GtkWidget *msg_box = gtk_widget_get_parent(file_box);
    GList *childs0 = gtk_container_get_children(GTK_CONTAINER(msg_box));
    GList *childs1 = gtk_container_get_children(GTK_CONTAINER(
        GTK_WIDGET(g_list_nth_data(childs0, 1))));
    GtkWidget *msg_id = GTK_WIDGET(g_list_nth_data(childs1, 0));

    download_file(g, mx_atoi((char *)gtk_label_get_text(GTK_LABEL(msg_id))));

    g_list_free(childs0);
    g_list_free(childs1);
}
