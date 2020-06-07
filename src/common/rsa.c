#include "uchat.h"

static long int cd(long int a) {
    long int k = 1;
    long int t = MX_T(MX_FIRST_PRIME, MX_SECOND_PRIME);

    while (true) {
        k += t;
        if (k % a == 0)
            return k / a;
    }
}

void mx_encryption_key(long int *e, long int *d) {
    int t = MX_T(MX_FIRST_PRIME, MX_SECOND_PRIME);
    bool flag = false;
    int k = 0;

    for (int i = 2; i < t; ++i) {
        if (t % i == 0)
            continue;
        flag = mx_isprime(i);
        if (flag && i != MX_FIRST_PRIME && i != MX_SECOND_PRIME) {
            e[k] = i;
            flag = cd(e[k]);
            if (flag > 0) {
                d[k] = flag;
                ++k;
            }
        if (k == 99)
            break;
        }
    }
}

char *mx_encrypt(char *data, long int *e, long int *m, long int *en) {
    long int pt;
    long int ct;
    long int key = e[0];
    long int k;
    long int temp[50];
    size_t len = strlen(data);
    size_t i = 0;

    while (i != len) {
        pt = m[i];
        pt = pt - 96;
        k = 1;
        for (int j = 0; j < key; ++j) {
            k = k * pt;
            k = k % MX_N(MX_FIRST_PRIME, MX_SECOND_PRIME);
        }
        temp[i] = k;
        ct = k + 96;
        en[i] = ct;
        ++i;
    }
    en[i] = -1;

    printf("\n\nTHE ENCRYPTED MESSAGE IS\n");
    for (i = 0; en[i] != -1; ++i)
        printf("%ld", en[i]);

    return "";
}

char *mx_decrypt(long int *d, long int *m, long int *en) {
    long int pt;
    long int ct;
    long int key = d[0];
    long int k = 0;
    long int temp[50];
    int i = 0;

    while (en[i] != -1) {
        ct = temp[i];
        k = 1;
        for (int j = 0; j < key; ++j) {
            k = k * ct;
            k = k % MX_N(MX_FIRST_PRIME, MX_SECOND_PRIME);
        }
        pt = k + 96;
        m[i] = pt;
        i++;
    }
    m[i] = -1;

    printf("\n\nTHE DECRYPTED MESSAGE IS\n");
    for (i = 0; m[i] != -1; ++i)
        printf("%ld", m[i]);

    return "";
}
