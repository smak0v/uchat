#include "uchat.h"

int mx_add_dialog(sqlite3 *db, int user_id1, int user_id2) {
	sqlite3_stmt *stmt;
	int rv = 0;

	rv = sqlite3_prepare_v2(db,
		"INSERT INTO DIALOG(USER_ID1, USER_ID2)VALUES(?1, ?2);",
		-1, &stmt, NULL);
	if (rv == SQLITE_ERROR)
		return -1;
	sqlite3_bind_int(stmt, 1, user_id1);
	sqlite3_bind_int(stmt, 2, user_id2);
	if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
        return -1;
	sqlite3_finalize(stmt);
    return mx_get_dialog_id(db, user_id1, user_id2);
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
    return d == NULL ? NULL : d;
}

t_dialog *mx_get_dialog_by_id1_id2(sqlite3 *db, int id1, int id2) {
    sqlite3_stmt *stmt;
    int rv = 0;

    sqlite3_prepare_v2(db,
    	"SELECT * FROM DIALOG WHERE USER_ID1 = ?1 AND USER_ID2 = ?2",
    	-1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, id1);
    sqlite3_bind_int(stmt, 2, id2);
    if (rv != SQLITE_OK)
		return NULL;
    return for_get_dialog(stmt);
}

int mx_get_dialog_id(sqlite3 *db, int id1, int id2) {
    sqlite3_stmt *stmt;
    int rv = 0;
    int id = -1;

    rv = sqlite3_prepare_v2(db, "SELECT * FROM DIALOG WHERE \
       USER_ID1 = ?1 AND USER_ID2 = ?2", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, id1);
    sqlite3_bind_int(stmt, 2, id2);
    if (rv != SQLITE_OK)
        return -1;
    if ((rv = sqlite3_step(stmt)) != SQLITE_ROW)
        if (rv == SQLITE_ERROR)
            return -1;
    id = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return id == 0 ? -1 : id;
}
