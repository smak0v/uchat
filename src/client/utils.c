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
void mx_hide_win(GtkWidget *sender, t_glade *g) {
    // gint x = 0;
    // gint y = 0;
    int w,h;
    int x,y;
    
    sender = NULL;
    gtk_window_get_position(GTK_WINDOW(g->w_log), &x, &y);
    gtk_window_get_size(GTK_WINDOW(g->w_log), &w, &h);

    gtk_widget_hide(g->w_log);

    // gtk_window_set_position(g->w_reg, x, y);
    gtk_window_resize(GTK_WINDOW(g->w_reg), w, h);
    gtk_window_move(GTK_WINDOW(g->w_reg), x, y + 22.35);
    
    gtk_widget_show_all(GTK_WIDGET(g->w_reg));
    // gtk_widget_show_all(window);
    printf("==========Hide on click!==========\n");
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
