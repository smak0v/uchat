#include "client.h"

static void set_user_box_styles(GtkWidget *user_box,
    GtkWidget *b_open_dialog, GtkWidget *l_username) {
    gtk_style_context_add_class(gtk_widget_get_style_context(
        user_box), "user_box");
    gtk_style_context_add_class(gtk_widget_get_style_context(
        b_open_dialog), "user_box_open_dialog_btn");
    gtk_style_context_add_class(gtk_widget_get_style_context(
        l_username), "user_box_username");
}

static void build_user_box(GtkWidget *user_box, GtkWidget *l_id,
    char *username, t_glade *g) {
    GtkWidget *l_username = gtk_label_new(username);
    GtkWidget *b_open_dialog = gtk_button_new();

    gtk_button_set_image(GTK_BUTTON(b_open_dialog),
        gtk_image_new_from_file("./src/client/ui/chat.png"));

    gtk_label_set_xalign(GTK_LABEL(l_username), 0.1);

    gtk_box_pack_start(GTK_BOX(user_box), l_id, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(user_box), l_username, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(user_box), b_open_dialog, FALSE, FALSE, 0);

    g_signal_connect(b_open_dialog, "clicked", G_CALLBACK(mx_open_dialog), g);

    set_user_box_styles(user_box, b_open_dialog, l_username);
}

static void add_user_box_to_gui(json_object *jobj, t_glade *g) {
    json_object *j_id = json_object_object_get(jobj, "id");
    json_object *j_username = json_object_object_get(jobj, "name");
    char *id = mx_itoa(json_object_get_int(j_id));
    GtkWidget *user_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *l_id = gtk_label_new(id);

    build_user_box(user_box, l_id, (char *)json_object_get_string(j_username),
        g);
    gtk_box_pack_start(GTK_BOX(g->box8), user_box, FALSE, FALSE, 0);

    gdk_threads_add_idle(mx_show_all_widget, user_box);
    gdk_threads_add_idle(mx_hide_widget, l_id);

    mx_strdel(&id);
}

static void parse_users(json_object *jobj, t_glade *g) {
    json_object *j_users = json_object_object_get(jobj, "users");
    GtkWidget *l_not_users_found = NULL;
    int len = 0;

    if (json_object_get_type(j_users) != json_type_null) {
        len = json_object_array_length(j_users);
        for (int i = 0; i < len; ++i)
            add_user_box_to_gui(json_object_array_get_idx(j_users, i), g);
    }
    else {
        l_not_users_found = gtk_label_new("No users found");
        gtk_style_context_add_class(gtk_widget_get_style_context(
            l_not_users_found), "l_not_users_found");
        gtk_box_pack_start(GTK_BOX(g->box8), l_not_users_found, TRUE, TRUE, 0);
        gdk_threads_add_idle(mx_show_widget, l_not_users_found);
    }
}

void mx_parse_serach_user_response(char *response, t_glade *g) {
    json_object *jobj = json_tokener_parse(response);
    json_object *j_code = NULL;

    if (json_object_get_type(jobj) == json_type_object) {
        json_object_object_get_ex(jobj, "code", &j_code);
        if (j_code && json_object_get_type(j_code) == json_type_int) {
            if (json_object_get_int(j_code) == 200) {
                parse_users(jobj, g);
                mx_clear_jobj(&jobj, MX_SUCCESS);
            }
            else
                mx_clear_jobj(&jobj, MX_FAILURE);
        }

        mx_clear_jobj(&jobj, MX_FAILURE);
    }

    mx_clear_jobj(&jobj, MX_FAILURE);
}
