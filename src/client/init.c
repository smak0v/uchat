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
    GtkWidget *win_log;
    GtkWidget *name_entry;
    GtkWidget *pass_entry;
    GtkWidget *button_log_in;
    GtkWidget *button_reg;
    GError *error = NULL;
    char *ui_path = mx_build_ui_path("builder.ui");
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    
    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, ui_path, &error) == 0) {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return MX_FAILURE;
    }
    win_log = GTK_WIDGET(gtk_builder_get_object(builder, "win_log"));

    name_entry = GTK_WIDGET(gtk_builder_get_object(builder, "name_entry"));
    pass_entry = GTK_WIDGET(gtk_builder_get_object(builder, "pass_entry"));
    button_log_in = GTK_WIDGET(gtk_builder_get_object(builder, "b_login"));
    button_reg = GTK_WIDGET(gtk_builder_get_object(builder, "b_reg"));
    
    gtk_css_provider_load_from_path(cssProvider, mx_build_ui_path("ex.css"), NULL);

    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                          GTK_STYLE_PROVIDER(cssProvider),
                                          GTK_STYLE_PROVIDER_PRIORITY_USER);


    g_signal_connect(win_log, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(name_entry, "changed", G_CALLBACK(entry_changed), NULL);
    g_signal_connect(pass_entry, "changed", G_CALLBACK(entry_changed), NULL);
    g_signal_connect(button_log_in, "clicked", G_CALLBACK(clicked_log), NULL);
    g_signal_connect(button_reg, "clicked", G_CALLBACK(clicked_reg), NULL);

    gtk_main();
    mx_strdel(&ui_path);

    return MX_SUCCESS;
}
