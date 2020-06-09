#include "uchat.h"

void mx_delete_user_by_login(sqlite3 *db, char *login) {
	int rv = 0;
	sqlite3_stmt *stmt;

	rv = sqlite3_prepare_v2(db, "DELETE FROM USER WHERE LOGIN = ?1;",
							-1, &stmt, NULL);
	sqlite3_bind_text(stmt, 1, login, -1, SQLITE_STATIC);
	if (rv == SQLITE_ERROR) {
		fprintf(stderr, "Can't delete user from db: %s\n", sqlite3_errmsg(db));
		return ;
	}
	if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
		fprintf(stderr, "Can't delete user from db: %s\n", sqlite3_errmsg(db));
	sqlite3_finalize(stmt);
}

void mx_delete_user_by_id(sqlite3 *db, int user_id) {
	int rv = 0;
	sqlite3_stmt *stmt;

	rv = sqlite3_prepare_v2(db, "DELETE FROM USER WHERE USER_ID = ?1;",
							-1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, user_id);
	if (rv == SQLITE_ERROR) {
		fprintf(stderr, "Can't delete user from db: %s\n", sqlite3_errmsg(db));
		return ;
	}
	if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
		fprintf(stderr, "Can't delete user from db: %s\n", sqlite3_errmsg(db));
	sqlite3_finalize(stmt);
}
