#include "server.h"

int mx_add_grp(sqlite3 *db, char *group_name) {
    sqlite3_stmt *stmt = NULL;
    int rv = 0;

    rv = sqlite3_prepare_v2(db, "INSERT INTO GRP(GROUP_NAME) VALUES(?1);",
                            -1, &stmt, NULL);

    if (rv == SQLITE_ERROR)
        return -1;

    sqlite3_bind_text(stmt, 1, group_name, -1, SQLITE_STATIC);

    if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
        return -1;

    sqlite3_finalize(stmt);

    return sqlite3_last_insert_rowid(db);
}

int mx_rename_grp_by_id(sqlite3 *db, int grp_id, char *new_name) {
    sqlite3_stmt *stmt = NULL;
    int rv = sqlite3_prepare_v2(db,
            "UPDATE GRP SET GROUP_NAME = ?1 WHERE GROUP_ID = ?2;",
            -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, new_name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, grp_id);

    if (rv == SQLITE_ERROR)
        return -1;

    if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
        return -1;

    sqlite3_finalize(stmt);

    return 0;
}

int mx_delete_grp_by_id(sqlite3 *db, int grp_id) {
    sqlite3_stmt *stmt = NULL;
    int rv = sqlite3_prepare_v2(db, "DELETE FROM GRP WHERE GROUP_ID = ?1;",
            -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, grp_id);

    if (rv == SQLITE_ERROR)
        return -1;

    if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
        return -1;

    sqlite3_finalize(stmt);

    return 0;
}

int mx_get_grp_id(sqlite3 *db, char *grp_name) {
    sqlite3_stmt *stmt = NULL;
    int id = -1;
    int rv = sqlite3_prepare_v2(db, "SELECT * FROM GRP WHERE GROUP_NAME = ?1",
            -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, grp_name, -1, SQLITE_STATIC);

    if (rv != SQLITE_OK)
        return -1;

    if ((rv = sqlite3_step(stmt)) != SQLITE_ROW)
        if (rv == SQLITE_ERROR)
            return -1;

    id = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);

    return id == 0 ? -1 : id;
}

char *mx_get_group_name_by_id(sqlite3 *db, int group_id) {
    sqlite3_stmt *stmt = NULL;
    char *grp_name = NULL;
    int rv = sqlite3_prepare_v2(db, "SELECT * FROM GRP WHERE GROUP_ID = ?1",
            -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, group_id);

    if (rv != SQLITE_OK)
        return NULL;

    if ((rv = sqlite3_step(stmt)) != SQLITE_ROW)
        if (rv == SQLITE_ERROR)
            return NULL;

    grp_name = strdup((char*)sqlite3_column_text(stmt, 1));

    sqlite3_finalize(stmt);

    return grp_name;
}
