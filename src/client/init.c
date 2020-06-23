#include "uchat.h"

int mx_init_client(int argc, char **argv) {
    t_glade *g = malloc(sizeof(t_glade));
    GError *error = NULL;
    char *ui_path = mx_build_ui_path("builder.ui");
    
    gtk_init(&argc, &argv);
    g->bd = gtk_builder_new();
    if (gtk_builder_add_from_file(g->bd, ui_path, &error) == 0) {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return MX_FAILURE;
    }
    mx_connect_css("ex.css");
    mx_create_win_log(g);
    mx_create_win_chat(g);

    gtk_main();
    mx_strdel(&ui_path);
    return MX_SUCCESS;
}
