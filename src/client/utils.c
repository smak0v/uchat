#include "uchat.h"

char *mx_build_ui_path(char *filename) {
    char *path = mx_strnew(mx_strlen(MX_UI_PATH) + mx_strlen(filename));

    path = mx_strcpy(path, MX_UI_PATH);
    path += mx_strlen(MX_UI_PATH);
    path = mx_strcpy(path, filename);
    path -= mx_strlen(MX_UI_PATH);

    return path;
}

// void mx_hide_win(GtkWidget *sender, GtkWidget *window) {
void mx_hide_win(GtkWidget *sender, GtkWindow *window) {
    printf("==========Hide on click!==========\n");
    gtk_widget_hide(gtk_widget_get_toplevel(sender));
    gtk_widget_show_all(GTK_WIDGET(window));
    // gtk_widget_show_all(window);
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
