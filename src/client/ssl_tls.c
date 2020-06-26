#include "uchat.h"

SSL_CTX *mx_init_client_ctx(void) {
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

void mx_show_server_certs(SSL *ssl) {
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
        mx_print_error_endl("uchat: no server certificates configured");
}
