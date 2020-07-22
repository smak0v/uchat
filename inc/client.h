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
typedef struct s_profile t_profile;
typedef struct s_dialogue t_dialogue;

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

struct s_profile {
    char *name;
    char *dob;
    char *email;
    char *status;
    char *country;
};

struct s_dialogue {
    int did;
    int uid2;
};

struct s_glade {
    GtkBuilder *bd;

    // connection data
    char *ip;
    SSL_CTX *ctx;

    // user data
    char *log;
    char *pass;
    char *token;
    int uid;
    SSL *ssl;
    t_list *dialogues;

    // message data
    int dgid;
    bool group;
    int uid2;

    // additional data
    char *filename;
    pthread_t listener;

    // window
    GtkWidget *window; //main window

    // log in window
    GtkWidget *w_log; // window login
    GtkWidget *e_name; // entry name
    GtkWidget *e_pass; // entry pass
    GtkWidget *b_log_in; // button login
    GtkWidget *b_reg; // button register
    GtkWidget *b_eye; // eye button
    GtkWidget *l_login_error; // label login error

    // register window
    GtkWidget *w_reg; // window register
    GtkWidget *r_ename; // entry name
    GtkWidget *r_epass; // entry pass
    GtkWidget *r_repass; // entry pass
    GtkWidget *b_reg_login; // button register and login
    GtkWidget *b_reg_back; // button back to login window
    GtkWidget *b_reye; // reg eye button
    GtkWidget *l_signup_error; // label signup error

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
    GtkWidget *l_chat_name; // label chat name
    GtkWidget *scrolledwindow1; // scrolled window with messages
    GtkWidget *box5; // box with chat name and settings button

    // profile window
    GtkWidget *b_save_profile; // button save profile
    GtkWidget *e_country; // entry country
    GtkWidget *e_birthday; // entry birthday
    GtkWidget *e_email; // entry email
    GtkWidget *e_profile_name; // entry profile name
    GtkWidget *e_status; // entry status

    // GUI blocks (chat)
    GtkWidget *common_area;
    GtkWidget *messages_area;
    GtkWidget *box_message;
    GtkWidget *profile_area;

    // dialogs
    GtkWidget *d_add_chat; // dialog for adding new chat
    GtkWidget *d_add_group; // dialog for adding new group

    // add chat dialog
    GtkWidget *b_add_chat_cancel; // button cancel add chat
    GtkWidget *e_chat_search; // entry chat search
    GtkWidget *box8; // box with search dialog results

    // add group dialog
    GtkWidget *b_add_group_ok; // button add group
    GtkWidget *b_add_group_cancel; // button cancel add group
    GtkWidget *e_new_group_name; // entry group search
    GtkWidget *err_group_name_label; // label error group name
};


// Functions
int mx_open_connection(char *ip, int port);

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
void mx_clear_input_text(t_glade *g);
char *mx_get_input_text(t_glade *g);
void mx_scroll_to_bottom(GtkWidget *w,  GdkRectangle *a, t_glade *g);
void *mx_send_file(void *data);
char *mx_get_time(time_t time);
void mx_process_send_file(t_glade *g, char *path, int port);
gboolean mx_hide_widget(gpointer w);
gboolean mx_show_widget(gpointer w);
gboolean mx_show_all_widget(gpointer w);
gboolean mx_destroy_widget(gpointer w);

// JSON builders
char *mx_json_string_login_signup(enum e_types type, char *log, char *passw);
char *mx_json_string_logout(char *token, int uid);
char *mx_json_string_new_group(char *token, int uid, char *group_name);
char *mx_json_string_load_dialogs_groups(enum e_types type, char *token,
    int uid);
char *mx_json_string_send_message(t_glade *g, t_msg *msg);
char *mx_json_string_load_messages(t_glade *g, int time, int dgid, bool group);
char *mx_json_string_get_profile(char *token, int uid, char *name);
char *mx_json_string_search_user(char *token, int uid, char *name);
char *mx_json_string_edit_profile(t_glade *g, t_profile *profile);
char *mx_json_string_s_file(int id, int num, char *buff, int buf_size);

// JSON parsers
int mx_parse_login_response(char *response, t_glade *g);
int mx_parse_signup_response(char *response, t_glade *g);
void mx_parse_logout_response(char *response, t_glade *g);
void mx_parse_load_dialogs_response(char *response, t_glade *g);
int mx_parse_new_group_response(char *response, t_glade *g);
void mx_parse_load_groups_response(char *response, t_glade *g);
void mx_parse_load_messages_response(char *response, t_glade *g);
int mx_parse_send_message_response(char *response);
void mx_parse_get_profile_response(char *response, t_glade *g);
void mx_parse_serach_user_response(char *response, t_glade *g);

// Processors
void mx_check_response_type(char *response, t_glade *g);
void mx_s_in_s_up(char *response, t_glade *g, int type);
void mx_s_out(char *response, t_glade *g);
void mx_load_dialogues(char *response, t_glade *g);
void mx_load_groups(char *response, t_glade *g);
void mx_find_user(char *response, t_glade *g);
void mx_n_grp(char *response, t_glade *g);
void mx_load_messages(char *response, t_glade *g);
void mx_edit_profile(char *response, t_glade *g);
void mx_search_users(char *response, t_glade *g);
void mx_s_msg(char *response, t_glade *g);
void mx_cli_file_transfer(char *response, t_glade *g);

// GUI
void mx_clear_login_inputs(t_glade *g);
void mx_clear_signup_inputs(t_glade *g);

char *mx_build_ui_path(char *filename);
int mx_connect_css(char *path);
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
void mx_login(t_glade *g);

void mx_open_logwin(GtkWidget *sender, t_glade *g);
void mx_open_regwin(GtkWidget *sender, t_glade *g);
void mx_show_win_chat(t_glade *g);

void mx_open_profile(GtkWidget *w, t_glade *g);
void mx_close_profile(GtkWidget *w, t_glade *g);
void mx_save_profile(GtkWidget *w, t_glade *g);
void mx_get_profile(t_glade *g);

void mx_add_chat(GtkWidget *w, t_glade *g);
void mx_add_group(GtkWidget *w, t_glade *g);

void mx_send_msg(GtkWidget *w, t_glade *g);
void mx_add_message_to_gui(t_glade *g, char *response);
void mx_attach_file(GtkWidget *w, t_glade *g);

void mx_load_dialogues_request(t_glade *g);
void mx_load_groups_request(t_glade *g);

void mx_load_messages_request(t_glade *g, time_t timestamp);
void mx_set_message_style(GtkWidget *msg_v_box, GtkWidget *l_username,
    GtkWidget *l_time, GtkWidget *l_msg);

gboolean mx_close_chat(GtkWidget *w, GdkEventKey *e, t_glade *g);

void mx_open_dialog(GtkWidget *w, t_glade *g);

void mx_find_gtk_objects_1(t_glade *g);
void mx_find_gtk_objects_2(t_glade *g);

void mx_gtk_quit(t_glade *g);
