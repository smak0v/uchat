#include "uchat.h"

void clicked_log() {
   printf("Log in button clicked\n");
}

void clicked_reg() {
   printf("Reg button clicked!\n");
}

void entry_changed(GtkEntry *s) {
    char *tmp = (char *)gtk_entry_get_text(s);

    mx_printstr_endl(tmp);    
}

int mx_init_client(int argc, char **argv) {
    GtkBuilder *builder;
    GObject *win_log;
    GObject *name_entry;
    GObject *pass_entry;
    GObject *button_log_in;
    GObject *button_reg;
    GError *error = NULL;
    char *ui_path = mx_build_ui_path("builder.ui");

    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, ui_path, &error) == 0) {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return MX_FAILURE;
    }

    win_log = gtk_builder_get_object(builder, "win_log");
    name_entry = gtk_builder_get_object(builder, "name_entry");
    pass_entry = gtk_builder_get_object(builder, "pass_entry");
    button_log_in = gtk_builder_get_object(builder, "b_login");
    button_reg = gtk_builder_get_object(builder, "b_reg");

    g_signal_connect(win_log, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(name_entry, "changed", G_CALLBACK(entry_changed), NULL);
    g_signal_connect(pass_entry, "changed", G_CALLBACK(entry_changed), NULL);
    g_signal_connect(button_log_in, "clicked", G_CALLBACK(clicked_log), NULL);
    g_signal_connect(button_reg, "clicked", G_CALLBACK(clicked_reg), NULL);

    gtk_main();
    mx_strdel(&ui_path);

    return MX_SUCCESS;
}
