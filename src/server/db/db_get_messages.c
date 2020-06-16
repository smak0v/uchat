#include "uchat.h"

static t_msg *get_msg(sqlite3_stmt *stmt) {
    t_msg *m = NULL;
    int rv = 0;

    if ((rv = sqlite3_step(stmt)) != SQLITE_ROW) {
        if (rv == SQLITE_ERROR)
			return NULL;
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
    if (rv != SQLITE_OK)
		return NULL;
    return get_msg(stmt);
}

int mx_get_msg(sqlite3 *db, t_msg *m) {
    sqlite3_stmt *stmt;
    int rv = 0;
    int id = -1;

    rv = sqlite3_prepare_v2(db, "SELECT * FROM MSG WHERE GROUP_ID = ?1 AND\
       MSG_BODY = ?2 AND SENDER = ?3 AND TIME = ?4", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, m->group_id);
    sqlite3_bind_text(stmt, 2, m->msg, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, m->sender);
    sqlite3_bind_int(stmt, 4, m->time);
    if (rv != SQLITE_OK)
        return -1;
    if ((rv = sqlite3_step(stmt)) != SQLITE_ROW)
        if (rv == SQLITE_ERROR)
            return -2;
    id = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return id == 0 ? -1 : id;
}
