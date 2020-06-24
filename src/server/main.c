#include "server.h"

static void show_certs(SSL *ssl){
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
        mx_print_error_endl("uchat_server: no certificates");
}

void *mx_communicate(void *data) {
    t_comm *connect = (t_comm *)data;
    char buff[MX_MAX];
    char *status = connect->status;
    int bytes_read = 0;
    char *response = NULL;

    while (1) {
        if (SSL_accept(connect->ssl) == MX_SSL_FAILURE)
            ERR_print_errors_fp(stderr);
        else {
            show_certs(connect->ssl);
            bzero(buff, MX_MAX);
            bytes_read = SSL_read(connect->ssl, buff, sizeof(buff));
            buff[bytes_read] = '\0';
            mx_printstr_endl(buff);
            if (bytes_read <= 0 || mx_strcmp(buff, "exit\n") == 0) {
                close(connect->connection_fd);
                *status = 0;
                ERR_print_errors_fp(stderr);
                mx_printstr_endl("Connection closed");
                pthread_exit(NULL);
            }
            response = mx_process_request(buff, connect->clients, connect->db, connect->connection_fd);
            mx_printstr_endl(response);
            // SSL_write(connect->ssl, response, strlen(response));
        }
    }
}

void accept_clients(int socket_fd, sqlite3 *db, SSL_CTX *ctx) {
    int connect_fd = 0;
    unsigned int len = 0;
    struct sockaddr_in client_addr;
    SSL *ssl = NULL;
    t_meta *trd_data = mx_init_threads(db);

    while (1) {
        mx_printstr_endl("Ready for new client");
        len = sizeof(client_addr);
        connect_fd = accept(socket_fd, (MX_SA *)&client_addr, &len);
        if (connect_fd < 0)
            mx_terminate("Server acccept failed!");
        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, connect_fd);
        trd_data->ssl = ssl;
        mx_thread_manager(connect_fd, &trd_data);
    }
}

static int open_listener(int port) {
    struct sockaddr_in server_addr;
    int socket_fd = -1;

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        mx_terminate("Socket creation failed!");
    mx_printstr_endl("Socket successfully created!");

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    if ((bind(socket_fd, (MX_SA *)&server_addr, sizeof(server_addr))))
        mx_terminate("Socket bind failed!");
    mx_printstr_endl("Socket successfully binded!");
    if ((listen(socket_fd, 5)))
        mx_terminate("Listen failed!");
    mx_printstr_endl("Server listening!");

    return socket_fd;
}

static SSL_CTX *init_server_ctx(void) {
    const SSL_METHOD *method = NULL;
    SSL_CTX *ctx = NULL;

    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    method = TLS_server_method();
    ctx = SSL_CTX_new(method);

    if (!ctx) {
        ERR_print_errors_fp(stderr);
        exit(MX_FAILURE);
    }

    return ctx;
}

static void load_certificates(SSL_CTX *ctx, char *cert_file, char *key_file) {
    if (SSL_CTX_use_certificate_file(ctx, cert_file, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(MX_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, key_file, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(MX_FAILURE);
    }

    if (!SSL_CTX_check_private_key(ctx)) {
        mx_print_error("uchat_server: private key doesn`t match ");
        mx_print_error_endl("the public certificate");
        exit(MX_FAILURE);
    }
}

int mx_start_server(int port) {
    int socket_fd = -1;
    sqlite3 *db = NULL;
    SSL_CTX *ctx = NULL;

    SSL_library_init();
    ctx = init_server_ctx();
    load_certificates(ctx, "./certs/uchat_cert.pem", "./certs/uchat_cert.pem");
    socket_fd = open_listener(port);
    db = mx_opendb("test.db");
    accept_clients(socket_fd, db, ctx);

    mx_closedb(db);
    close(socket_fd);
    SSL_CTX_free(ctx);

    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        mx_print_error_endl("uchat_server: must take parameter - port to run");
        mx_print_error_endl("usage: ./uchat_server PORT");
        exit(MX_FAILURE);
    } else {
        if (!mx_check_port(argv[1]))
            mx_terminate("uchat_server: not valid port");
        mx_start_server(mx_atoi(argv[1]));
    }

    pthread_exit(NULL);
}
