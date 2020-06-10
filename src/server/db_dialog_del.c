#include "uchat.h"

void mx_delete_by_dialog_id(sqlite3 *db, int dialog_id) {
	int rv = 0;
	sqlite3_stmt *stmt;

	rv = sqlite3_prepare_v2(db, "DELETE FROM DIALOG WHERE DIALOG_ID = ?1;",
							-1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, dialog_id);
	if (rv == SQLITE_ERROR) {
		fprintf(stderr, "Can't delete user from db: %s\n", sqlite3_errmsg(db));
		return ;
	}
	if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
		fprintf(stderr, "Can't delete user from db: %s\n", sqlite3_errmsg(db));
	sqlite3_finalize(stmt);
}

void mx_update_by_dialog_id(sqlite3 *db, int dialog_id, 
							int user_id1, int user_id2) {
	int rv = 0;
	sqlite3_stmt *stmt;

	rv = sqlite3_prepare_v2(db, 
		"UPDATE DIALOG SET USER_ID1 = ?2, USER_ID2 = ?3 WHERE DIALOG_ID = ?1;",
		-1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, dialog_id);
	sqlite3_bind_int(stmt, 2, user_id1);
	sqlite3_bind_int(stmt, 3, user_id2);
	if (rv == SQLITE_ERROR) {
		fprintf(stderr, "Can't update dialog in db: %s\n", sqlite3_errmsg(db));
		return ;
	}
	if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
		fprintf(stderr, "Can't update user in db: %s\n", sqlite3_errmsg(db));
	sqlite3_finalize(stmt);
}
