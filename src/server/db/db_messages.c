#include "server.h"

static int insert_to_table(sqlite3 *db, t_msg *m, sqlite3_stmt *stmt) {
    int rv = 0;

    sqlite3_bind_int(stmt, 1, m->group_id);
    sqlite3_bind_int(stmt, 2, m->dialog_id);
    sqlite3_bind_int(stmt, 3, m->sender);
    sqlite3_bind_text(stmt, 4, m->msg, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, m->time);
    sqlite3_bind_int(stmt, 6, m->edited);
    sqlite3_bind_int(stmt, 7, m->read);
    sqlite3_bind_text(stmt, 8, m->file, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 9, m->forwarded, -1, SQLITE_STATIC);

    if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
        return -1;
    sqlite3_finalize(stmt);

    return mx_get_msg(db, m);
}

int mx_add_msg(sqlite3 *db, t_msg *m) {
    sqlite3_stmt *stmt = NULL;
    int rv = sqlite3_prepare_v2(db, "INSERT INTO MSG\
            (GROUP_ID, DIALOG_ID, SENDER, MSG_BODY, TIME, EDITED, READ, "\
            "FILE, FORWARDED)VALUES(?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9);",
            -1, &stmt, NULL);

    if (rv == SQLITE_ERROR)
        return -1;

    return insert_to_table(db, m, stmt);
}

int mx_delete_msg_by_id(sqlite3 *db, int id) {
    sqlite3_stmt *stmt = NULL;
    int rv = sqlite3_prepare_v2(db, "DELETE FROM MSG WHERE ID = ?1;",
            -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, id);

    if (rv == SQLITE_ERROR)
        return -1;

    if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
        return -1;

    sqlite3_finalize(stmt);

    return 0;
}

int mx_update_msg_by_id(sqlite3 *db, char *msg, int id) {
    sqlite3_stmt *stmt = NULL;
    int rv = sqlite3_prepare_v2(db,
            "UPDATE MSG SET MSG_BODY = ?1, EDITED = ?2 WHERE ID = ?3;",
            -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, msg, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, 1);
    sqlite3_bind_int(stmt, 3, id);

    if (rv == SQLITE_ERROR)
        return -1;

    if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
        return -1;

    sqlite3_finalize(stmt);

    return 0;
}
