#include "server.h"

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

unsigned char *mx_generate_token(void) {
    unsigned char *token = malloc(sizeof(unsigned char *) * 257);

    if (RAND_bytes(token, 256) != 1)
        return NULL;

    token[256] = '\0';

    for (int i = 0; i < 256; i++) {
        if (token[i] < 33)
            token[i] = 75;
        else if (token[i] == 34)
            token[i] = 64;
        else if (token[i] > 126)
            token[i] = 106;
        else if (token[i] == '{' || token[i] == '}')
            token[i] = 70;
    }

    return token;
}

char *mx_hmac_sha_256(char *key, char *data) {
    unsigned char *result = NULL;
    unsigned int result_len = 0;

    HMAC(EVP_sha256(), (const void *)key, strlen(key),
         (const unsigned char *)data, strlen(data), result, &result_len);

    return (char *)result;
}
