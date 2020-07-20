#include "server.h"

    void mx_daemonize(int port) {
    pid_t process_id = fork();
    
    if (process_id < 0)
        mx_terminate("Daemonization failure");

    if (process_id > 0)
        mx_terminate("Working as a daemon now");

    umask(0);
    if (setsid() < 0)
        mx_terminate("Произошла неведомая хуйня в setsid");

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    mx_start_server(port);
}
