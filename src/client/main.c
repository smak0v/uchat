#include "uchat.h"

static void communicate(SSL *ssl, int socket_fd) {
    int bytes_read = 0;
    char buff[MX_MAX];
    int n = 0;

    while(1) {
        mx_show_server_certs(ssl);
        mx_printstr("Enter the string: ");
        while ((buff[n++] = getchar()) != '\n');
        n = 0;

        if (buff[0] == '1') {
            while (1) {
                bytes_read = read(socket_fd, buff, sizeof(buff));
                buff[bytes_read] = '\0';
                mx_printstr_endl(buff);
                char f = getchar();
                if (f == '2')
                    break;
            }
        }

        SSL_write(ssl, buff, strlen(buff));
        bzero(buff, sizeof(buff));
        bytes_read = SSL_read(ssl, buff, sizeof(buff));
        buff[bytes_read] = '\0';
        mx_printstr_endl(buff);
        // if ((strncmp(buff, "exit", 4)) == 0) {
        //     mx_printstr_endl("Client Exit...");
        //     break;
        // }
    }
}

static int open_connection(char *ip, int port) {
    struct sockaddr_in server_address;
    struct hostent *host = NULL;
    int socket_fd = -1;

    if ((host = gethostbyname(ip)) == NULL) {
        perror(ip);
        exit(MX_FAILURE);
    }
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        mx_print_error_endl("Socket creation failed!");
        exit(MX_FAILURE);
    }
    else
        mx_printstr_endl("Socket successfully created!");
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip);
    server_address.sin_port = htons(port);
    if (connect(socket_fd, (MX_SA*)&server_address, sizeof(server_address))) {
        close(socket_fd);
        mx_print_error_endl("Connection with the server failed!");
        exit(MX_FAILURE);
    }
    else
        mx_printstr_endl("Connected to the server!");
    return socket_fd;
}

int mx_start_client(char *ip, int port) {
    int socket_fd = -1;
    SSL_CTX *ctx = NULL;
    SSL *ssl = NULL;

    SSL_library_init();
    ctx = mx_init_client_ctx();
    socket_fd = open_connection(ip, port);
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, socket_fd);

    if (SSL_connect(ssl) == MX_SSL_FAILURE)
        ERR_print_errors_fp(stderr);
    else {
        communicate(ssl, socket_fd);
        SSL_free(ssl);
    }
    close(socket_fd);
    SSL_CTX_free(ctx);

    return 0;
}

int main(int argc, char **argv) {
    int status = MX_SUCCESS;

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
        // status = mx_init_client(argc, argv);
        mx_start_client(argv[1], mx_atoi(argv[2]));
    }
    return status;
}


// {"type": "REG", "name": "kali", "passw": "qwerty"}
// {"type": "REG", "name": "geralt", "passw": "qwerty"}
// {"type": "REG", "name": "BogdanUeban", "passw": "qwerty"}
// {"type": "REG", "name": "MrSkout", "passw": "qwerty"}
// {"type": "S_IN", "name": "kali", "passw": "qwerty"}
// {"type": "S_IN", "name": "geralt", "passw": "qwerty"}
// {"type": "S_IN", "name": "BogdanUeban", "passw": "qwerty"}
// {"type": "S_IN", "name": "MrSkout", "passw": "qwerty"}
// {"type": "S_MES", "gid": -1, "did": -2, "uid": 2, "uid2": 1, "msg": "Hello I'm Geralt", "time": 3819524, "file": ""}
// {"type": "S_OUT", "id": 1}
// {"type": "S_OUT", "id": 2}

// {"type": "N_GRP", "name": "TEST2", "id": [1], "tok":"MKK;KPK]KaKKKKK'B-KKK:XFKpDuK+KKOKKKKKKIS:O\/`KKKKXKKKKKX?KgKMzKKKK_<L(KKKCKKKKkKKKK,KPe?KK6KqKK(<K2KKHKKdKKKKaKKZKKKKlKQKNKJKK7KKKK$yKKrKKJ,K2KKKK!KKKKKKKKKK-KKKKK!KKJK:KKFKyK|(KyKHK1KKKWKKKKSKv<0KKgUKtKKKKKKVKDH&K`KKKKK7JKK)gKKKK\/KKeKoK]KK>KKKKKKqKrsK?fLK"}
// {"type": "INV", "gid": 1, "add": [2], "uid": 1, "tok": "MKK;KPK]KaKKKKK'B-KKK:XFKpDuK+KKOKKKKKKIS:O\/`KKKKXKKKKKX?KgKMzKKKK_<L(KKKCKKKKkKKKK,KPe?KK6KqKK(<K2KKHKKdKKKKaKKZKKKKlKQKNKJKK7KKKK$yKKrKKJ,K2KKKK!KKKKKKKKKK-KKKKK!KKJK:KKFKyK|(KyKHK1KKKWKKKKSKv<0KKgUKtKKKKKKVKDH&K`KKKKK7JKK)gKKKK\/KKeKoK]KK>KKKKKKqKrsK?fLK"}
// // {"type": "S_MES", "gid": 1, "did": -1, "uid": 1, "uid2": -1, "msg": "привет", "time": 3819524, "file": "", "tok":"MKK;KPK]KaKKKKK'B-KKK:XFKpDuK+KKOKKKKKKIS:O\/`KKKKXKKKKKX?KgKMzKKKK_<L(KKKCKKKKkKKKK,KPe?KK6KqKK(<K2KKHKKdKKKKaKKZKKKKlKQKNKJKK7KKKK$yKKrKKJ,K2KKKK!KKKKKKKKKK-KKKKK!KKJK:KKFKyK|(KyKHK1KKKWKKKKSKv<0KKgUKtKKKKKKVKDH&K`KKKKK7JKK)gKKKK\/KKeKoK]KK>KKKKKKqKrsK?fLK"}
// {"type": "S_MES", "gid": -1, "did": -2, "uid": 1, "uid2": 2, "msg": "привет", "time": 3819524, "file": "", "tok":"MKK;KPK]KaKKKKK'B-KKK:XFKpDuK+KKOKKKKKKIS:O\/`KKKKXKKKKKX?KgKMzKKKK_<L(KKKCKKKKkKKKK,KPe?KK6KqKK(<K2KKHKKdKKKKaKKZKKKKlKQKNKJKK7KKKK$yKKrKKJ,K2KKKK!KKKKKKKKKK-KKKKK!KKJK:KKFKyK|(KyKHK1KKKWKKKKSKv<0KKgUKtKKKKKKVKDH&K`KKKKK7JKK)gKKKK\/KKeKoK]KK>KKKKKKqKrsK?fLK"}

// "MKK;KPK]KaKKKKK'B-KKK:XFKpDuK+KKOKKKKKKIS:O\/`KKKKXKKKKKX?KgKMzKKKK_<L(KKKCKKKKkKKKK,KPe?KK6KqKK(<K2KKHKKdKKKKaKKZKKKKlKQKNKJKK7KKKK$yKKrKKJ,K2KKKK!KKKKKKKKKK-KKKKK!KKJK:KKFKyK|(KyKHK1KKKWKKKKSKv<0KKgUKtKKKKKKVKDH&K`KKKKK7JKK)gKKKK\/KKeKoK]KK>KKKKKKqKrsK?fLK"
// "KKKKKC5FKKMKKK(KKKKKKghKKK7KKKJKPKKKiKv^IKK&K,KSKKKKKK?KK;KKKKKhtKlsKKKKKKKKKKKKKKK2Kc]KKJKGhK4K@K]KiwKK]\/XKKKKKKK\\*KBKYKKKqKKKK?BlKKKKKKKeKH:KK-:KUMKoKKK`KKKKKKKIKKKcKP\\zKKKKKKK4KKKuKKKKKKKd;K~KqK@KKiNKKKKK|K'9KKB*K@KyKKKK7toKKQQKKKvGKKKKKKKKKKKKKKKKKKKKM"
