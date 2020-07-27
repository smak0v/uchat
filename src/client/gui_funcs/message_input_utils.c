#include "client.h"

void mx_clear_input_text(t_glade *g) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(
        GTK_TEXT_VIEW(g->e_message));
    GtkTextIter start;
    GtkTextIter end;

    gtk_text_buffer_get_bounds(buffer, &start, &end);
    gtk_text_buffer_delete(buffer, &start, &end);
}

char *mx_get_input_text(t_glade *g) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(
        GTK_TEXT_VIEW(g->e_message));
    GtkTextIter start;
    GtkTextIter end;

    gtk_text_buffer_get_bounds(buffer, &start, &end);
    return (char *)gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
}

void mx_scroll_to_bottom(GtkWidget *w,  GdkRectangle *a, t_glade *g) {
    GtkAdjustment *v_adjustment = gtk_scrolled_window_get_vadjustment(
        GTK_SCROLLED_WINDOW(g->scrolledwindow1));

    gtk_adjustment_set_value(GTK_ADJUSTMENT(v_adjustment),
        gtk_adjustment_get_upper(GTK_ADJUSTMENT(v_adjustment)));

    (void)w;
    (void)a;
}

void mx_add_id_to_msg_block(json_object *msg, GtkWidget *msg_vbox,
    t_glade *g, bool play) {
    GtkWidget *l_msg_id = gtk_label_new(json_object_get_string(
        json_object_object_get(msg, "mid")));
    GtkWidget *uid = gtk_label_new(json_object_get_string(
        json_object_object_get(msg, "uid")));

    gtk_box_pack_start(GTK_BOX(msg_vbox), l_msg_id, FALSE, FALSE, 0);

    gtk_widget_hide(l_msg_id);

    if (g->uid != mx_atoi((char *)gtk_label_get_text(GTK_LABEL(uid))) && play)
        mx_play_audio_thread(g, MX_DILIN_PATH);
}
