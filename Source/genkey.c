#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_BITS 128

// Hàm sinh số ngẫu nhiên 128 bit
void generateRandom128Bit(unsigned char *buffer) {
    int i;
    for (i = 0; i < NUM_BITS / 8; i++) {
        buffer[i] = rand() % 256; // Sinh một byte ngẫu nhiên trong khoảng từ 0 đến 255
    }
}

// Hàm chính
int main() {
    // Thiết lập seed cho hàm rand() dựa trên thời gian hiện tại
    srand(time(NULL));

    // Khai báo mảng để lưu trữ số ngẫu nhiên 128 bit (16 byte)
    unsigned char random_number[NUM_BITS / 8];

    // Gọi hàm generateRandom128Bit để sinh số ngẫu nhiên
    generateRandom128Bit(random_number);

    // In ra số ngẫu nhiên đã sinh dưới dạng hex để kiểm tra
    printf("Số ngẫu nhiên 128 bit (dưới dạng hex): ");
    int i;
    for (i = 0; i < NUM_BITS / 8; i++) {
        printf("%02x", random_number[i]);
    }
    printf("\n");

    return 0;
}
