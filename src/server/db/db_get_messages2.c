#include "server.h"

static t_list *get_group_msg(sqlite3 *db, int group_id, int n) {
    sqlite3_stmt *stmt;
    t_list *group_msg = NULL;

    sqlite3_prepare_v2(db, "SELECT * FROM MSG INNER JOIN USER ON " \
                       "MSG.SENDER = USER.USER_ID WHERE GROUP_ID = ?1 " \
                       "ORDER BY TIME DESC LIMIT ?2;", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, group_id);
    sqlite3_bind_int(stmt, 2, n);
    while(sqlite3_step(stmt) == SQLITE_ROW) {
        t_msg *tmp = mx_fill_msg(stmt);

        tmp->username = strdup((const char*)sqlite3_column_text(stmt, 11));
        mx_push_back(&group_msg, tmp);
    }
    sqlite3_finalize(stmt);
    return group_msg;
}

static t_list *get_dialog_msg(sqlite3 *db, int dialog_id, int n) {
    sqlite3_stmt *stmt;
    t_list *dialog_msg = NULL;

    sqlite3_prepare_v2(db, "SELECT * FROM MSG INNER JOIN USER ON " \
                       "MSG.SENDER = USER.USER_ID WHERE DIALOG_ID = ?1 " \
                       "ORDER BY TIME DESC LIMIT ?2;", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, dialog_id);
    sqlite3_bind_int(stmt, 2, n);
    while(sqlite3_step(stmt) == SQLITE_ROW) {
        t_msg *tmp = mx_fill_msg(stmt);

        tmp->username = strdup((const char*)sqlite3_column_text(stmt, 11));
        mx_push_back(&dialog_msg, tmp);
    }
    sqlite3_finalize(stmt);
    return dialog_msg;
}

t_list *mx_db_load_messages(sqlite3 *db, int group_id, int dialog_id, int n) {
    t_list *messages = NULL;

    if (group_id > 0 && dialog_id < 0) {
        messages = get_group_msg(db, group_id, n);
    }

    if (group_id < 0 && dialog_id > 0) {
        messages = get_dialog_msg(db, dialog_id, n);
    }

    return messages;
}


