#include "uchat.h"

void mx_add_dialog(sqlite3 *db, int user_id1, int user_id2) {
	sqlite3_stmt *stmt;
	int rv = 0;

	rv = sqlite3_prepare_v2(db, 
		"INSERT INTO DIALOG(USER_ID1, USER_ID2)VALUES(?1, ?2);",
		-1, &stmt, NULL);
	if (rv == SQLITE_ERROR) {
		fprintf(stderr, "Can't insert dialog into db: %s\n", 
				sqlite3_errmsg(db));
		return ;
	}
	sqlite3_bind_int(stmt, 1, user_id1);
	sqlite3_bind_int(stmt, 2, user_id2);
	if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
		fprintf(stderr, "Can't insert dialog into db: %s\n", 
				sqlite3_errmsg(db));
	sqlite3_finalize(stmt);
}

static t_dialog *for_get_dialog(sqlite3_stmt *stmt) {
    t_dialog *d = NULL;
    int rv;
    
    if ((rv = sqlite3_step(stmt)) != SQLITE_ROW) {
        if (rv == SQLITE_ERROR) {
			fprintf(stderr, "get dialog\n");
			return NULL;
        }
        sqlite3_finalize(stmt);
        return NULL;
    }
    d = malloc(sizeof(t_dialog));
    d->dialog_id = sqlite3_column_int(stmt, 0);
    d->user_id1 = sqlite3_column_int(stmt, 1);
    d->user_id2 = sqlite3_column_int(stmt, 2);
    sqlite3_finalize(stmt);
    return d;
}

t_dialog *mx_get_by_dialog_id(sqlite3 *db, int dialog_id) {
    sqlite3_stmt *stmt;
    int rv = 0;

    sqlite3_prepare_v2(db, 
		"SELECT * FROM DIALOG WHERE DIALOG_ID = ?1",
    	-1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, dialog_id);
    if (rv != SQLITE_OK) {
		fprintf(stderr, "Can't group member user from db: %s\n", 
				sqlite3_errmsg(db));
		return NULL;
    }
    return for_get_dialog(stmt);
}
