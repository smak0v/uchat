#include "uchat.h"

void mx_new_table_user(sqlite3 *database) {
	sqlite3_exec(database, "CREATE TABLE IF NOT EXISTS USER(" \
				 "USER_ID  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
				 "LOGIN    TEXT NOT NULL," \
				 "PASSWORD TEXT NOT NULL);",
				  0, 0, 0);
}

void mx_new_table_group_members(sqlite3 *database) {
	sqlite3_exec(database, "CREATE TABLE IF NOT EXISTS GROUP_MEMBERS(" \
				 "GROUP_MEMBERS_ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"\
				 "USER_ID INTEGER NOT NULL," \
				 "GROUP_ID INTEGER NOT NULL);",
				  0, 0, 0);
}

void mx_new_table_group(sqlite3 *database) {
	sqlite3_exec(database, "CREATE TABLE IF NOT EXISTS GRP(" \
				 "GROUP_ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"\
				 "GROUP_NAME TEXT NOT NULL);", 
				  0, 0, 0);
}

void mx_new_table_dialog(sqlite3 *database) {
	sqlite3_exec(database, "CREATE TABLE IF NOT EXIST DIALOG(" \
				 "DIALOG_ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"\
				 "USER_ID1 INTEGER NOT NULL," \
				 "USER_ID2 INTEGER NOT NULL);",
				  0, 0, 0);
}

