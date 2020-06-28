#pragma once

// Includes
#include <gtk/gtk.h>

// Constants
#define MX_UI_PATH "./src/client/ui/"


// Structures
typedef struct s_glade t_glade;

// Functions
int mx_init_client(int argc, char **argv);

// UI
char *mx_build_ui_path(char *filename);
int mx_connect_css(char *path); // connect .css
void mx_open_regwin(GtkWidget *sender, t_glade *g);
void mx_create_win_log(t_glade *g); // entry window
void mx_widget_visible(GtkWidget *widget, gboolean is_visible); // TRUE - show widget, FALSE - hide
void mx_widget_set_visibility(GtkBuilder *bd, gchar *name, 
                              gboolean is_visible); // set visible by name
void mx_b_log(GtkButton *b, t_glade *g); // when click button -> in log window
void mx_b_reg_log(GtkButton *b, t_glade *g); // when click button -> in register window
void mx_create_win_chat(t_glade *g);
void mx_open_win_chat(GtkWidget *w, t_glade *g);
void mx_show_win_chat(GtkWidget *v, t_glade *g);

struct s_glade {
    GtkBuilder *bd; // builder
    // user data
    char *log;
    char *pass;

    // log in window
    GtkWidget *w_log; // window log
    GtkWidget *e_name; // entry name
    GtkWidget *e_pass; // entry pass
    GtkWidget *b_log_in; // button log in 
    GtkWidget *b_reg; // button register
    // register window
    GtkWidget *w_reg; // window register
    GtkWidget *r_ename; // entry name
    GtkWidget *r_epass; // entry pass
    GtkWidget *r_repass; // entry pass
    GtkWidget *b_reg_login; // button log in
    GtkWidget *b_reg_back; // button back to log in window
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
