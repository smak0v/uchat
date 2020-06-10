#include "uchat.h"

void mx_add_grp(sqlite3 *db, char *group_name) {
	sqlite3_stmt *stmt;
	int rv = 0;

	rv = sqlite3_prepare_v2(db, 
		"INSERT INTO GRP(GROUP_NAME) VALUES(?1);",
		-1, &stmt, NULL);
	if (rv == SQLITE_ERROR) {
		fprintf(stderr, "Can't insert grp_name into db: %s\n", 
				sqlite3_errmsg(db));
		return ;
	}
	sqlite3_bind_text(stmt, 1, group_name, -1, SQLITE_STATIC);
	if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
		fprintf(stderr, "[2] Can't insert grp_name into db: %s\n", 
				sqlite3_errmsg(db));
	sqlite3_finalize(stmt);
}

void mx_update_grp_by_name(sqlite3 *db, char *group_name, char *new_name) {
	int rv = 0;
	sqlite3_stmt *stmt;

	rv = sqlite3_prepare_v2(db, 
		"UPDATE GRP SET GROUP_NAME = ?1 WHERE GROUP_NAME = ?2;",
		-1, &stmt, NULL);
	sqlite3_bind_text(stmt, 1, new_name, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, group_name, -1, SQLITE_STATIC);
	if (rv == SQLITE_ERROR) {
		fprintf(stderr, "Can't update grp_name: %s\n", sqlite3_errmsg(db));
		return ;
	}
	if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
		fprintf(stderr, "Can't update grp_name: %s\n", sqlite3_errmsg(db));
	sqlite3_finalize(stmt);
}

void mx_delete_grp_by_name(sqlite3 *db, char *grp_name) {
	int rv = 0;
	sqlite3_stmt *stmt;

	rv = sqlite3_prepare_v2(db, "DELETE FROM GRP WHERE GROUP_NAME = ?1;",
							-1, &stmt, NULL);
	sqlite3_bind_text(stmt, 1, grp_name, -1, SQLITE_STATIC);
	if (rv == SQLITE_ERROR) {
		fprintf(stderr, "Can't delete by grp_name: %s\n", sqlite3_errmsg(db));
		return ;
	}
	if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
		fprintf(stderr, "Can't delete by grp_name: %s\n", sqlite3_errmsg(db));
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

int mx_get_grp_id(sqlite3 *db, char *grp_name) {
    sqlite3_stmt *stmt;
    int rv = 0;

    sqlite3_prepare_v2(db, "SELECT * FROM GRP WHERE GROUP_NAME = ?1",
        -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, grp_name, -1, SQLITE_STATIC);
    if (rv != SQLITE_OK) {
		fprintf(stderr, "Can't get group_id: %s\n", sqlite3_errmsg(db));
    	return -1;
    }
    return ret(stmt);
}
