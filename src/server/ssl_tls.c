#include "uchat.h"

void mx_load_certificates(SSL_CTX *ctx, char *cert_file, char *key_file) {
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

SSL_CTX *mx_init_server_ctx(void) {
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

void mx_show_client_certs(SSL *ssl) {
    char *line = NULL;
    X509 *cert = SSL_get_peer_certificate(ssl);

    if (cert) {
        mx_printstr_endl("Client certificates:");

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
        mx_print_error_endl("uchat_server: no client certificates configured");
}
