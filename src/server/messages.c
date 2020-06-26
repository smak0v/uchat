#include "server.h"

// struct s_msg {
// 	int id;
// 	int group_id;
//  int dialogue_id;
// 	int sender;
// 	char *msg;
// 	int time;
// 	bool edited;
// 	bool read;
//  char *file;
// };

// static char *send_group_message(t_msg *message, sqlite3 *db) {
// //    int *group_members = mx_get_group_members(db, message->group_id);
//     int socket_fd = -1;
//     char *json_string = create_json_string(message);

//     for (int i = 0; i < GROUP_MEMBERS_LEN; i++) {
//         socket_fd = mx_get_sock_by_user_id(db, group_members[i]);
//         if (socket_fd != -1)
//             send_response(json_string, socket_fd);
//     }
// }

char *mx_send_message(void *jobj, sqlite3 *db, int fd) {
    t_msg *message = extract_message(jobj);

    if (!message)
        return mx_bad_request(NULL, NULL, 0);

    mx_add_msg(db, message);

    // if (message->group_id != -1)
    //     return send_group_message(message, db);
    // else
        return send_private_message(message, db);
// find target user in db

// create a chat if it does not yet exist

// if the target user is online send the message

// send the reponse to the client
}