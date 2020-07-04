#include "server.h"

int *mx_get_all_user_groups_member(sqlite3 *db, int user_id) { 
    sqlite3_stmt *stmt;
    int *groups = malloc(2 * sizeof(int));
    int n = 0;

    for (int i = 0; i < 2; i++)
        groups[i] = -1;
    sqlite3_prepare_v2(db, "SELECT * FROM GROUP_MEMBERS " \
                        "WHERE USER_ID = ?1", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, user_id);
    while(sqlite3_step(stmt) == SQLITE_ROW) {
        mx_printint(n);
        groups = realloc(groups, (sizeof(int) * (n + 1)));
        groups[n++] = sqlite3_column_int(stmt, 2);
        mx_printint(n);
    }
    sqlite3_finalize(stmt);
    if (n < 1)
        return groups;
    groups[n] = -1;
    return groups;
}

t_list *mx_get_all_user_dialogs(sqlite3 *db, int user_id) {
    sqlite3_stmt *stmt;
    t_list *g = NULL;

    sqlite3_prepare_v2(db, "SELECT * FROM DIALOG " \
                       "WHERE USER_ID1 = ?1 OR USER_ID2 = ?2", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, user_id);
    while(sqlite3_step(stmt) == SQLITE_ROW) {
        t_dialog *d = malloc(sizeof(t_dialog));
        d->dialog_id = sqlite3_column_int(stmt, 0);
        d->user_id1 = sqlite3_column_int(stmt, 1);
        d->user_id2 = sqlite3_column_int(stmt, 2);
        mx_push_back(&g, d);
    }
    sqlite3_finalize(stmt);
    if (g == NULL)
        return NULL;

    return g;
}
