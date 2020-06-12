#include "uchat.h"

// static void create_paned_layout(GtkWidget *window) {
//     GtkWidget *vertical_paned = gtk_paned_new(GTK_ORIENTATION_VERTICAL);
//     GtkWidget *frame_1 = gtk_frame_new(NULL);
//     GtkWidget *frame_2 = gtk_frame_new(NULL);

//     gtk_frame_set_shadow_type(GTK_FRAME(frame_1), GTK_SHADOW_IN);
//     gtk_frame_set_shadow_type(GTK_FRAME(frame_2), GTK_SHADOW_IN);

//     gtk_widget_set_size_request(vertical_paned, 200, -1);

//     gtk_paned_pack1(GTK_PANED(vertical_paned), frame_1, TRUE, FALSE);
//     gtk_widget_set_size_request(frame_1, 50, -1);

//     gtk_paned_pack2(GTK_PANED(vertical_paned), frame_2, FALSE, FALSE);
//     gtk_widget_set_size_request(frame_2, 50, -1);

//     gtk_container_add(GTK_CONTAINER(window), vertical_paned);
// }

//  int mx_init_client(int argc, char **argv) {
//     int status = MX_SUCCESS;
//     GtkWidget *window = NULL;

//     gtk_init(&argc, &argv);

//     window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//     gtk_window_set_title(GTK_WINDOW(window), "UCHAT");
//     gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
//     g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

//     create_paned_layout(window);

//     gtk_widget_show_all(window);
//     gtk_main();

//     return status;
//  }

// static void helloworld (GtkButton *button, gpointer label) {
//     gtk_label_set_text (label, "Привет, Хабр!");
// }

static GtkWidget *create_window(char *path) {
    GtkWidget *window = NULL;
    GError* error = NULL;
    GtkBuilder *builder = gtk_builder_new();

    if (!gtk_builder_add_from_file(builder, path, &error)) {
        g_critical("Не могу загрузить файл: %s", error->message);
        g_error_free(error);
    }
    gtk_builder_connect_signals (builder, NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    if (!window) {
        g_critical ("Ошибка при получении виджета окна");
    }

    g_object_unref (builder);
    return window;
}

int mx_init_client(int argc, char **argv) {
    GtkWidget *window = NULL;

    gtk_init (&argc, &argv);

    window = create_window("builder.ui");
    g_signal_connect(window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    gtk_widget_show(window);
    gtk_main();

    return MX_SUCCESS;
}
