#include "uchat.h"

void mx_delete_user_from_group(sqlite3 *db, int user_id, int group_id) {
	int rv = 0;
	sqlite3_stmt *stmt;

	rv = sqlite3_prepare_v2(db, 
		"DELETE FROM GROUP_MEMBERS WHERE USER_ID = ?1 AND GROUP_ID = ?2;\
		DELETE FROM USER WHERE USER_ID = ?1",
		-1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, user_id);
	sqlite3_bind_int(stmt, 2, group_id);
	if (rv == SQLITE_ERROR) {
		fprintf(stderr, "Can't delete group_member_id from db: %s\n",
				sqlite3_errmsg(db));
		return ;
	}
	if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
		fprintf(stderr, "Can't delete user: %s\n", sqlite3_errmsg(db));
	sqlite3_finalize(stmt);
}
