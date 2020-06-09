#pragma once

#include "sqlite3.h"

typedef struct s_user {
	unsigned int user_id;
	const char *user_login;
	const char *user_pass;
}				t_user;

sqlite3 *mx_opendb(char *name); // open database.db
void mx_closedb(sqlite3 *db); // close database.db

void mx_new_table_user(sqlite3 *database); // create table USER

t_user *mx_get_usertable(sqlite3 *db); // returns struct user from db
void mx_insert_user_in_db(sqlite3 *db, char *login, char *pass); // add usr
t_user *mx_get_user_by_login(sqlite3 *db, char *user_login); // returns data from table
void mx_print_db(sqlite3 *db, char *table); // print TABLE
void mx_delete_user_by_login(sqlite3 *db, char *login); // del user by login
void mx_delete_user_by_id(sqlite3 *db, int user_id); // del user by user_id





