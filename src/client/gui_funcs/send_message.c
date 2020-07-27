#include "client.h"

static void buld_msg_block(GtkWidget *msg_v_box, json_object *msg, char *time,
    t_glade *g) {
    GtkWidget *l_msg = gtk_label_new(json_object_get_string(
        json_object_object_get(msg, "msg")));
    GtkWidget *l_username = gtk_label_new(json_object_get_string(
        json_object_object_get(msg, "nme")));
    GtkWidget *l_time = gtk_label_new(time);
    GtkWidget *u_t_h_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *msg_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    gtk_label_set_xalign(GTK_LABEL(l_username), 0);
    gtk_label_set_xalign(GTK_LABEL(l_time), 1);
    gtk_label_set_xalign(GTK_LABEL(l_msg), 0);
    gtk_box_pack_start(GTK_BOX(msg_v_box), u_t_h_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(msg_v_box), msg_vbox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(u_t_h_box), l_username, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(u_t_h_box), l_time, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(msg_vbox), l_msg, FALSE, FALSE, 0);
    mx_append_file_to_msg_block(msg_v_box, msg, g, l_msg);

    mx_set_message_style(msg_v_box, l_username, l_time, l_msg);
    mx_add_id_to_msg_block(msg, msg_vbox);
}

static t_msg *build_msg(t_glade *g, char *user_message) {
    t_msg *msg = mx_memalloc(sizeof(t_msg));

    msg->group = g->group;
    msg->dgid = g->dgid;
    msg->uid2 = g->uid2;
    msg->msg = user_message;
    msg->time = time(NULL);
    msg->file = g->filename;

    return msg;
}

static bool check_did_gid(t_glade *g, json_object *j_msg) {
    json_object *j_gid = NULL;
    json_object *j_did = NULL;

    json_object_object_get_ex(j_msg, "gid", &j_gid);
    json_object_object_get_ex(j_msg, "did", &j_did);

    if (g->group && g->dgid == json_object_get_int(j_gid))
        return true;
    else if (!g->group && g->dgid == json_object_get_int(j_did))
        return true;
    else
        return false;
}

void mx_add_message_to_gui(t_glade *g, char *response) {
    json_object *jobj = json_tokener_parse(response);
    json_object *j_msg = json_object_object_get(jobj, "msg");
    char *time = NULL;
    GtkWidget *msg_v_box = NULL;

    if (check_did_gid(g, j_msg)) {
        time = mx_get_time(json_object_get_int(json_object_object_get(j_msg,
        "time")));
        msg_v_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        buld_msg_block(msg_v_box, j_msg, time, g);
        gtk_box_pack_start(GTK_BOX(g->messages_area), msg_v_box, FALSE, FALSE, 0);

        if (mx_atoi(json_object_get_string(json_object_object_get(j_msg, "uid2")))
            == g->uid)
            gtk_widget_set_halign(GTK_WIDGET(msg_v_box), GTK_ALIGN_START);
        else
            gtk_widget_set_halign(GTK_WIDGET(msg_v_box), GTK_ALIGN_END);
        mx_strdel(&time);
        mx_clear_jobj(&jobj, MX_SUCCESS);
    }
}

void mx_send_msg(GtkWidget *w, t_glade *g) {
    char *user_message = mx_get_input_text(g);
    char *request = NULL;
    t_msg *msg = build_msg(g, user_message);

    request = mx_json_string_send_message(g, msg);
    SSL_write(g->ssl, request, strlen(request));

    mx_clear_input_text(g);
    free(msg);
    msg = NULL;
    mx_strdel(&user_message);
    mx_strdel(&request);
    mx_strdel(&g->filename);

    (void)w;
}
