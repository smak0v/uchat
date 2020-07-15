#include "client.h"

static void init_client(int argc, char **argv) {
    t_glade *g = mx_memalloc(sizeof(t_glade));
    GError *error = NULL;
    char *ui_path = mx_build_ui_path("builder.ui");

    gtk_init(&argc, &argv);
    g->bd = gtk_builder_new();
    g->ip = argv[1];
    if (gtk_builder_add_from_file(g->bd, ui_path, &error) == 0) {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
    }

    mx_connect_css("dark.css");
    mx_create_win_log(g);
    mx_create_win_chat(g);

    mx_start_client(argv[1], mx_atoi(argv[2]), g);

    mx_strdel(&ui_path);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        mx_print_error("uchat: must take two parameters -");
        mx_print_error_endl("server IP address and port");
        mx_print_error_endl("usage: ./uchat IP PORT");
        exit(MX_FAILURE);
    }
    else {
        if (!mx_check_ip(strdup(argv[1]))) {
            mx_print_error_endl("uchat: not valid IP address");
            exit(MX_FAILURE);
        }
        if (!mx_check_port(argv[2])) {
            mx_print_error_endl("uchat: not valid port");
            exit(MX_FAILURE);
        }
        init_client(argc, argv);
    }

    return MX_SUCCESS;
}
