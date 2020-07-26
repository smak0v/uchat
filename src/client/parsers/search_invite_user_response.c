#include "client.h"

static void set_user_box_styles(GtkWidget *user_box,
    GtkWidget *b_invite, GtkWidget *l_username) {
    gtk_style_context_add_class(gtk_widget_get_style_context(
        user_box), "user_box");

    gtk_style_context_add_class(gtk_widget_get_style_context(
        b_invite), "user_box_open_dialog_btn");

    gtk_style_context_add_class(gtk_widget_get_style_context(
        l_username), "user_box_username");
}

static void build_user_box(GtkWidget *user_box, GtkWidget *l_id,
    char *username, t_glade *g) {
    GtkWidget *l_username = gtk_label_new(username);
    GtkWidget *b_invite = gtk_button_new();

    gtk_button_set_image(GTK_BUTTON(b_invite),
        gtk_image_new_from_file("./src/client/ui/add.png"));

    gtk_label_set_xalign(GTK_LABEL(l_username), 0.1);

    gtk_box_pack_start(GTK_BOX(user_box), l_id, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(user_box), l_username, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(user_box), b_invite, FALSE, FALSE, 0);

    g_signal_connect(b_invite, "clicked", G_CALLBACK(mx_invite_user_to_group),
        g);

    set_user_box_styles(user_box, b_invite, l_username);
}

static void add_user_box_to_gui(json_object *jobj, t_glade *g) {
    json_object *j_id = json_object_object_get(jobj, "id");
    json_object *j_username = json_object_object_get(jobj, "name");
    char *id = mx_itoa(json_object_get_int(j_id));
    GtkWidget *user_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *l_id = gtk_label_new(id);

    build_user_box(user_box, l_id, (char *)json_object_get_string(j_username),
        g);

    gtk_box_pack_start(GTK_BOX(g->box10), user_box, FALSE, FALSE, 0);

    gtk_widget_show_all(user_box);
    gtk_widget_hide(l_id);

    mx_strdel(&id);
}

gboolean mx_parse_invite_users(gpointer data) {
    json_object *jobj = json_tokener_parse(((t_main_thread *)data)->response);
    json_object *j_users = json_object_object_get(jobj, "users");
    GtkWidget *l_not_users_found = NULL;

    if (json_object_get_type(j_users) != json_type_null) {
        for (size_t i = 0; i < json_object_array_length(j_users); ++i)
            add_user_box_to_gui(json_object_array_get_idx(j_users, i),
                ((t_main_thread *)data)->g);
    }
    else {
        l_not_users_found = gtk_label_new("No users found");
        gtk_style_context_add_class(gtk_widget_get_style_context(
            l_not_users_found), "l_not_users_found");
        gtk_box_pack_start(GTK_BOX(((t_main_thread *)data)->g->box10),
            l_not_users_found, TRUE, TRUE, 0);
        gtk_widget_show(l_not_users_found);
    }
    mx_clear_jobj(&jobj, MX_SUCCESS);
    mx_delete_main_thread_struct((t_main_thread **)&data);
    return G_SOURCE_REMOVE;
}
