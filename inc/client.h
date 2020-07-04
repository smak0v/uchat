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

    // register window
    GtkWidget *w_reg; // window register
    GtkWidget *r_ename; // entry name
    GtkWidget *r_epass; // entry pass
    GtkWidget *r_repass; // entry pass
    GtkWidget *b_reg_login; // button register and login
    GtkWidget *b_reg_back; // button back to login window

    // chat window
    GtkWidget *w_chat;
    // GtkWidget *c_box; // window register
    // GtkWidget *c_label; // window register
    // GtkWidget *c_entry; // window register
    // GtkWidget *c_b_go; // window register
    // GtkWidget *c_main;
    GtkWidget *box1;
    GtkWidget *button1;
    GtkWidget *box2;
    GtkWidget *label1;
    GtkWidget *label2;
    GtkWidget *button2;
    GtkWidget *box3;
    GtkWidget *entry1;
    GtkWidget *scroll;
    GtkWidget *listbox;
    GtkWidget *box5;
    GtkWidget *box4;

    int n;
    // int message_num;
};


// Functions
void mx_init_client(int argc, char **argv);
void mx_start_client(char *ip, int port, t_glade *g);
void *mx_listen_server(void *data);
char *mx_read_server_response(t_glade *g);

// SSL/TLS
SSL_CTX *mx_init_client_ctx(void);
void mx_show_server_certs(SSL *ssl);

// Validators
int mx_validate_login_data(t_glade *g);
int mx_validate_signup_data(t_glade *g, char *repeat);

// Threads
void mx_client_thread_manager(t_glade *glade, SSL *ssl);

// Utils
void mx_create_error_modal_window(char *first, char *second, GtkWidget *win);
int mx_clear_jobj(json_object **jobj, int status);

// JOSN builders
char *mx_json_string_login_signup(enum e_types type, char *log, char *passw);

// JSON parsers
int mx_parse_login_response(char *response, t_glade *g);
int mx_parse_signup_response(char *response, t_glade *g);

// UI
char *mx_build_ui_path(char *filename);
int mx_connect_css(char *path);
void mx_open_regwin(GtkWidget *sender, t_glade *g);
void mx_create_win_log(t_glade *g);
void mx_widget_visible(GtkWidget *widget, gboolean is_visible);
void mx_widget_set_visibility(GtkBuilder *bd, gchar *name,
                              gboolean is_visible);
void mx_b_log(GtkButton *b, t_glade *g);
void mx_b_reg_log(GtkButton *b, t_glade *g);
void mx_create_win_chat(t_glade *g);
void mx_open_win_chat(GtkWidget *w, t_glade *g);
void mx_show_win_chat(GtkWidget *v, t_glade *g);
