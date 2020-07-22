#include "client.h"

void mx_widget_visible(GtkWidget *widget, gboolean is_visible) {
    if (is_visible)
        gdk_threads_add_idle(mx_show_widget, widget);
    else
        gdk_threads_add_idle(mx_hide_widget, widget);
}

void mx_widget_set_visibility(GtkBuilder *bd, gchar *name,
                              gboolean is_visible) {
    GtkWidget *widget = GTK_WIDGET(gtk_builder_get_object(bd, name));

    mx_widget_visible(widget, is_visible);
}

char *mx_build_ui_path(char *filename) {
    char *path = mx_strnew(mx_strlen(MX_UI_PATH) + mx_strlen(filename));

    path = mx_strcpy(path, MX_UI_PATH);
    path += mx_strlen(MX_UI_PATH);
    path = mx_strcpy(path, filename);
    path -= mx_strlen(MX_UI_PATH);

    return path;
}

int mx_connect_css(char *path) {
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    char *path_css = mx_build_ui_path(path);

    if (gtk_css_provider_load_from_path(cssProvider, path_css, NULL)) {
        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                          GTK_STYLE_PROVIDER(cssProvider),
                                          GTK_STYLE_PROVIDER_PRIORITY_USER);

        mx_strdel(&path_css);

        return MX_SUCCESS;
    }

    return MX_FAILURE;
}

void mx_entry_visibility(GtkButton *b, GtkWidget *entry) {
    (void)b;

    if (gtk_entry_get_visibility(GTK_ENTRY(entry)))
        gtk_entry_set_visibility(GTK_ENTRY(entry), false);
    else
        gtk_entry_set_visibility(GTK_ENTRY(entry), true);
}
