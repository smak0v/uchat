#include "client.h"

static void download(GList *childs0, t_glade *g) {
    GList *childs1 = gtk_container_get_children(GTK_CONTAINER(
        GTK_WIDGET(g_list_nth_data(childs0, 1))));
    GtkWidget *msg_id = GTK_WIDGET(g_list_nth_data(childs1, 0));
    char *request = mx_json_string_download_file(g->token, g->uid,
        mx_atoi((char *)gtk_label_get_text(GTK_LABEL(msg_id))));

    SSL_write(g->ssl, request, strlen(request));

    mx_strdel(&request);
    g_list_free(childs1);
}

static char *get_filename(GList *childs0) {
    GtkWidget *message_box = GTK_WIDGET(g_list_nth_data(childs0, 2));
    GList *childs2 = gtk_container_get_children(GTK_CONTAINER(message_box));
    GtkWidget *l_filename = GTK_WIDGET(g_list_nth_data(childs2, 1));
    char *filename = mx_strdup(
        (char *)gtk_label_get_text(GTK_LABEL(l_filename)));

    g_list_free(childs2);

    return filename;
}

gboolean mx_send_audio(GtkWidget *w, GdkEventKey *e, t_glade *g) {
    pthread_mutex_lock(&g->recorder_mutex);
    g->record_audio_pressed = false;
    pthread_mutex_unlock(&g->recorder_mutex);

    (void)w;
    (void)e;

    return FALSE;
}

void mx_play(GtkWidget *w, t_glade *g) {
    GList *childs0 = gtk_container_get_children(GTK_CONTAINER(
        gtk_widget_get_parent(gtk_widget_get_parent(w))));
    char *filename = get_filename(childs0);
    char *tmp = mx_strjoin(getpwuid(getuid())->pw_dir, "/Downloads/");
    char *path = NULL;

    download(childs0, g);

    path = mx_strjoin(tmp, filename);

    mx_play_audio_thread(g, path);

    mx_strdel(&tmp);
    mx_strdel(&path);
    g_list_free(childs0);
}
