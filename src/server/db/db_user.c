#include "uchat.h"

static t_user *for_get_user(sqlite3_stmt *stmt) {
    t_user *user = NULL;
    int rv = 0;

    if ((rv = sqlite3_step(stmt)) != SQLITE_ROW) {
        if (rv == SQLITE_ERROR)
            return NULL;

        sqlite3_finalize(stmt);

        return NULL;
    }

    user = malloc(sizeof(t_user));
    user->user_id = sqlite3_column_int(stmt, 0);
    user->user_login = strdup((const char*)sqlite3_column_text(stmt, 1));
    user->user_pass = strdup((const char*)sqlite3_column_text(stmt, 2));

    sqlite3_finalize(stmt);

    return user;
}

int mx_add_user(sqlite3 *db, char *login, char *pass) {
    sqlite3_stmt *stmt = NULL;
    int rv = 0;

    if (mx_get_user_id_by_login(db, login) > 0)
        return -2;

    rv = sqlite3_prepare_v2(db, "INSERT INTO USER(LOGIN, PASSWORD)"\
                            "VALUES(?1, ?2);", -1, &stmt, NULL);

    if (rv == SQLITE_ERROR)
        return -1;

    sqlite3_bind_text(stmt, 1, login, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, pass, -1, SQLITE_STATIC);

    if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
        return -1;

    sqlite3_finalize(stmt);
    return mx_get_user_id_by_login(db, login);
}

t_user *mx_get_user_by_login(sqlite3 *db, char *user_login) {
    sqlite3_stmt *stmt = NULL;
    int rv = sqlite3_prepare_v2(db, "SELECT * FROM USER WHERE LOGIN = ?1",
                                -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, user_login, -1, SQLITE_STATIC);

    if (rv != SQLITE_OK)
        return NULL;

    return for_get_user(stmt);
}

t_user *mx_get_user_by_user_id(sqlite3 *db, int user_id) {
    sqlite3_stmt *stmt;
    int rv = sqlite3_prepare_v2(db, "SELECT * FROM USER WHERE USER_ID = ?1",
                                -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, user_id);

    if (rv != SQLITE_OK)
        return NULL;

    return for_get_user(stmt);
}

int mx_get_user_id_by_login(sqlite3 *db, char *login) {
    sqlite3_stmt *stmt = NULL;
    int id = -1;
    int rv = sqlite3_prepare_v2(db, "SELECT * FROM USER WHERE LOGIN = ?1",
                                -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, login, -1, SQLITE_STATIC);

    if (rv != SQLITE_OK)
        return -1;

    if ((rv = sqlite3_step(stmt)) != SQLITE_ROW)
        if (rv == SQLITE_ERROR)
            return -1;

    id = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);

    return id == 0 ? -1 : id;
}
