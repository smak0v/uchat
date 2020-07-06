#pragma once

// Includes
#include <gtk/gtk.h>

#include "uchat.h"


// Constants
#define MX_UI_PATH "./src/client/ui/"


// Structures
typedef struct s_glade t_glade;
typedef struct s_thread_data t_thread_data;

struct s_thread_data {
    SSL *ssl;
    t_glade *glade;
};

struct s_glade {
    GtkBuilder *bd;

    // user data
    char *log;
    char *pass;
    char *token;
    int uid;
    SSL *ssl;

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
    GtkWidget *i_search; // image search
    GtkWidget *b_attach_file; // button attach file
    GtkWidget *l_select_chat; // label select chat
    GtkWidget *b_add_chat; // button add chat
    GtkWidget *b_add_group; // button add group

    // profile window
    GtkWidget *b_save_profile; // button save profile

    // GUI blocks (chat)
    GtkWidget *common_area;
    GtkWidget *messages_area;
    GtkWidget *box_message;
    GtkWidget *profile_area;
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

// JOSN builders
char *mx_json_string_login_signup(enum e_types type, char *log, char *passw);
char *mx_json_string_logout(char *token, int uid);

// JSON parsers
int mx_parse_login_response(char *response, t_glade *g);
int mx_parse_signup_response(char *response, t_glade *g);
void mx_parse_logout_response(char *response, t_glade *g);

// GUI
void mx_create_error_modal_window(char *first, char *second, GtkWidget *win);
void mx_clear_login_inputs(t_glade *g);
void mx_clear_signup_inputs(t_glade *g);

char *mx_build_ui_path(char *filename);
int mx_connect_css(char *path);
void mx_gtk_quit(void);

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
