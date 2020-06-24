#include "uchat.h"

static void show_certs(SSL *ssl) {
    char *line = NULL;
    X509 *cert = SSL_get_peer_certificate(ssl);

    if (cert) {
        mx_printstr_endl("Server certificates:");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        mx_printstr("Subject: ");
        mx_printstr_endl(line);
        mx_strdel(&line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        mx_printstr("Issuer: ");
        mx_printstr_endl(line);
        mx_strdel(&line);
        X509_free(cert);
    }
    else
        mx_print_error_endl("uchat: no client certificates configured");
}

void func(int socket_fd, SSL *ssl) {
    char buff[MX_MAX];
    int bytes_read = 0;

    while(1) {
        show_certs(ssl);
        mx_printstr("Enter the string: ");
        bzero(buff, sizeof(buff));
        bytes_read = SSL_read(ssl, buff, sizeof(buff));
        buff[bytes_read] = '\0';
        mx_printstr_endl(buff);

        socket_fd = 0;

        // bzero(buff, sizeof(buff));
        // mx_printstr("Enter the string: ");
        // n = 0;

        // while ((buff[n++] = getchar()) != '\n')
        //     ;

        // write(socket_fd, buff, sizeof(buff));
        // bzero(buff, sizeof(buff));
        // read(socket_fd, buff, sizeof(buff));
        // mx_printstr(buff);
        // if ((strncmp(buff, "exit", 4)) == 0) {
        //     mx_printstr_endl("Client Exit...");
        //     break;
        // }
    }
}

static SSL_CTX *init_ctx(void) {
    const SSL_METHOD *method = NULL;
    SSL_CTX *ctx = NULL;

    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    method = TLS_client_method();
    ctx = SSL_CTX_new(method);

    if (!ctx) {
        ERR_print_errors_fp(stderr);
        exit(MX_FAILURE);
    }

    return ctx;
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
    int socket_fd = 0;
    SSL_CTX *ctx = NULL;
    SSL *ssl = NULL;

    SSL_library_init();
    ctx = init_ctx();
    socket_fd = open_connection(ip, port);
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, socket_fd);

    if (SSL_connect(ssl) == MX_SSL_FAILURE)
        ERR_print_errors_fp(stderr);
    else {
        func(socket_fd, ssl);
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
