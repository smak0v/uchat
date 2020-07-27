#include "client.h"

static void build_user_box(GtkWidget *user_box, GtkWidget *l_id,
    char *username, t_glade *g) {
    GtkWidget *l_username = gtk_label_new(username);
    GtkWidget *b_open_dialog = gtk_button_new();

    gtk_button_set_image(GTK_BUTTON(b_open_dialog),
        gtk_image_new_from_file("./src/client/ui/chat.png"));
    gtk_label_set_xalign(GTK_LABEL(l_username), 0.1);
    gtk_box_pack_start(GTK_BOX(user_box), l_id, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(user_box), l_username, TRUE, TRUE, 0);

    if (g->uid != mx_atoi(gtk_label_get_text(GTK_LABEL(l_id))))
        gtk_box_pack_start(GTK_BOX(user_box), b_open_dialog, FALSE, FALSE, 0);

    g_signal_connect(b_open_dialog, "clicked", G_CALLBACK(mx_open_dialog), g);

    gtk_style_context_add_class(gtk_widget_get_style_context(
        user_box), "user_box");
    gtk_style_context_add_class(gtk_widget_get_style_context(
        b_open_dialog), "user_box_open_dialog_btn");
    gtk_style_context_add_class(gtk_widget_get_style_context(
        l_username), "user_box_username");
}

static void add_member_box_to_gui(json_object *jobj, t_glade *g) {
    json_object *j_id = json_object_object_get(jobj, "id");
    json_object *j_username = json_object_object_get(jobj, "name");
    char *id = mx_itoa(json_object_get_int(j_id));
    GtkWidget *user_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *l_id = gtk_label_new(id);

    build_user_box(user_box, l_id, (char *)json_object_get_string(j_username),
        g);

    gtk_box_pack_start(GTK_BOX(g->box13), user_box, FALSE, FALSE, 0);

    gtk_widget_show_all(user_box);
    gtk_widget_hide(l_id);

    mx_strdel(&id);
}

gboolean mx_parse_load_group_members_response(gpointer data) {
    json_object *jobj = json_tokener_parse(((t_main_thread *)data)->response);
    json_object *j_code = json_object_object_get(jobj, "code");
    json_object *j_members = NULL;

    if (json_object_get_int(j_code) == 200) {
        mx_delete_childs(((t_main_thread *)data)->g->box13);

        j_members = json_object_object_get(jobj, "mem");

        if (json_object_get_type(j_members) != json_type_null)
            for (size_t i = 0; i < json_object_array_length(j_members); ++i)
                add_member_box_to_gui(json_object_array_get_idx(j_members, i),
                    ((t_main_thread *)data)->g);
    }

    mx_clear_jobj(&jobj, MX_SUCCESS);
    mx_delete_main_thread_struct((t_main_thread **)&data);

    return G_SOURCE_REMOVE;
}
