#include "server.h"

static t_profile *init() {
    t_profile *profile = malloc(sizeof(t_profile));

    profile->user_id = 0;
    profile->name = NULL;
    profile->birth = NULL;
    profile->email = NULL;
    profile->status = NULL;
    profile->country = NULL;
    return profile;
}

static t_profile *take_profile(sqlite3_stmt *stmt) {
    t_profile *profile = init();

    profile->user_id = sqlite3_column_int(stmt, 0);
    profile->name = strdup((const char*)sqlite3_column_text(stmt, 1));
    if (sqlite3_column_text(stmt, 2) != NULL)
        profile->birth = strdup((const char*)sqlite3_column_text(stmt, 2));
    if (sqlite3_column_text(stmt, 3) != NULL)
        profile->email = strdup((const char*)sqlite3_column_text(stmt, 3));
    if (sqlite3_column_text(stmt, 4) != NULL)
        profile->status = strdup((const char*)sqlite3_column_text(stmt, 4));
    if (sqlite3_column_text(stmt, 5) != NULL)
        profile->country = strdup((const char*)sqlite3_column_text(stmt, 5));
    sqlite3_finalize(stmt);
    return profile;
}

t_profile *mx_get_profile_by_id(sqlite3 *db, int user_id) {
    sqlite3_stmt *stmt = NULL;
    int rv = sqlite3_prepare_v2(db,
            "SELECT * FROM PROFILES WHERE USER_ID = ?1", -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, user_id);

    if (rv != SQLITE_OK)
        return NULL;

    if ((rv = sqlite3_step(stmt)) != SQLITE_ROW) {
        if (rv == SQLITE_ERROR)
            return NULL;
        sqlite3_finalize(stmt);
        return NULL;
    }

    return take_profile(stmt);
}

int mx_change_profile_by_id(sqlite3 *db, t_profile *profile, int user_id) {
    int ret = 0;

    if ((ret = mx_delete_profile(db, user_id)) < 0)
        return -1;

    if ((ret = mx_add_profile(db, profile)) < 0)
        return -1;

    return ret;
}
