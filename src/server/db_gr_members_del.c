#include "uchat.h"

void mx_delete_by_gr_mem_id(sqlite3 *db, int group_members_id) {
	int rv = 0;
	sqlite3_stmt *stmt;

	rv = sqlite3_prepare_v2(db, 
		"DELETE FROM GROUP_MEMBERS WHERE GROUP_MEMBERS_ID = ?1;",
		-1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, group_members_id);
	if (rv == SQLITE_ERROR) {
		fprintf(stderr, "Can't delete group_member_id from db: %s\n",
				sqlite3_errmsg(db));
		return ;
	}
	if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
		fprintf(stderr, "Can't delete user from db: %s\n", sqlite3_errmsg(db));
	sqlite3_finalize(stmt);
}

void mx_delete_by_user_id(sqlite3 *db, int user_id) {
	int rv = 0;
	sqlite3_stmt *stmt;

	rv = sqlite3_prepare_v2(db, 
		"DELETE FROM GROUP_MEMBERS WHERE USER_ID = ?1;",
		-1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, user_id);
	if (rv == SQLITE_ERROR) {
		fprintf(stderr, "Can't delete group_member_id from db: %s\n",
				sqlite3_errmsg(db));
		return ;
	}
	if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
		fprintf(stderr, "Can't delete user from db: %s\n", sqlite3_errmsg(db));
	sqlite3_finalize(stmt);
}

void mx_delete_by_gr_id(sqlite3 *db, int group_id) {
	int rv = 0;
	sqlite3_stmt *stmt;

	rv = sqlite3_prepare_v2(db, 
		"DELETE FROM GROUP_MEMBERS WHERE GROUP_ID = ?1;",
		-1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, group_id);
	if (rv == SQLITE_ERROR) {
		fprintf(stderr, "Can't delete group_member_id from db: %s\n",
				sqlite3_errmsg(db));
		return ;
	}
	if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
		fprintf(stderr, "Can't delete user from db: %s\n", sqlite3_errmsg(db));
	sqlite3_finalize(stmt);
}
