#include "uchat.h"

static int insert_to_table(sqlite3 *db, t_msg *m, char *str) {
    sqlite3_stmt *stmt = NULL;
    int rv = 0;

    if ((rv = sqlite3_prepare_v2(db, str, -1, &stmt, NULL)) == SQLITE_ERROR)
        return -1;

    sqlite3_bind_int(stmt, 3, m->sender);
    sqlite3_bind_int(stmt, 1, m->group_id);
    sqlite3_bind_text(stmt, 2, m->msg, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, m->time);
    sqlite3_bind_int(stmt, 5, m->edited);
    sqlite3_bind_int(stmt, 6, m->read);

    if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
        return -1;

    sqlite3_finalize(stmt);

    return mx_get_msg(db, m);
}

int mx_add_msg(sqlite3 *db, t_msg *m) {
    sqlite3_str *str = sqlite3_str_new(db);
    char *string = NULL;

    sqlite3_str_appendall(str, "INSERT INTO MSG");
    sqlite3_str_appendall(str,
                          "(GROUP_ID, MSG_BODY, SENDER, TIME, EDITED, READ)"
                          "VALUES(?1, ?2, ?3, ?4, ?5, ?6);");

    string = sqlite3_str_finish(str);

    return insert_to_table(db, m, string);
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

int mx_update_msg_by_id(sqlite3 *db, t_msg *m, int id) {
    sqlite3_stmt *stmt = NULL;
    int rv = sqlite3_prepare_v2(db,
        "UPDATE MSG SET MSG_BODY = ?1, EDITED = ?2 WHERE ID = ?3;",
        -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, m->msg, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, m->edited);
    sqlite3_bind_int(stmt, 3, id);

    if (rv == SQLITE_ERROR)
        return -1;

    if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
        return -1;

    sqlite3_finalize(stmt);

    return 0;
}
