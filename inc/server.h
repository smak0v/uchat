#pragma once

// Includes
#include "uchat.h"
#include "sqlite3.h"

// Constants
#define MX_MAX_THREADS 50

// Structures
typedef struct s_communication t_comm;
typedef struct s_metadata t_meta;
typedef char *(*api_function)(void *, t_comm *);
typedef struct s_load_dialogues t_ld_d;

// DB
typedef struct s_user t_user;
typedef struct s_gr_members t_gr_members;
typedef struct s_all_gr_member t_all_gr_member;
typedef struct s_dialog t_dialog;
typedef struct s_dialog_sorted t_dialog_sorted;
typedef struct s_dialog_users t_dialog_users;
typedef struct s_msg t_msg;
typedef struct s_profile t_profile;

struct s_communication {
    int fd;
    char *status;
    char *ft_status;
	sqlite3 *db;
	SSL *ssl;
	SSL_CTX *ctx;
};

struct s_metadata {
    pthread_t *threads;
    char *status;
    char *ft_status;
	sqlite3 *db;
	SSL *ssl;
	SSL_CTX *ctx;
};

struct s_user {
	int user_id;
	const char *user_login;
	const char *user_pass;
};

struct s_gr_members {
	int gr_members_id;
	int user_id;
	int group_id;
	bool adm;
};

struct s_all_gr_member {
	int group_id;
	char *group_name;
};

struct s_dialog {
	int dialog_id;
	int user_id1;
	int user_id2;
};

struct s_dialog_sorted {
	int dialog_id;
	int user_id1;
	int user_id2;
	int time;
};

struct s_dialog_users {
	int dialog_id;
	char *username;
	int user_id;
};

struct s_load_dialogues {
	int *dialog_id;
	char **username;
	int *user_id;
};

struct s_msg {
	int id;
	int group_id;
	int dialog_id;
	int sender;
	int recepient;
	char *msg;
	int time;
	bool edited;
	bool read;
	char *file;
	char *forwarded;
	char *username;
};

struct s_profile {
	int user_id;
	char *name;
	char *birth;
	char *email;
	char *status;
	char *country;
};

// static char *add_to_group(sqlite3 *db, json_object *arr, int gr_id, int adm)
struct s_groups_auditor {
	sqlite3 *db;
	json_object *arr;
	char *birth;
	char *email;
	char *status;
	char *country;
};

// Functions
int mx_start_server(int port);
t_meta *mx_init_threads(sqlite3 *db, SSL_CTX *ctx);
void mx_thread_manager(int connection_fd, t_meta **metadata);
void *mx_communicate(void *data);
char *mx_process_request(char *request, t_comm *connect);
int mx_open_listener(int port);
void mx_daemonize(int port);

// Server API
char *mx_bad_request(void *jobj, t_comm *connect);
char *mx_register_user(void *jobj, t_comm *connect);
char *mx_sign_in(void *jobj, t_comm *connect);
char *mx_sign_out(void *jobj, t_comm *connect);
char *mx_new_group(void *jobj, t_comm *connect);
char *mx_add_to_group(void *jobj, t_comm *connect);
char *mx_rename_group(void *jobj, t_comm *connect);
char *mx_leave_group(void *jobj, t_comm *connect);
char *mx_del_group(void *jobj, t_comm *connect);
char *mx_send_message(void *jobj, t_comm *connect);
char *mx_edit_message(void *jobj, t_comm *connect);
char *mx_delete_message(void *jobj, t_comm *connect);
char *mx_load_dialogues(void *jobj, t_comm *connect);
char *mx_load_groups(void *jobj, t_comm *connect);
char *mx_load_messages(void *jobj, t_comm *connect);
char *mx_del_user(void *jobj, t_comm *connect);
char *mx_get_user(void *jobj, t_comm *connect);
char *mx_find_user(void *jobj, t_comm *connect);
char *mx_edit_profile(void *jobj, t_comm *connect);

// JSON builders
char *mx_json_string_msg(t_msg *msg);
char *mx_json_string_s_in(int uid, char *tok);
char *mx_json_string_add_to_gr(int gid);
char *mx_json_string_load_dlg(t_ld_d *arrays, int len);
char *mx_json_string_load_grp(t_ld_d *arrs, int len);
char *mx_msg_json_builder(t_msg *msg);
char *mx_json_string_notify_gr(int gid, char *gr_name, int type);
char *mx_json_string_code_type(int code, int type);
char *mx_add_type(char *j_str, int val);

void mx_fill_array_int(json_object *jobj, int *arr, int len);
void mx_fill_array_str(json_object *jobj, char **arr, int len);
void mx_fill_array_msg(json_object *jobj, t_list *msg_list);
void mx_fill_array_user(json_object *jobj, t_list *user_list);

json_object *mx_create_profile_object(t_profile *prof);
json_object *mx_json_builder_msg(t_msg *msg);

// Server Utils
t_msg *mx_extract_message(void *jobj);
int mx_extract_edit_msg(json_object *jobj, int *uid, int *mid, char **msg);
int mx_extract_delete_message(json_object *jobj, int *uid, int *mid);
unsigned char *mx_generate_token(void);
int mx_validate_token(sqlite3 *db, int id, void *v_jobj);
char *mx_hmac_sha_256(char *key, char *data);
int *mx_parse_sock_str(sqlite3 *db, int uid, int *len);
int mx_extract_name_passw(json_object *json, const char **name,
                              const char **passw);
json_object *mx_unpack_addtogroup(json_object *jobj, int *gid, int *uid);
void mx_send_to_all_clients(sqlite3 *db, char *j_str, int uid);
char *mx_file_transfer(t_comm *connect, char *file, char *res, int msg_id);
int mx_get_free_thread(char *status, int *counter);
char *mx_post_reg_sign_in(t_comm *connect, char *name, char *passw);

// Notifications
void mx_notify_add_to_group(sqlite3 *db, json_object *cli_arr, int gid);
void mx_notify_group_renamed(sqlite3 *db, int gid, char *name);

//Sockets
char *mx_add_socket(char *sock, int fd);
char *mx_remove_socket(sqlite3 *db, int fd, int uid);
int *mx_parse_sock_str(sqlite3 *db, int uid, int *len);

// Wrappers
int mx_j_o_o_a(json_object *jso, const char *key, json_object *val);
enum json_type mx_j_o_g_t(json_object *jso);

// DB API
sqlite3 *mx_opendb(char *name);
void mx_closedb(sqlite3 *db);

void mx_new_table_user(sqlite3 *database);
void mx_new_table_group_members(sqlite3 *database);
void mx_new_table_group(sqlite3 *database);
void mx_new_table_dialog(sqlite3 *database);
void mx_new_table_message(sqlite3 *database);
void mx_new_table_socket(sqlite3 *database);
void mx_new_table_profiles(sqlite3 *database);

// USER table
t_user *mx_get_user_by_login(sqlite3 *db, char *user_login);
t_user *mx_get_user_by_user_id(sqlite3 *db, int user_id);
int mx_add_user(sqlite3 *db, char *login, char *pass);
int mx_delete_user_by_login(sqlite3 *db, char *login);
int mx_delete_user_by_id(sqlite3 *db, int user_id);
int mx_get_user_id_by_login(sqlite3 *db, char *login);
int mx_change_user_pass(sqlite3 *db, int user_id, char *new_pass);
char *mx_get_user_login_by_id(sqlite3 *db, int user_id);
int mx_change_user_username(sqlite3 *db, int user_id, char *username);
t_list *mx_find_user_by_char(sqlite3 *db, char *str);

// GROUP_MEMBERS table
t_gr_members *mx_get_by_group_mem_id(sqlite3 *db, int gr_member_id);
t_gr_members *mx_get_by_user_id(sqlite3 *db, int user_id);
t_gr_members *mx_get_by_group_id(sqlite3 *db, int group_id);
int mx_add_group_member(sqlite3 *db, int user_id, int group_id, bool adm);
int mx_delete_user_from_group(sqlite3 *db, int user_id, int group_id);
int mx_check_group_member(sqlite3 *db, int user_id, int group_id);
t_list *mx_get_all_group_members(sqlite3 *db, int gid);
int mx_change_admin_status(sqlite3 *db, int user_id, int group_id, bool adm);
int *mx_get_all_id_group_members(sqlite3 *db, int group_id);
int mx_get_size_group_mem_by_group_id(sqlite3 *db, int group_id);
int *mx_get_all_user_groups_member(sqlite3 *db, int user_id);
int mx_check_group_member(sqlite3 *db, int user_id, int group_id);
t_list *mx_get_groups(sqlite3 *db, int user_id, int *len);

// GROUP table
int mx_add_grp(sqlite3 *db, char *group_name);
int mx_delete_grp_by_id(sqlite3 *db, int grp_id);
int mx_rename_grp_by_id(sqlite3 *db, int grp_id, char *new_name);
int mx_get_grp_id(sqlite3 *db, char *grp_name);
char *mx_get_group_name_by_id(sqlite3 *db, int group_id);
t_list *mx_get_all_user_groups(sqlite3 *db, int user_id);

// DIALOG table
int mx_add_dialog(sqlite3 *db, int user_id1, int user_id2);
int mx_delete_by_dialog_id(sqlite3 *db, int dialog_id);
t_dialog *mx_get_dialog_by_id1_id2(sqlite3 *db, int id1, int id2);
int mx_get_dialog_id(sqlite3 *db, int id1, int id2);
int mx_delete_dialog_by_id1_id2(sqlite3 *db, int user_id1, int user_id2);
int *mx_get_users_id_by_dialog_id(sqlite3 *db, int dialog_id);
int mx_get_time_dialog(sqlite3 *db, int dialog_id, int user_id);
t_list *mx_get_all_user_dialogs(sqlite3 *db, int user_id);
t_list *mx_get_dialog_users(sqlite3 *db, int usr_id, int *len);

// MSG table
int mx_add_msg(sqlite3 *db, t_msg *m);
t_msg *mx_get_msg_by_id(sqlite3 *db, int id);
int mx_delete_msg_by_id(sqlite3 *db, int id);
int mx_update_msg_by_id(sqlite3 *db, char *msg, int id);
int mx_get_msg(sqlite3 *db, t_msg *m);
t_list *mx_db_load_messages(sqlite3 *db, int group_id, int dialog_id, int n);
t_msg *mx_fill_msg(sqlite3_stmt *stmt);
t_list *mx_db_load_next_messages(sqlite3 *db, int group_id,
                                 int dialog_id, int n, int time);

//SOCKETS table
int mx_add_sock_user(sqlite3 *db, int user_id,
                     char *sock_fd, char *token);
char *mx_get_sock_by_user_id(sqlite3 *db, int user_id);
char *mx_get_token_by_user_id(sqlite3 *db, int user_id);
int mx_delete_sock_by_user_id(sqlite3 *db, int user_id);
int mx_update_socket_by_user_id(sqlite3 *db, char *socket, int user_id);
int mx_get_user_id_by_socket(sqlite3 *db, int sock);

//PROFILES table
int mx_add_profile(sqlite3 *db, t_profile *usr);
int mx_profile_exist(sqlite3 *db, int user_id);
int mx_delete_profile(sqlite3 *db, int user_id);
t_profile *mx_get_profile_by_id(sqlite3 *db, int user_id);
int mx_change_profile_by_id(sqlite3 *db, t_profile *profile, int user_id);

// Utils
void mx_print_db(sqlite3 *db, char *table);
void mx_print_list_members(t_list *mem);
int mx_get_size_table(sqlite3 *db, char *table);
void mx_delete_list(t_list *head);
void mx_recv_file(SSL *ssl, char *filename);

// TLS/SSL
void mx_load_certificates(SSL_CTX *ctx, char *cert_file, char *key_file);
SSL_CTX *mx_init_server_ctx(void);
void mx_show_client_certs(SSL *ssl);

