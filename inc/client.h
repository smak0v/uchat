#pragma once

// Includes
#include <gtk/gtk.h>

#include "uchat.h"


// Constants
#define MX_UI_PATH "./src/client/ui/"
#define MX_MISTERY -666


// Structures
typedef struct s_glade t_glade;
typedef struct s_thread_data t_thread_data;
typedef struct s_msg t_msg;

struct s_thread_data {
    SSL *ssl;
    t_glade *glade;
};

struct s_msg {
    int dgid;
    bool group;
    int uid2;
    char *msg;
    char *file;
    char *fwrd;
    int time;
};

struct s_glade {
    GtkBuilder *bd;

    // user data
    char *log;
    char *pass;
    char *token;
    int uid;
    SSL *ssl;

    // message data
    int dgid;
    bool group;
    int uid2;

    // log in window
    GtkWidget *w_log; // window login
    GtkWidget *e_name; // entry name
    GtkWidget *e_pass; // entry pass
    GtkWidget *b_log_in; // button login
    GtkWidget *b_reg; // button register
    GtkWidget *b_eye; // eye button

    // register window
    GtkWidget *w_reg; // window register
    GtkWidget *r_ename; // entry name
    GtkWidget *r_epass; // entry pass
    GtkWidget *r_repass; // entry pass
    GtkWidget *b_reg_login; // button register and login
    GtkWidget *b_reg_back; // button back to login window
    GtkWidget *b_reye; // reg eye button

    // chat window
    GtkWidget *w_chat; // window chat
    GtkWidget *b_logout; // button logout
    GtkWidget *b_send_msg; // button send message
    GtkWidget *b_username; // button username
    GtkWidget *b_close_profile; // button close profile
    GtkWidget *e_search; // entry search
    GtkWidget *e_message; // entry message
    GtkWidget *b_attach_file; // button attach file
    GtkWidget *l_select_chat; // label select chat
    GtkWidget *b_add_chat; // button add chat
    GtkWidget *b_add_group; // button add group
    GtkWidget *groups_box; // box with groups
    GtkWidget *dialogs_box; // box with dialogs
    GtkWidget *gc_notebook; // left groups and chats notebook

    // profile window
    GtkWidget *b_save_profile; // button save profile

    // GUI blocks (chat)
    GtkWidget *common_area;
    GtkWidget *messages_area;
    GtkWidget *box_message;
    GtkWidget *profile_area;

    // dialogs
    GtkWidget *d_add_chat; // dialog for adding new chat
    GtkWidget *d_add_group; // dialog for adding new group

    // add chat dialog
    GtkWidget *b_add_chat_ok; // button add chat
    GtkWidget *b_add_chat_cancel; // button cancel add chat
    GtkWidget *e_chat_search; // entry chat search

    // add group dialog
    GtkWidget *b_add_group_ok; // button add group
    GtkWidget *b_add_group_cancel; // button cancel add group
    GtkWidget *e_new_group_name; // entry group search
    GtkWidget *err_group_name_label; // label error group name
};


// Functions
// SSL/TLS
SSL_CTX *mx_init_client_ctx(void);
void mx_show_server_certs(SSL *ssl);

// Validators
int mx_validate_login_data(t_glade *g);
int mx_validate_signup_data(t_glade *g, char *repeat);

// Threads
void mx_client_thread_manager(t_glade *glade, SSL *ssl);

// Utils
void mx_start_client(char *ip, int port, t_glade *g);
void *mx_listen_server(void *data);
char *mx_read_server_response(t_glade *g);
int mx_clear_jobj(json_object **jobj, int status);
GtkWidget *mx_get_gtk_obj(t_glade *g, char *name);
void mx_logout(t_glade *g);

// JOSN builders
char *mx_json_string_login_signup(enum e_types type, char *log, char *passw);
char *mx_json_string_logout(char *token, int uid);
char *mx_json_string_new_group(char *token, int uid, char *group_name);
char *mx_json_string_load_groups(char *token, int uid);
char *mx_json_string_send_message(t_glade *g, t_msg *msg);
char *mx_json_string_load_messages(t_glade *g, int time, int dgid, bool group);

// JSON parsers
int mx_parse_login_response(char *response, t_glade *g);
int mx_parse_signup_response(char *response, t_glade *g);
void mx_parse_logout_response(char *response, t_glade *g);
int mx_parse_new_group_response(char *response, t_glade *g);
void mx_parse_load_groups_response(char *response, t_glade *g);
void mx_parse_load_messages_response(char *response, t_glade *g);
int mx_parse_send_message_response(char *response);

// GUI
void mx_create_error_modal_window(char *first, char *second, GtkWidget *win);
void mx_clear_login_inputs(t_glade *g);
void mx_clear_signup_inputs(t_glade *g);

char *mx_build_ui_path(char *filename);
int mx_connect_css(char *path);
void mx_gtk_quit(GtkWidget *w, t_glade *g);
void mx_delete_childs(GtkWidget *w);

void mx_create_win_log(t_glade *g);
void mx_create_win_reg(t_glade *g);
void mx_create_win_chat(t_glade *g);

void mx_widget_visible(GtkWidget *widget, gboolean is_visible);
void mx_widget_set_visibility(GtkBuilder *bd, gchar *name,
                              gboolean is_visible);
void mx_entry_visibility(GtkButton *b, GtkWidget *entry);

void mx_b_log(GtkButton *b, t_glade *g);
void mx_b_reg_log(GtkButton *b, t_glade *g);
void mx_b_logout(GtkButton *b, t_glade *g);

void mx_open_logwin(GtkWidget *sender, t_glade *g);
void mx_open_regwin(GtkWidget *sender, t_glade *g);
void mx_show_win_chat(GtkWidget *v, t_glade *g);

void mx_open_profile(GtkWidget *w, t_glade *g);
void mx_close_profile(GtkWidget *w, t_glade *g);
void mx_save_profile(GtkWidget *w, t_glade *g);

void mx_add_chat(GtkWidget *w, t_glade *g);
void mx_add_group(GtkWidget *w, t_glade *g);

void mx_send_msg(GtkWidget *w, t_glade *g);

void mx_load_groups(t_glade *g);

void mx_load_messages(t_glade *g, time_t timestamp);

gboolean mx_close_chat(GtkWidget *w, GdkEventKey *e, t_glade *g);
