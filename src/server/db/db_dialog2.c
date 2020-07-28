#include "server.h"

static t_list *all_user_dialogs_non_sorted(sqlite3 *db, int user_id) {
    sqlite3_stmt *stmt;
    t_list *g = NULL;
    t_dialog_sorted *d = NULL;

    sqlite3_prepare_v2(db, "SELECT * FROM DIALOG WHERE USER_ID1 = ?1 " \
                       "OR USER_ID2 = ?1", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, user_id);
    while(sqlite3_step(stmt) == SQLITE_ROW) {
        d = malloc(sizeof(t_dialog_sorted));
        d->dialog_id = sqlite3_column_int(stmt, 0);
        d->user_id1 = sqlite3_column_int(stmt, 1);
        d->user_id2 = sqlite3_column_int(stmt, 2);
        d->time = mx_get_time_dialog(db, d->dialog_id, user_id);
        mx_push_back(&g, d);
    }
    sqlite3_finalize(stmt);
    return g;
}

t_list *mx_get_dialog_users(sqlite3 *db, int usr_id, int *len) {
    t_list *g = mx_get_all_user_dialogs(db, usr_id);
    t_list *dialog = NULL;
    t_dialog_users *usr = NULL;
    t_dialog *d = NULL;

    for (t_list *n = g; n; n = n->next) {
        usr = malloc(sizeof(t_dialog_users));
        d = n->data;
        usr->dialog_id = d->dialog_id;
        if (d->user_id1 == usr_id && d->user_id2 != usr_id)
            usr->user_id = d->user_id2;
        if (d->user_id2 == usr_id && d->user_id1 != usr_id)
            usr->user_id = d->user_id1;
        usr->username = mx_get_user_login_by_id(db, usr->user_id);
        mx_push_back(&dialog, usr);
        free(d);
        *len += 1;
    }
    mx_delete_list(g);
    return dialog;
}

int mx_get_time_dialog(sqlite3 *db, int dialog_id, int user_id) {
    sqlite3_stmt *stmt;
    int time = -1;
    int rv = sqlite3_prepare_v2(db, "SELECT TIME FROM MSG WHERE \
        DIALOG_ID = ?1 AND SENDER = ?2 ORDER BY TIME DESC", -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, dialog_id);
    sqlite3_bind_int(stmt, 2, user_id);
    if (rv != SQLITE_OK)
        return -1;
    if ((rv = sqlite3_step(stmt)) != SQLITE_ROW)
        if (rv == SQLITE_ERROR)
            return -1;
    time = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return time;
}

static bool time_dialog(void *data_1, void *data_2) {
    t_dialog_sorted *d1 = (t_dialog_sorted *)data_1;
    t_dialog_sorted *d2 = (t_dialog_sorted *)data_2;

    if (d1->time < d2->time)
        return true;
    return false;
}

t_list *mx_get_all_user_dialogs(sqlite3 *db, int user_id) {
    t_list *dialogs = all_user_dialogs_non_sorted(db, user_id);
    t_list *new = mx_sort_list(dialogs, time_dialog);
    t_list *sorted = NULL;
    t_dialog *d = NULL;

    for (t_list *p = new; p; p = p->next) {
        t_dialog_sorted *ptr = p->data;

        d = malloc(sizeof(t_dialog));
        d->dialog_id = ptr->dialog_id;
        d->user_id1 = ptr->user_id1;
        d->user_id2 = ptr->user_id2;
        mx_push_back(&sorted, d);
        free(ptr);
    }
    mx_delete_list(dialogs);
    return sorted;
}
