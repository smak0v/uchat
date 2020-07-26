#include "client.h"

static void append_play_btn(GtkWidget *file_box, t_glade *g) {
    GtkWidget *b_play = gtk_button_new();

    gtk_button_set_image(GTK_BUTTON(b_play),
        gtk_image_new_from_file("./src/client/ui/play.png"));

    gtk_style_context_add_class(gtk_widget_get_style_context(b_play),
        "b_play");

    g_signal_connect(b_play, "clicked", G_CALLBACK(mx_play), g);

    gtk_box_pack_start(GTK_BOX(file_box), b_play, FALSE, FALSE, 0);
}

static void append_download_btn(GtkWidget *file_box, t_glade *g) {
    GtkWidget *b_download = gtk_button_new();

    gtk_button_set_image(GTK_BUTTON(b_download),
        gtk_image_new_from_file("./src/client/ui/download.png"));

    gtk_style_context_add_class(gtk_widget_get_style_context(b_download),
        "b_download");

    g_signal_connect(b_download, "clicked", G_CALLBACK(mx_download), g);

    gtk_box_pack_start(GTK_BOX(file_box), b_download, FALSE, FALSE, 0);
}

static void append_filename_label(GtkWidget *file_box, GtkWidget *l_msg,
    char *filename) {
    GtkWidget *l_filename = gtk_label_new(filename);

    gtk_style_context_add_class(gtk_widget_get_style_context(l_filename),
        "l_filename");

    if (strcmp((char *)gtk_label_get_text(GTK_LABEL(l_msg)), ""))
        gtk_style_context_add_class(gtk_widget_get_style_context(l_msg),
            "msg_margin_bottom");

    gtk_label_set_xalign(GTK_LABEL(l_filename), 0.1);

    gtk_box_pack_end(GTK_BOX(file_box), l_filename, FALSE, FALSE, 0);
}

void mx_append_file_to_msg_block(GtkWidget *msg_v_box, json_object *msg,
    t_glade *g, GtkWidget *l_msg) {
    GtkWidget *file_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    char *filename = (char *)json_object_get_string(json_object_object_get(msg,
        "file"));

    if (filename && strcmp(filename, "")) {
        if (mx_is_audio(filename))
           append_play_btn(file_box, g);
        else
            append_download_btn(file_box, g);

        append_filename_label(file_box, l_msg, filename);

        gtk_box_pack_start(GTK_BOX(msg_v_box), file_box, FALSE, FALSE, 0);
    }
}
