#include "uchat.h"

void mx_new_table_user(sqlite3 *database) {
	sqlite3_exec(database, "CREATE TABLE USER(" \
				 "USER_ID  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
				 "LOGIN    TEXT 							 NOT NULL," \
				 "PASSWORD TEXT								 NOT NULL);",
				  0, 0, 0);
}

// void mx_new_table_user(sqlite3 *database) {
// 	sqlite3_exec(database, "CREATE TABLE USER(" \
// 				 "USER_ID  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
// 				 "LOGIN    TEXT 							 NOT NULL," \
// 				 "PASSWORD TEXT								 NOT NULL);",
// 				  0, 0, 0);
// }
