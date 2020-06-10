#include "uchat.h"

static void insert_to_table(sqlite3 *db, t_msg *m, char *str) {
	sqlite3_stmt *stmt;
	int rv = 0;

	if ((rv = sqlite3_prepare_v2(db, str, -1, &stmt, NULL)) == SQLITE_ERROR)
		fprintf(stderr, "Can't add msg: %s\n", sqlite3_errmsg(db));
	// sqlite3_bind_int(stmt, 4, m->time);
	sqlite3_bind_int(stmt, 3, m->sender);
	sqlite3_bind_int(stmt, 1, m->group_id);
	sqlite3_bind_text(stmt, 2, m->msg, -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 4, m->time);
	sqlite3_bind_int(stmt, 5, m->edited);
	sqlite3_bind_int(stmt, 6, m->read);
	if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
		fprintf(stderr, "[2] Can't add msg: %s\n", sqlite3_errmsg(db));
    sqlite3_finalize(stmt);
}

void mx_add_msg(sqlite3 *db, t_msg *m) { // need to edit!!!
	sqlite3_str *str = sqlite3_str_new(db);
	char *string = NULL;

	sqlite3_str_appendall(str, "INSERT INTO MSG");
	sqlite3_str_appendall(str, 
						  "(GROUP_ID, MSG_BODY, SENDER, TIME, EDITED, READ)"
						  "VALUES(?1, ?2, ?3, ?4, ?5, ?6);");
	string = sqlite3_str_finish(str);
	insert_to_table(db, m, string);
	// return m;
}

static t_msg *get_msg(sqlite3_stmt *stmt) {
    t_msg *m = NULL;
    int rv;
    
    if ((rv = sqlite3_step(stmt)) != SQLITE_ROW) {
        if (rv == SQLITE_ERROR) {
			fprintf(stderr, "getmsg\n");
			return NULL;
        }
        sqlite3_finalize(stmt);
        return NULL;
    }
    m = malloc(sizeof(t_msg));
    m->id = sqlite3_column_int(stmt, 0);
    m->group_id = sqlite3_column_int(stmt, 1);
    m->sender = sqlite3_column_int(stmt, 2);
    m->msg = strdup((const char*)sqlite3_column_text(stmt, 3));
    m->time = sqlite3_column_int(stmt, 4);
    m->edited = sqlite3_column_int(stmt, 5);
    m->read = sqlite3_column_int(stmt, 6);
    sqlite3_finalize(stmt);
    return m;
}

t_msg *mx_get_msg_by_id(sqlite3 *db, int id) {
    sqlite3_stmt *stmt;
    int rv = 0;

    sqlite3_prepare_v2(db, "SELECT * FROM MSG WHERE ID = ?1",
        -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, id);
    if (rv != SQLITE_OK) {
		fprintf(stderr, "Can't get msg: %s\n", sqlite3_errmsg(db));
		return NULL;
    }
    return get_msg(stmt);
}

void mx_delete_msg_by_id(sqlite3 *db, int id) {
	int rv = 0;
	sqlite3_stmt *stmt;

	rv = sqlite3_prepare_v2(db, "DELETE FROM MSG WHERE ID = ?1;",
							-1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, id);
	if (rv == SQLITE_ERROR) {
		fprintf(stderr, "Can't delete msg: %s\n", sqlite3_errmsg(db));
		return ;
	}
	if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
		fprintf(stderr, "Can't delete msg: %s\n", sqlite3_errmsg(db));
	sqlite3_finalize(stmt);
}

void mx_update_msg_by_id(sqlite3 *db, t_msg *m, int id) {
	int rv = 0;
	sqlite3_stmt *stmt;

	rv = sqlite3_prepare_v2(db, 
		"UPDATE MSG SET MSG_BODY = ?1, EDITED = ?2 WHERE ID = ?3;",
		-1, &stmt, NULL);
	sqlite3_bind_text(stmt, 1, m->msg, -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 2, m->edited);
	sqlite3_bind_int(stmt, 3, id);
	if (rv == SQLITE_ERROR) {
		fprintf(stderr, "Can't update msg: %s\n", sqlite3_errmsg(db));
		return ;
	}
	if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
		fprintf(stderr, "Can't update msg: %s\n", sqlite3_errmsg(db));
	sqlite3_finalize(stmt);
}
