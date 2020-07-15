#include "client.h"

static void add_message_to_gui(json_object *msg, t_glade *g) {
    json_object *j_msg_text = json_object_object_get(msg, "msg");
    json_object *j_time = json_object_object_get(msg, "time");
    GtkWidget *l_msg = gtk_label_new(json_object_get_string(j_msg_text));
    GtkWidget *l_username = gtk_label_new("username");
    char buff[20];
    time_t time = (time_t)mx_atoi(json_object_get_string(j_time));
    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&time));
    GtkWidget *l_time = gtk_label_new(buff);
    GtkWidget *msg_v_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *u_t_h_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *msg_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    gtk_label_set_xalign(GTK_LABEL(l_username), 0.0);
    gtk_label_set_xalign(GTK_LABEL(l_time), 1);
    gtk_label_set_xalign(GTK_LABEL(l_msg), 0.0);
    gtk_box_pack_end(GTK_BOX(g->messages_area), msg_v_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(msg_v_box), u_t_h_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(msg_v_box), msg_vbox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(u_t_h_box), l_username, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(u_t_h_box), l_time, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(msg_vbox), l_msg, FALSE, FALSE, 0);

    if (mx_atoi(json_object_get_string(json_object_object_get(msg, "uid2"))) != -1)
        gtk_widget_set_halign(GTK_WIDGET(msg_v_box), GTK_ALIGN_END);
    else
        gtk_widget_set_halign(GTK_WIDGET(msg_v_box), GTK_ALIGN_START);

    gtk_style_context_add_class(gtk_widget_get_style_context(msg_v_box), "msg");
    gtk_style_context_add_class(gtk_widget_get_style_context(l_username), "username");
    gtk_style_context_add_class(gtk_widget_get_style_context(l_time), "time");
    gtk_style_context_add_class(gtk_widget_get_style_context(l_msg), "msgtxt");

    gtk_widget_show_all(msg_v_box);
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
            add_message_to_gui(json_object_array_get_idx(j_msgs, i), g);
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
