#include "uchat.h"

int main(int argc, char **argv) {
    if (argc < 3) {
        mx_print_error("uchat: must take two parameters -");
        mx_print_error_endl("server IP address and port");
        mx_print_error_endl("usage: ./uchat IP PORT");
        exit(MX_FAILURE);
    }
    else {
        if (!mx_check_ip(strdup(argv[1]))) {
            mx_print_error_endl("uchat: not valid IP address");
            exit(MX_FAILURE);
        }
        if (!mx_check_port(argv[2])) {
            mx_print_error_endl("uchat: not valid port");
            exit(MX_FAILURE);
        }
        mx_init_client(argc, argv);
    }

    return MX_SUCCESS;
}
