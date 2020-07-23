#include "client.h"

static void open_group(GtkWindow *event_box, GdkEvent *e, t_glade *g) {
    GList *childs = gtk_container_get_children(GTK_CONTAINER(
        gtk_bin_get_child(GTK_BIN(event_box))));
    GtkWidget *id = GTK_WIDGET(g_list_nth_data(childs, 0));

    if (g->dgid != mx_atoi((char *)gtk_label_get_text(GTK_LABEL(id))) && e++) {
        mx_delete_childs(g->messages_area);
        mx_clear_input_text(g);
        gtk_label_set_text(GTK_LABEL(g->l_chat_name),
            gtk_label_get_text(GTK_LABEL(g_list_nth_data(childs, 1))));
        g->group = true;
        g->dgid = mx_atoi((char *)gtk_label_get_text(GTK_LABEL(id)));
        mx_show_hide_chat_group_utils(g);
        mx_load_messages_request(g, time(NULL));
        g_list_free(childs);
    }
}

static void add_group_to_gui(t_glade *g, int gid, char *name) {
    GtkWidget *event_box = gtk_event_box_new();
    GtkWidget *group_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *l_name = gtk_label_new(name);
    GtkWidget *l_uid = gtk_label_new(mx_itoa(gid));

    gtk_box_pack_start(GTK_BOX(g->groups_box), event_box, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(event_box), group_box);
    gtk_box_pack_end(GTK_BOX(group_box), l_name, TRUE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(group_box), l_uid, TRUE, TRUE, 0);
    gtk_style_context_add_class(gtk_widget_get_style_context(group_box),
        "chat_group_box");
    g_signal_connect(event_box, "button_press_event",
        G_CALLBACK(open_group), g);
    gtk_widget_realize(event_box);
    gtk_widget_add_events(event_box, GDK_BUTTON_PRESS_MASK);
    gdk_threads_add_idle(mx_show_all_widget, event_box);
    gdk_threads_add_idle(mx_hide_widget, l_uid);
}

static void parse_arrays(t_glade *g,  int len, json_object *j_gids,
    json_object *j_names) {
    json_object *j_gid_val = NULL;
    json_object *j_name_val = NULL;

    for (int i = 0; i < len; ++i) {
        j_gid_val = json_object_array_get_idx(j_gids, i);
        j_name_val = json_object_array_get_idx(j_names, i);

        add_group_to_gui(g, json_object_get_int(j_gid_val),
            (char *)json_object_get_string(j_name_val));
    }
}

static int check_response_code(int code, json_object *jobj, t_glade *g) {
    json_object *j_len = NULL;
    json_object *j_gids = NULL;
    json_object *j_names = NULL;
    int len = 0;

    if (code == 400 || code == 401 || code == 500)
        return MX_FAILURE;
    else {
        json_object_object_get_ex(jobj, "len", &j_len);
        if (j_len && json_object_get_type(j_len) == json_type_int
            && (len = json_object_get_int(j_len)) > 0) {
            j_gids = json_object_object_get(jobj, "gid");
            j_names = json_object_object_get(jobj, "names");
            parse_arrays(g, len, j_gids, j_names);
        }
        return MX_SUCCESS;
    }
}

void mx_parse_load_groups_response(char *response, t_glade *g) {
    json_object *jobj = json_tokener_parse(response);
    json_object *j_code = NULL;

    if (json_object_get_type(jobj) == json_type_object) {
        json_object_object_get_ex(jobj, "code", &j_code);
        if (j_code && json_object_get_type(j_code) == json_type_int) {
            if (!check_response_code(json_object_get_int(j_code), jobj, g))
                mx_clear_jobj(&jobj, MX_SUCCESS);
            else
                mx_clear_jobj(&jobj, MX_FAILURE);
        }

        mx_clear_jobj(&jobj, MX_FAILURE);
    }

    mx_clear_jobj(&jobj, MX_FAILURE);
}
