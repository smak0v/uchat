#include "uchat.h"


static t_user *for_get_user(sqlite3_stmt *stmt) {
    t_user *user = NULL;
    int rv;
    
    if ((rv = sqlite3_step(stmt)) != SQLITE_ROW) {
        if (rv == SQLITE_ERROR)
			fprintf(stderr, "getuser\n");
        sqlite3_finalize(stmt);
        return NULL;
    }
    user = malloc(sizeof(t_user));
    user->user_id = sqlite3_column_int(stmt, 0);
    user->user_login = strdup((const char*)sqlite3_column_text(stmt, 1));
    user->user_pass = strdup((const char*)sqlite3_column_text(stmt, 2));
    sqlite3_finalize(stmt);
    return user;
}

t_user *mx_get_user_by_login(sqlite3 *db, char *user_login) {
    sqlite3_stmt *stmt;
    int rv = 0;

    sqlite3_prepare_v2(db, "SELECT * FROM USERS WHERE USER_LOGIN = ?1",
        -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, user_login, -1, SQLITE_STATIC);
    if (rv != SQLITE_OK)
		fprintf(stderr, "Can't get user from db: %s\n", sqlite3_errmsg(db));
    return for_get_user(stmt);
}

void mx_insert_user_in_db(sqlite3 *db, char *login, char *pass) {
	sqlite3_stmt *stmt;
	int rv = 0;

	rv = sqlite3_prepare_v2(db, 
		"INSERT INTO USER(LOGIN, PASSWORD)VALUES(?1, ?2);",
		-1, &stmt, NULL);
	if (rv == SQLITE_ERROR)
		fprintf(stderr, "Can't insert user into db: %s\n", sqlite3_errmsg(db));
	sqlite3_bind_text(stmt, 1, login, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, pass, -1, SQLITE_STATIC);
	if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
		fprintf(stderr, "Can't insert user into db: %s\n", sqlite3_errmsg(db));
	sqlite3_finalize(stmt);
}

t_user *mx_get_usertable(sqlite3 *db) {
	t_user *usr = NULL;
	int rv;
	sqlite3_stmt *stmt;

	sqlite3_prepare_v2(db, "SELECT * FROM USER", -1, &stmt, NULL);
	if ((rv = sqlite3_step(stmt)) != SQLITE_ROW) {
		if (rv == SQLITE_ERROR)
			fprintf(stderr, "Can't get usertable: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return NULL;
	}
	usr = malloc(sizeof(t_user));
	usr->user_id = sqlite3_column_int(stmt, 0);
	usr->user_login = strdup((const char *)sqlite3_column_text(stmt, 1));
	usr->user_pass = strdup((const char *)sqlite3_column_text(stmt, 2));
	sqlite3_finalize(stmt);
	return usr;
}

void mx_new_table_user(sqlite3 *database) {
	sqlite3_exec(database, "CREATE TABLE USER(" \
				 "USER_ID  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
				 "LOGIN    TEXT 							 NOT NULL," \
				 "PASSWORD TEXT								 NOT NULL);",
				  0, 0, 0);
}


sqlite3 *mx_opendb(char *name) {
	sqlite3 *db;
	int rv;

	if ((rv = sqlite3_open(name, &db)) != SQLITE_OK)
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	else
      fprintf(stderr, "Opened database successfully\n");
	
	// create table 
 	// mx_new_table_group_member(db); 
 	mx_new_table_user(db);
 	// mx_new_table_group(db); 
 	// mx_new_table_dialog(db);
	return db;
}

void mx_closedb(sqlite3 *db) {
	int val = sqlite3_close(db);

	if (val != SQLITE_OK) {
		fprintf(stderr, "Can't close database: %s\n", sqlite3_errmsg(db));
	}
	else 
		fprintf(stderr, "Closed database successfully\n");

}
