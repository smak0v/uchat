#include "uchat.h"

void mx_add_group(sqlite3 *db, int group_id) {
	sqlite3_stmt *stmt;
	int rv = 0;

	rv = sqlite3_prepare_v2(db, 
		"INSERT INTO GRP(GROUP_ID) VALUES(?1);",
		-1, &stmt, NULL);
	if (rv == SQLITE_ERROR) {
		fprintf(stderr, "Can't insert group_id into db: %s\n", 
				sqlite3_errmsg(db));
		return ;
	}
	sqlite3_bind_int(stmt, 1, group_id);
	if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
		fprintf(stderr, "[2] Can't insert group_id into db: %s\n", 
				sqlite3_errmsg(db));
	sqlite3_finalize(stmt);
}

void mx_update_group_id(sqlite3 *db, int group_id, int new_id) {
	int rv = 0;
	sqlite3_stmt *stmt;

	rv = sqlite3_prepare_v2(db, 
		"UPDATE GROUP SET GROUP_ID = ?1 WHERE GROUP_ID = ?2;",
		-1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, new_id);
	sqlite3_bind_int(stmt, 2, group_id);
	if (rv == SQLITE_ERROR) {
		fprintf(stderr, "Can't update dialog in db: %s\n", sqlite3_errmsg(db));
		return ;
	}
	if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
		fprintf(stderr, "Can't update user in db: %s\n", sqlite3_errmsg(db));
	sqlite3_finalize(stmt);
}

void mx_delete_group_id(sqlite3 *db, int group_id) {
	int rv = 0;
	sqlite3_stmt *stmt;

	rv = sqlite3_prepare_v2(db, "DELETE FROM GROUP WHERE GROUP_ID = ?1;",
							-1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, group_id);
	if (rv == SQLITE_ERROR) {
		fprintf(stderr, "Can't delete group_id from db: %s\n", sqlite3_errmsg(db));
		return ;
	}
	if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
		fprintf(stderr, "Can't delete group_id from db: %s\n", sqlite3_errmsg(db));
	sqlite3_finalize(stmt);
}

static int ret(sqlite3_stmt *stmt) {
    int rv = 0;
    int retval = 0;

	if ((rv = sqlite3_step(stmt)) != SQLITE_ROW) {
    	if (rv == SQLITE_ERROR) {
			fprintf(stderr, "Group_id\n");
			return -1;
        }
        sqlite3_finalize(stmt);
        return -1;
    }
    retval = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return retval;
}

int mx_get_group_id(sqlite3 *db) {
    sqlite3_stmt *stmt;
    int rv = 0;

    sqlite3_prepare_v2(db, "SELECT GROUP_ID FROM GROUP",
        -1, &stmt, NULL);
    // sqlite3_bind_int(stmt, 1, NULL);
    if (rv != SQLITE_OK) {
		fprintf(stderr, "Can't get group_id: %s\n", sqlite3_errmsg(db));
    	return -1;
    }
    return ret(stmt);
}
