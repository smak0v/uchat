#include "server.h"

// t_msg *mx_fill_msg(sqlite3_stmt *stmt) {
//     t_msg *m = malloc(sizeof(t_msg));

//     m->id = sqlite3_column_int(stmt, 0);
//     m->group_id = sqlite3_column_int(stmt, 1);
//     m->dialog_id = sqlite3_column_int(stmt, 2);
//     m->sender = sqlite3_column_int(stmt, 3);
//     m->msg = strdup((const char*)sqlite3_column_text(stmt, 4));
//     m->time = sqlite3_column_int(stmt, 5);
//     m->edited = sqlite3_column_int(stmt, 6);
//     m->read = sqlite3_column_int(stmt, 7);
//     if (sqlite3_column_text(stmt, 8) != NULL)
//         m->file = strdup((const char*)sqlite3_column_text(stmt, 8));
//     else
//         m->file = NULL;
//     if (sqlite3_column_text(stmt, 9) != NULL)
//         m->forwarded = strdup((const char*)sqlite3_column_text(stmt, 9));
//     else
//         m->forwarded = NULL;
//     return m;
// }

static t_list *get_group_msg(sqlite3 *db, int group_id, int n) {
    sqlite3_stmt *stmt;
    t_list *group_msg = NULL;
    
    sqlite3_prepare_v2(db, "SELECT * FROM MSG WHERE GROUP_ID = ?1 " \
                       "ORDER BY TIME DESC LIMIT ?2;", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, group_id);
    sqlite3_bind_int(stmt, 2, n);

    while(sqlite3_step(stmt) == SQLITE_ROW) {
        t_msg *tmp = mx_fill_msg(stmt);

        mx_push_back(&group_msg, tmp);
    }
    sqlite3_finalize(stmt);

    return group_msg;
}

static t_list *get_dialog_msg(sqlite3 *db, int dialog_id, int n) {
    sqlite3_stmt *stmt;
    t_list *dialog_msg = NULL;

    sqlite3_prepare_v2(db, "SELECT * FROM MSG WHERE DIALOG_ID = ?1 " \
                       "ORDER BY TIME DESC LIMIT ?2;", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, dialog_id);
    sqlite3_bind_int(stmt, 2, n);

    while(sqlite3_step(stmt) == SQLITE_ROW) {
        t_msg *tmp = mx_fill_msg(stmt);

        mx_push_back(&dialog_msg, tmp);
    }
    sqlite3_finalize(stmt);

    return dialog_msg;
}

t_list *mx_db_load_messages(sqlite3 *db, int group_id, int dialog_id, int n) {
    t_list *messages = NULL;

    if (group_id > 0 && dialog_id < 0) {
        messages = get_group_msg(db, group_id, n);
        mx_printstr_endl("group********");
    }
    if (group_id < 0 && dialog_id > 0) {
        messages = get_dialog_msg(db, dialog_id, n);
        mx_printstr_endl("dialog********");
    }
    return messages;
}


