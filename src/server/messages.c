#include "server.h"

// struct s_msg {
// 	int id;
// 	int group_id;
// 	int dialog_id;
// 	int sender;
// 	int recepient;
// 	char *msg;
// 	int time;
// 	bool edited;
// 	bool read;
// 	char *file;
// };

// {"type": "S_MES", "gid": -1, "did": -2, "uid": 2, "uid2": 1, "msg": "Hello I'm Geralt", "time": 3819524, "file": ""}




char *mx_send_message(void *jobj, t_comm *connect) {
    t_msg *message = mx_extract_message(jobj);

    if (!message)
        return mx_bad_request(NULL, NULL);

    // if (message->group_id != -1)
    //     res = send_group_message(message, connect->db, connect->fd);
    // else
    //     res = send_private_message(message, connect->db);
        
    if (message->file) {
        printf("hi\n");
        mx_recv_file(connect->fd, message->file);
        printf("bye\n");
    }

    return "foo";
}
