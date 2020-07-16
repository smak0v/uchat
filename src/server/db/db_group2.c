#include "server.h"

t_list *mx_get_all_user_groups(sqlite3 *db, int user_id) {
    sqlite3_stmt *stmt;
    t_list *grp_sorted = NULL;
    t_all_gr_member *d = NULL;
    int rv = sqlite3_prepare_v2(db, "SELECT * FROM GRP INNER JOIN MSG ON " \
                       "GRP.GROUP_ID = MSG.GROUP_ID WHERE MSG.SENDER = ?1 "\
                       "GROUP BY GRP.GROUP_ID ORDER BY MSG.TIME DESC",
                       -1, &stmt, NULL);

    if (rv == SQLITE_ERROR)
        return NULL;
    sqlite3_bind_int(stmt, 1, user_id);
    while(sqlite3_step(stmt) == SQLITE_ROW) {
        d = mx_memalloc(sizeof(t_all_gr_member));
        d->group_id = sqlite3_column_int(stmt, 0);
        d->group_name = strdup((const char *)sqlite3_column_text(stmt, 1));
        mx_push_back(&grp_sorted, d);
    }
    sqlite3_finalize(stmt);
    return grp_sorted;
}
