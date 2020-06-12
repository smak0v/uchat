#pragma once

#include "sqlite3.h"

typedef struct s_user {
	int user_id;
	const char *user_login;
	const char *user_pass;
} t_user;

typedef struct s_gr_members {
	int gr_members_id;
	int user_id;
	int group_id;
} t_gr_members;

typedef struct s_dialog {
	int dialog_id;
	int user_id1;
	int user_id2;
} t_dialog;

typedef struct s_msg {
	int id;
	int group_id;
	int sender;
	char *msg;
	int time;
	bool edited;
	bool read;
} t_msg;



sqlite3 *mx_opendb(char *name); // open database.db
void mx_closedb(sqlite3 *db); // close database.db

void mx_new_table_user(sqlite3 *database); // create table USER
void mx_new_table_group_members(sqlite3 *database); // create table GROUP_MEMBERS
void mx_new_table_group(sqlite3 *database); // create table GROUP
void mx_new_table_dialog(sqlite3 *database); // create table DIALOG
void mx_new_table_message(sqlite3 *database); // create table MSG (message history)

// USER table +
t_user *mx_get_user_by_login(sqlite3 *db, char *user_login); // returns data by user_Login from user table
t_user *mx_get_user_by_user_id(sqlite3 *db, int user_id); // returns data by user_id from user table
void mx_add_user(sqlite3 *db, char *login, char *pass); // add usr in db
void mx_delete_user_by_login(sqlite3 *db, char *login); // del user by login
void mx_delete_user_by_id(sqlite3 *db, int user_id); // del user by user_id
int mx_get_user_id_by_login(sqlite3 *db, char *login); // returns user_id by login
void mx_change_user_pass(sqlite3 *db, int user_id, char *new_pass); // new: change pass by usr_id

// GROUP_MEMBERS table 
t_gr_members *mx_get_by_group_mem_id(sqlite3 *db, int gr_member_id); // returns t_gr_members data from group members table
void mx_add_group_member(sqlite3 *db, int user_id, int group_id); // add group member in db
void mx_delete_user_from_group(sqlite3 *db, int user_id, int group_id); // del user from GROUP_MEMBERS and USER table; +

//GROUP table
void mx_add_grp(sqlite3 *db, char *group_name); // add to GRP table
void mx_delete_grp_by_id(sqlite3 *db, int grp_id); // delete from GRP table 
void mx_rename_grp_by_name(sqlite3 *db, char *name, char *new_name); // rename GROUP_NAME by GROUP_NAME
int mx_get_grp_id(sqlite3 *db, char *grp_name); // returns GROUP_ID by GROUP_NAME

// DIALOG table
void mx_add_dialog(sqlite3 *db, int user_id1, int user_id2); // add to DIALOG Ω
void mx_delete_by_dialog_id(sqlite3 *db, int dialog_id); // delete from DIALOG table by dialog_id
t_dialog *mx_get_dialog_by_id1_id2(sqlite3 *db, int id1, int id2); // returns USER_1 USER_2 DIALOG_ID

// MSG table
void mx_add_msg(sqlite3 *db, t_msg *m); // need to edit!!!
t_msg *mx_get_msg_by_id(sqlite3 *db, int id); // returns msg by ID 
void mx_delete_msg_by_id(sqlite3 *db, int id);
void mx_update_msg_by_id(sqlite3 *db, t_msg *m, int id);

// UTILS
void mx_print_db(sqlite3 *db, char *table); // print TABLE NAME 

// DELETED funcs 
// t_dialog *mx_get_by_dialog_id(sqlite3 *db, int dialog_id); // returns t_dialog data from dialog table
// void mx_update_by_dialog_id(sqlite3 *db, int dialog_id, 
							// int user_id1, int user_id2); // update by dialog_id user1 user2; 
