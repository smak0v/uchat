#include "uchat.h"

void mx_add_group_member(sqlite3 *db, int user_id, int group_id) {
	sqlite3_stmt *stmt;
	int rv = 0;

	rv = sqlite3_prepare_v2(db, 
		"INSERT INTO GROUP_MEMBERS(USER_ID, GROUP_ID)VALUES(?1, ?2);",
		-1, &stmt, NULL);
	if (rv == SQLITE_ERROR) {
		fprintf(stderr, "Can't insert group member into db: %s\n", 
				sqlite3_errmsg(db));
		return ;
	}
	sqlite3_bind_int(stmt, 1, user_id);
	sqlite3_bind_int(stmt, 2, group_id);
	if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
		fprintf(stderr, "Can't insert group member into db: %s\n", 
				sqlite3_errmsg(db));
	sqlite3_finalize(stmt);
}

static t_gr_members *for_get_member(sqlite3_stmt *stmt) {
    t_gr_members *mem = NULL;
    int rv;
    
    if ((rv = sqlite3_step(stmt)) != SQLITE_ROW) {
        if (rv == SQLITE_ERROR) {
			fprintf(stderr, "get member\n");
			return NULL;
        }
        sqlite3_finalize(stmt);
        return NULL;
    }
    mem = malloc(sizeof(t_gr_members));
    mem->gr_members_id = sqlite3_column_int(stmt, 0);
    mem->user_id = sqlite3_column_int(stmt, 1);
    mem->group_id = sqlite3_column_int(stmt, 2);
    sqlite3_finalize(stmt);
    return mem;
}

t_gr_members *mx_get_by_group_mem_id(sqlite3 *db, char *gr_member_id) {
    sqlite3_stmt *stmt;
    int rv = 0;

    sqlite3_prepare_v2(db, 
		"SELECT * FROM GROUP_MEMBERS WHERE GROUP_MEMBERS_ID = ?1",
    	-1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, gr_member_id, -1, SQLITE_STATIC);
    if (rv != SQLITE_OK) {
		fprintf(stderr, "Can't group member user from db: %s\n", 
				sqlite3_errmsg(db));
		return NULL;
    }
    return for_get_member(stmt);
}
