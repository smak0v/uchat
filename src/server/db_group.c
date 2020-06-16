#include "uchat.h"

int mx_add_grp(sqlite3 *db, char *group_name) {
	sqlite3_stmt *stmt;
	int rv = 0;

	if (mx_get_grp_id(db, group_name) > 0)
		return -2;
	rv = sqlite3_prepare_v2(db, "INSERT INTO GRP(GROUP_NAME) VALUES(?1);",
		-1, &stmt, NULL);
	if (rv == SQLITE_ERROR)
		return -1;
	sqlite3_bind_text(stmt, 1, group_name, -1, SQLITE_STATIC);
	if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
		return -1;
	sqlite3_finalize(stmt);
	return mx_get_grp_id(db, group_name);
}

int mx_rename_grp_by_name(sqlite3 *db, char *grp_name, char *new_name) {
    int rv = 0;
    sqlite3_stmt *stmt;

    rv = sqlite3_prepare_v2(db, 
       "UPDATE GRP SET GROUP_NAME = ?1 WHERE GROUP_NAME = ?2;",
       -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, new_name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, grp_name, -1, SQLITE_STATIC);
    if (rv == SQLITE_ERROR)
        return -1;
    if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
        return -1;
    sqlite3_finalize(stmt);
	return 1;
}

int mx_delete_grp_by_id(sqlite3 *db, int grp_id) {
	int rv = 0;
	sqlite3_stmt *stmt;

	rv = sqlite3_prepare_v2(db, "DELETE FROM GRP WHERE GROUP_ID = ?1;",
	   -1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, grp_id);
	if (rv == SQLITE_ERROR)
		return -1;
	if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
		return -1;
	sqlite3_finalize(stmt);
	return 1;
}

int mx_get_grp_id(sqlite3 *db, char *grp_name) {
    sqlite3_stmt *stmt;
    int rv = 0;
    int id = -1;

    rv = sqlite3_prepare_v2(db, "SELECT * FROM GRP WHERE GROUP_NAME = ?1",
       -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, grp_name, -1, SQLITE_STATIC);
    if (rv != SQLITE_OK)
        return -1;
    if ((rv = sqlite3_step(stmt)) != SQLITE_ROW)
        if (rv == SQLITE_ERROR)
            return -1;
    id = sqlite3_column_int(stmt, 0); // 0 = not found
    sqlite3_finalize(stmt);
    return id == 0 ? -1 : id;
}


