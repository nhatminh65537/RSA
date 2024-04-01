#include <int256.h>

void decrypt() {
    long int pt, ct, key = d[0], k;
    i = 0;
    while (en[i] != -1) {
        ct = temp[i];
        k = 1;
        for (j = 0; j < key; j++) {
            k = k * ct;
            k = k % n;
        }
        pt = k + 96;
        m[i] = pt;
        i++;
    }
    m[i] = -1;
    printf("\nVan ban da giai ma: ");
    for (i = 0; m[i] != -1; i++)
        printf("%c", m[i]);
    printf("\n");
}

#include <int256.h>

void decrypt() {
    // Khai b�o c�c bi?n c?n thi?t
    INT256 ciphertext, key, plaintext;

    // Kh?i t?o c�c gi� tr? ciphertext v� key
    ciphertext = int256_c("Ciphertext", HEXMODE);
    key = int256_c("Key", ASCIIMODE);

    // Th?c hi?n ph�p XOR gi?a ciphertext v� key d? gi?i m�
    plaintext = sub(ciphertext, key, zero);

    // Hi?n th? plaintext
    show(plaintext, HEXMODE);
}
