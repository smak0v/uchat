#include "uchat.h"


// t_user *mx_get_usertable(sqlite3 *db) {
// 	t_user *usr = NULL;
// 	int rv;
// 	sqlite3_stmt *stmt;

// 	sqlite3_prepare_v2(db, "SELECT * FROM USER", -1, &stmt, NULL);
// 	if ((rv = sqlite3_step(stmt)) != SQLITE_ROW) {
// 		if (rv == SQLITE_ERROR)
// 			fprintf(stderr, "Can't get usertable: %s\n", sqlite3_errmsg(db));
// 		sqlite3_finalize(stmt);
// 		return NULL;
// 	}
// 	usr = malloc(sizeof(t_user));
// 	usr->user_id = sqlite3_column_int(stmt, 0);
// 	usr->user_login = strdup((const char *)sqlite3_column_text(stmt, 1));
// 	usr->user_pass = strdup((const char *)sqlite3_column_text(stmt, 2));
// 	sqlite3_finalize(stmt);
// 	return usr;
// }

void mx_add_user(sqlite3 *db, char *login, char *pass) {
	sqlite3_stmt *stmt;
	int rv = 0;

	rv = sqlite3_prepare_v2(db, 
		"INSERT INTO USER(LOGIN, PASSWORD)VALUES(?1, ?2);",
		-1, &stmt, NULL);
	if (rv == SQLITE_ERROR) {
		fprintf(stderr, "Can't insert user into db: %s\n", sqlite3_errmsg(db));
		return ;
	}
	sqlite3_bind_text(stmt, 1, login, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, pass, -1, SQLITE_STATIC);
	if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
		fprintf(stderr, "Can't insert user into db: %s\n", sqlite3_errmsg(db));
	sqlite3_finalize(stmt);
}

static t_user *for_get_user(sqlite3_stmt *stmt) {
    t_user *user = NULL;
    int rv;
    
    if ((rv = sqlite3_step(stmt)) != SQLITE_ROW) {
        if (rv == SQLITE_ERROR) {
			fprintf(stderr, "getuser\n");
			return NULL;
        }
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

    sqlite3_prepare_v2(db, "SELECT * FROM USER WHERE USER_LOGIN = ?1",
        -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, user_login, -1, SQLITE_STATIC);
    if (rv != SQLITE_OK) {
		fprintf(stderr, "Can't get user from db: %s\n", sqlite3_errmsg(db));
		return NULL;
    }
    return for_get_user(stmt);
}

t_user *mx_get_user_by_user_id(sqlite3 *db, int user_id) {
    sqlite3_stmt *stmt;
    int rv = 0;

    sqlite3_prepare_v2(db, "SELECT * FROM USER WHERE USER_ID = ?1",
        -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, user_id);
    if (rv != SQLITE_OK) {
		fprintf(stderr, "Can't get user from db: %s\n", sqlite3_errmsg(db));
		return NULL;
    }
    return for_get_user(stmt);
}


