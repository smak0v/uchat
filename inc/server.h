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


sqlite3 *mx_opendb(char *name); // open database.db
void mx_closedb(sqlite3 *db); // close database.db

void mx_new_table_user(sqlite3 *database); // create table USER
void mx_new_table_group_members(sqlite3 *database); // create table GROUP_MEMBERS
void mx_new_table_group(sqlite3 *database); // create table GROUP
void mx_new_table_dialog(sqlite3 *database); // create table DIALOG

// USER table
// t_user *mx_get_usertable(sqlite3 *db); // returns struct user from db
t_user *mx_get_user_by_login(sqlite3 *db, char *user_login); // returns data by user_Login from user table
t_user *mx_get_user_by_user_id(sqlite3 *db, int user_id); // returns data by user_id from user table
void mx_add_user(sqlite3 *db, char *login, char *pass); // add usr in db
void mx_delete_user_by_login(sqlite3 *db, char *login); // del user by login
void mx_delete_user_by_id(sqlite3 *db, int user_id); // del user by user_id

// GROUP_MEMBERS table 
t_gr_members *mx_get_by_group_mem_id(sqlite3 *db, int gr_member_id); // returns t_gr_members data from group members table
void mx_add_group_member(sqlite3 *db, int user_id, int group_id); // add group member in db
void mx_delete_by_gr_mem_id(sqlite3 *db, int group_members_id); // del group member by group_members_id
void mx_delete_by_gr_id(sqlite3 *db, int group_id); // del group member by group_id
void mx_delete_by_user_id(sqlite3 *db, int user_id); // del group member by user_id;
void mx_update_gr_members(sqlite3 *db, int group_member_id,
							int user_id, int group_id);


//GROUP table

// DIALOG table
t_dialog *mx_get_by_dialog_id(sqlite3 *db, int dialog_id); // returns t_dialog data from dialog table
void mx_add_dialog(sqlite3 *db, int user_id1, int user_id2); // add to DIALOG Ω
void mx_delete_by_dialog_id(sqlite3 *db, int dialog_id); // delete from DIALOG table by dialog_id
void mx_update_by_dialog_id(sqlite3 *db, int dialog_id, 
							int user_id1, int user_id2); // update by dialog_id user1 user2; 


// UTILS
void mx_print_db(sqlite3 *db, char *table); // print TABLE NAME 




