#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "../Header/int256.h"

#define BITS 255
void genkey(INT256* p, INT256* q, INT256* n, INT256* e, INT256* d)
    
}
}

INT256 randint256()
{
    srand(time(NULL));
    INT256 result;
    for (int i  = 0; i < MAXBYTE; ++i)
    {
        result.value[i] = rand();
        printf("%2x", result.value[i]);
    }
    printf("\n");
    return result;
}



void printInt256(INT256 num) 
{
    for (int i = MAXBYTE - 1; i >= 0; i--) {
        printf("%02x", num.value[i]);
    }
    printf("\n");
}

int main() {
    // uint64_t bigPrime = getBigPrime();
    INT256 result;
    result = randint256();
    int i;

    printf("Prime number in Int256 format: ");
    printInt256(result);

    return 0;
}
