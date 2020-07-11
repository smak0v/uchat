#include "server.h"

static t_list *next_group_msg(sqlite3 *db, int group_id, int n, int time) {
    sqlite3_stmt *stmt;
    t_list *group_msg = NULL;

    sqlite3_prepare_v2(db, "SELECT * FROM MSG WHERE GROUP_ID = ?1 AND TIME < ?2" \
                       "ORDER BY TIME DESC LIMIT ?3;", -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, group_id);
    sqlite3_bind_int(stmt, 2, time);
    sqlite3_bind_int(stmt, 3, n);

    while(sqlite3_step(stmt) == SQLITE_ROW) {
        t_msg *tmp = mx_fill_msg(stmt);

        mx_push_back(&group_msg, tmp);
        // mx_push_front(&group_msg, tmp);
    }

    sqlite3_finalize(stmt);

    return group_msg;
}

static t_list *next_dialog_msg(sqlite3 *db, int dialog_id, int n, int time) {
    sqlite3_stmt *stmt;
    t_list *dialog_msg = NULL;

    sqlite3_prepare_v2(db, "SELECT * FROM MSG WHERE DIALOG_ID = ?1 " \
                       "ORDER BY TIME < ?2 DESC LIMIT ?3;", -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, dialog_id);
    sqlite3_bind_int(stmt, 2, time);
    sqlite3_bind_int(stmt, 3, n);

    while(sqlite3_step(stmt) == SQLITE_ROW) {
        t_msg *tmp = mx_fill_msg(stmt);

        mx_push_back(&dialog_msg, tmp);
        // mx_push_front(&dialog_msg, tmp);
    }

    sqlite3_finalize(stmt);

    return dialog_msg;
}

t_list *mx_db_load_next_messages(sqlite3 *db, int group_id,
                                 int dialog_id, int n, int time) {
    t_list *messages = NULL;

    if (group_id > 0 && dialog_id < 0) {
        messages = next_group_msg(db, group_id, n, time);
    }

    if (group_id < 0 && dialog_id > 0) {
        messages = next_dialog_msg(db, dialog_id, n, time);
    }

    return messages;
}


