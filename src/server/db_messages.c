#include "uchat.h"

static void insert_to_table(sqlite3 *db, t_msg *m, char *str) {
	sqlite3_stmt *stmt;
	int rv = 0;

	if ((rv = sqlite3_prepare_v2(db, str, -1, &stmt, NULL)) == SQLITE_ERROR)
		fprintf(stderr, "Can't add msg: %s\n", sqlite3_errmsg(db));
	// sqlite3_bind_int(stmt, 4, m->time);
	sqlite3_bind_int(stmt, 3, m->sender);
	sqlite3_bind_int(stmt, 1, m->group_id);
	sqlite3_bind_text(stmt, 2, m->msg, -1, SQLITE_STATIC);
	// sqlite3_bind_int(stmt, 4, m->edited);
	// sqlite3_bind_int(stmt, 4, m->read);
	if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
		fprintf(stderr, "[2] Can't add msg: %s\n", sqlite3_errmsg(db));
    sqlite3_finalize(stmt);
}

// void mx_add_msg(sqlite3 *db, int group_id, char *msg_body, int sender, int edited, int read) { // need to edit!!!
t_msg *mx_add_msg(sqlite3 *db, char *msg_body, int group_id, int sender) { // need to edit!!!
	sqlite3_str *str = sqlite3_str_new(db);
	t_msg *m = malloc(sizeof(t_msg));
	char *string = NULL;

	sqlite3_str_appendall(str, "INSERT INTO MSG");
	sqlite3_str_appendall(str, 
						  "(GROUP_ID, MSG_BODY, SENDER)"
						  "VALUES(?1, ?2, ?3);");

						  // "(GROUP_ID, MSG_BODY, SENDER, TIME)"
						  // "VALUES(?1, ?2, ?3, ?4);");
	string = sqlite3_str_finish(str);
	// m->time = time(NULL);
	m->msg = strdup(msg_body);
	m->group_id = group_id;
	m->sender = sender;
	// m->edited = edited;
	// m->read = read;
	insert_to_table(db, m, string);
	return m;
}

