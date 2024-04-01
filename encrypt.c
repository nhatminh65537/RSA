#include <int256.h>

void encrypt() {
    long int pt, ct, key = e[0], k, len;
    i = 0;
    len = strlen(msg);
    while (i != len) {
        pt = m[i];
        pt = pt - 96;
        k = 1;
        for (j = 0; j < key; j++) {
            k = k * pt;
            k = k % n;
        }
        temp[i] = k;
        ct = k + 96;
        en[i] = ct;
        i++;
    }
    en[i] = -1;
    printf("\nVan ban da ma hoa : ");
    for (i = 0; en[i] != -1; i++)
        printf("%c", en[i] );
        printf("\n");
}

#include <int256.h>

void encrypt() {
    // Khai b�o c�c bi?n c?n thi?t
    INT256 plaintext, key, ciphertext;

    // Kh?i t?o c�c gi� tr? plaintext v� key
    plaintext = int256_c("Hello", ASCIIMODE);
    key = int256_c("Key", ASCIIMODE);

    // Th?c hi?n ph�p XOR gi?a plaintext v� key d? m� h�a
    ciphertext = pls(plaintext, key, zero);

    // Hi?n th? ciphertext
    show(ciphertext, HEXMODE);
}

