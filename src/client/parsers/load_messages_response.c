#include "client.h"

static void buld_msg_block(GtkWidget *msg_v_box, json_object *msg,
    char *time) {
    GtkWidget *u_t_h_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *msg_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *l_msg = gtk_label_new(json_object_get_string(
        json_object_object_get(msg, "msg")));
    GtkWidget *l_username = gtk_label_new(json_object_get_string(
        json_object_object_get(msg, "nme")));
    GtkWidget *l_time = gtk_label_new(time);

    gtk_label_set_xalign(GTK_LABEL(l_username), 0);
    gtk_label_set_xalign(GTK_LABEL(l_time), 1);
    gtk_label_set_xalign(GTK_LABEL(l_msg), 0);

    gtk_box_pack_start(GTK_BOX(msg_v_box), u_t_h_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(msg_v_box), msg_vbox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(u_t_h_box), l_username, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(u_t_h_box), l_time, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(msg_vbox), l_msg, FALSE, FALSE, 0);

    mx_set_message_style(msg_v_box, l_username, l_time, l_msg);
}

static void add_message_to_gui(json_object *msg, t_glade *g) {
    char *time = mx_get_time((time_t)mx_atoi(json_object_get_string(
        json_object_object_get(msg, "time"))));
    GtkWidget *msg_v_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    buld_msg_block(msg_v_box, msg, time);
    gtk_box_pack_start(GTK_BOX(g->messages_area), msg_v_box, FALSE, FALSE, 0);

    if (mx_atoi(json_object_get_string(json_object_object_get(msg, "uid")))
        == g->uid)
        gtk_widget_set_halign(GTK_WIDGET(msg_v_box), GTK_ALIGN_END);
    else
        gtk_widget_set_halign(GTK_WIDGET(msg_v_box), GTK_ALIGN_START);

    mx_strdel(&time);
}

static int check_response_code(int code, json_object *jobj, t_glade *g) {
    json_object *j_msgs = NULL;
    int len = 0;

    if (code == 400 || code == 401 || code == 500)
        return MX_FAILURE;
    else {
        j_msgs = json_object_object_get(jobj, "msg");
        len = json_object_array_length(j_msgs);
        if (!len)
            g->dgid = -1;
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
