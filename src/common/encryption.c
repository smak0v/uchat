#include "uchat.h"

char *mx_generate_key(char *str, char *key) {
    char *new_key = mx_strnew(strlen(str));

    new_key = strncpy(new_key, key, strlen(key));

    for (size_t i = 0; ; ++i) {
        if (strlen(str) == i)
            i = 0;

        if (strlen(new_key) == strlen(str))
            break;

        new_key[strlen(new_key)] = new_key[i];
    }

    return new_key;
}

char *mx_encrypt(char *str, char *key) {
    char *encrypted_str = mx_strnew(strlen(str));
    char x;

    for (size_t i = 0; i < strlen(str); ++i) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            x = str[i] + MX_CAESAR_KEY;
            if (x > 'z')
                x = x - 'z' + 'a' - 1;
        }
        else if (str[i] >= 'A' && str[i] <= 'Z') {
            x = (str[i] + key[i]) % 26;
            x += 'A';
        }
        else
            x = str[i];

        encrypted_str[i] = x;
    }

    return encrypted_str;
}

char *mx_decrypt(char *str, char *key) {
    char *decrypted_str = mx_strnew(strlen(str));
    char x;

    for (size_t i = 0; i < strlen(str); ++i) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            x = str[i] - MX_CAESAR_KEY;
            if (x < 'a')
                x = x + 'z' - 'a' + 1;
        }
        else if (str[i] >= 'A' && str[i] <= 'Z') {
            x = (str[i] - key[i] + 26) % 26;
            x += 'A';
        }
        else
            x = str[i];

        decrypted_str[i] = x;
    }

    return decrypted_str;
}
