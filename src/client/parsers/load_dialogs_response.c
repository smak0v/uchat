#include "client.h"

static void open_dialogue(GtkWindow *event_box, GdkEvent *e, t_glade *g) {
    GtkWidget *group_box = gtk_bin_get_child(GTK_BIN(event_box));
    GList *childs = gtk_container_get_children(GTK_CONTAINER(group_box));
    GtkWidget *l_did = GTK_WIDGET(g_list_nth_data(childs, 1));

    mx_delete_childs(g->messages_area);
    mx_clear_input_text(g);
    gtk_label_set_text(GTK_LABEL(g->l_chat_name),
        gtk_label_get_text(GTK_LABEL(g_list_nth_data(childs, 2))));
    gdk_threads_add_idle(mx_show_widget, g->messages_area);
    gdk_threads_add_idle(mx_show_widget, g->box_message);
    gdk_threads_add_idle(mx_show_widget, g->e_search);
    gdk_threads_add_idle(mx_show_widget, g->box5);
    gdk_threads_add_idle(mx_hide_widget, g->l_select_chat);
    gdk_threads_add_idle(mx_hide_widget, g->profile_area);
    g->group = false;
    g->dgid = mx_atoi((char *)gtk_label_get_text(GTK_LABEL(l_did)));
    mx_load_messages_request(g, time(NULL));
    g_list_free(childs);
    childs = NULL;
    (void)e;
}

static void add_dialogue_to_gui(t_glade *g, int did, int uid2, char *name) {
    GtkWidget *event_box = gtk_event_box_new();
    GtkWidget *dialogue_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *l_name = gtk_label_new(name);
    char *str_did = mx_itoa(did);
    char *str_uid2 = mx_itoa(uid2);
    GtkWidget *l_did = gtk_label_new(str_did);
    GtkWidget *l_uid2 = gtk_label_new(str_uid2);

    mx_strdel(&str_did);
    mx_strdel(&str_uid2);

    gtk_box_pack_start(GTK_BOX(g->dialogs_box), event_box, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(event_box), dialogue_box);
    gtk_box_pack_end(GTK_BOX(dialogue_box), l_name, TRUE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(dialogue_box), l_did, TRUE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(dialogue_box), l_uid2, TRUE, TRUE, 0);
    gtk_style_context_add_class(gtk_widget_get_style_context(dialogue_box),
        "chat_group_box");
    g_signal_connect(event_box, "button_press_event",
        G_CALLBACK(open_dialogue), g);
    gtk_widget_realize(event_box);
    gtk_widget_add_events(event_box, GDK_BUTTON_PRESS_MASK);
    gdk_threads_add_idle(mx_show_all_widget, event_box);
    gdk_threads_add_idle(mx_hide_widget, l_did);
    gdk_threads_add_idle(mx_hide_widget, l_uid2);
}

static void parse_arrays(t_glade *g,  int len, json_object *jobj) {
    json_object *j_names = json_object_object_get(jobj, "names");
    json_object *j_dids = json_object_object_get(jobj, "did");
    json_object *j_uids = json_object_object_get(jobj, "uid");
    t_dialogue *dialogue = NULL;

    for (int i = 0; i < len; ++i) {
        dialogue = mx_memalloc(sizeof(dialogue));
        dialogue->did = json_object_get_int(
            json_object_array_get_idx(j_dids, i));
        dialogue->uid2 = json_object_get_int(
            json_object_array_get_idx(j_uids, i));

        mx_push_back(&g->dialogues, dialogue);

        add_dialogue_to_gui(g, json_object_get_int(
            json_object_array_get_idx(j_dids, i)),
            json_object_get_int(json_object_array_get_idx(j_uids, i)),
            (char *)json_object_get_string(
                json_object_array_get_idx(j_names, i)));
    }
}

static int check_response_code(int code, json_object *jobj, t_glade *g) {
    json_object *j_len = NULL;
    int len = 0;

    if (code == 400 || code == 401 || code == 500)
        return MX_FAILURE;
    else {
        json_object_object_get_ex(jobj, "len", &j_len);
        if (j_len && json_object_get_type(j_len) == json_type_int
            && (len = json_object_get_int(j_len)) > 0) {
            parse_arrays(g, len, jobj);
        }
        return MX_SUCCESS;
    }
}

void mx_parse_load_dialogs_response(char *response, t_glade *g) {
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
