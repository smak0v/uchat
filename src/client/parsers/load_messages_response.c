#include "client.h"

static void add_message_to_gui(json_object *msg, t_glade *g, int i) {
    json_object *j_msg_text = json_object_object_get(msg, "msg");
    GtkWidget *event_box = gtk_event_box_new();
    GtkWidget *msg_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *l_msg = gtk_label_new(json_object_get_string(j_msg_text));

    gtk_grid_attach(GTK_GRID(g->messages_area), event_box, 1, i, 1, 1);
    gtk_container_add(GTK_CONTAINER(event_box), msg_box);
    gtk_label_set_width_chars(GTK_LABEL(l_msg), 30);
    gtk_label_set_line_wrap(GTK_LABEL(l_msg), TRUE);
    gtk_box_pack_end(GTK_BOX(msg_box), l_msg, FALSE, FALSE, 0);

    gtk_style_context_add_class(gtk_widget_get_style_context(msg_box), "msg");

    gtk_widget_realize(event_box);
    gtk_widget_add_events(event_box, GDK_BUTTON_PRESS_MASK);
    gtk_widget_show_all(event_box);
}

static int check_response_code(int code, json_object *jobj, t_glade *g) {
    json_object *j_msgs = NULL;
    int len = 0;

    if (code == 400 || code == 401 || code == 500)
        return MX_FAILURE;
    else {
        j_msgs = json_object_object_get(jobj, "msg");
        len = json_object_array_length(j_msgs);
        for (int i = 0; i < len; ++i)
            add_message_to_gui(json_object_array_get_idx(j_msgs, i), g, i);
        gtk_widget_set_vexpand(g->messages_area, TRUE);
        return MX_SUCCESS;
    }
}

void mx_parse_load_messages_response(char *response, t_glade *g) {
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
