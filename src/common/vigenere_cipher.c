#include "uchat.h"

char *mx_generate_key(char *str, char *key) {
    char *new_key = mx_strnew(strlen(str));

    new_key = strncpy(new_key, key, strlen(key));
    for (size_t i = 0; ; ++i) {
        if (strlen(str) == i)
            i = 0;

        if (strlen(key) == strlen(str))
            break;

        new_key[strlen(new_key)] = key[i];
        mx_printstr_endl(new_key);
        mx_printstr_endl(str);
    }

    key = new_key;

    return key;
}

char *mx_encrypt(char *str, char *key) {
    char *encrypted_str = mx_strnew(strlen(str));
    char x;

    for (size_t i = 0; i < strlen(str); ++i) {
        x = (str[i] + key[i]) % 26;
        x += 'A';
        encrypted_str[i] = x;
    }

    return encrypted_str;
}

char *mx_decrypt(char *str, char *key) {
    char *decrypted_str = mx_strnew(strlen(str));
    char x;

    for (size_t i = 0; i < strlen(str); ++i) {
        x = (str[i] - key[i] + 26) % 26;
        x += 'A';
        decrypted_str[i] = x;
    }

    return decrypted_str;
}
