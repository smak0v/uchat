#include "server.h"

t_list *mx_get_all_user_dialogs(sqlite3 *db, int user_id) {
    sqlite3_stmt *stmt;
    t_list *g = NULL;
    t_dialog *d = NULL;

    sqlite3_prepare_v2(db, "SELECT * FROM DIALOG WHERE USER_ID1 = ?1 " \
                       "OR USER_ID2 = ?2", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, user_id);
    while(sqlite3_step(stmt) == SQLITE_ROW) {
        d = malloc(sizeof(t_dialog));
        d->dialog_id = sqlite3_column_int(stmt, 0);
        d->user_id1 = sqlite3_column_int(stmt, 1);
        d->user_id2 = sqlite3_column_int(stmt, 2);
        mx_push_back(&g, d);
    }
    sqlite3_finalize(stmt);
    return g;
}

t_list *mx_get_dialog_users(sqlite3 *db, int usr_id) {
    t_list *g = mx_get_all_user_dialogs(db, usr_id);
    t_list *dialog = NULL;

    for (t_list *n = g; n; n = n->next) {
        t_dialog_users *usr = malloc(sizeof(t_dialog_users));
        t_dialog *d = n->data;

        usr->dialog_id = d->dialog_id;
        if (d->user_id1 == usr_id && d->user_id2 != usr_id)
            usr->user_id = d->user_id2;
        if (d->user_id2 == usr_id && d->user_id1 != usr_id)
            usr->user_id = d->user_id1;
        usr->username = mx_get_user_login_by_id(db, usr->user_id);
        mx_push_back(&dialog, usr);
        free(d);
    }
    mx_delete_list(g);
    return dialog;
}
