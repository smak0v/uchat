#include "server.h"

char *mx_get_user_login_by_id(sqlite3 *db, int user_id) {
    sqlite3_stmt *stmt = NULL;
    char *login = NULL;
    int rv = sqlite3_prepare_v2(db, "SELECT * FROM USER WHERE USER_ID = ?1",
                                -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, user_id);

    if (rv != SQLITE_OK)
        return NULL;
    if ((rv = sqlite3_step(stmt)) != SQLITE_ROW)
        if (rv == SQLITE_ERROR)
            return NULL;
    if (sqlite3_column_text(stmt, 1) != NULL)
        login = strdup((char*)sqlite3_column_text(stmt, 1));

    sqlite3_finalize(stmt);

    return login;
}

t_list *mx_find_user_by_char(sqlite3 *db, char *str) {
    sqlite3_stmt *stmt;
    t_list *g = NULL;
    char *sql = mx_strjoin("SELECT * FROM USER WHERE LOGIN LIKE '", str);
    char *req = mx_strjoin(sql, "%'");

    sqlite3_prepare_v2(db, req, -1, &stmt, NULL);
    while(sqlite3_step(stmt) == SQLITE_ROW) {
        t_user *d = malloc(sizeof(t_user));

        d->user_id = sqlite3_column_int(stmt, 0);
        d->user_login = strdup((char*)sqlite3_column_text(stmt, 1));
        d->user_pass = strdup((char*)sqlite3_column_text(stmt, 2));
        mx_push_back(&g, d);
    }
    sqlite3_finalize(stmt);
    free(req);
    free(sql);
    return g;
}
