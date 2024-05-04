#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "../Header/int256.h"

#define BITS 255

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

uint64_t mulmod(uint64_t a, uint64_t b, uint64_t m) {
    int64_t res = 0;

    while (a != 0) {
        if (a & 1) {
            res = (res + b) % m;
        }
        a >>= 1;
        b = (b << 1) % m;
    }
    return res;
}

uint64_t powMod(uint64_t a, uint64_t b, uint64_t n) {
    uint64_t x = 1;
    a %= n;

    while (b > 0) {
        if (b % 2 == 1) {
            x = mulmod(x, a, n);
        }
        a = mulmod(a, a, n);
        b >>= 1;
    }
    return x % n;
}

uint64_t first_primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
                            31, 37, 41, 43, 47, 53, 59, 61, 67,
                            71, 73, 79, 83, 89, 97, 101, 103,
                            107, 109, 113, 127, 131, 137, 139,
                            149, 151, 157, 163, 167, 173, 179,
                            181, 191, 193, 197, 199, 211, 223,
                            227, 229, 233, 239, 241, 251, 257,
                            263, 269, 271, 277, 281, 283, 293,
                            307, 311, 313, 317, 331, 337, 347, 349 };

uint64_t getRandom64() {
    int i;
    uint64_t a = 0;
    srand(time(NULL));

    for (i = 0; i < BITS; i++) {
        a |= (uint64_t)(rand() % 2) << i;
    }

    a |= (uint64_t)1 << 0;
    a |= (uint64_t)1 << (BITS - 1);

    return a;
}

uint64_t getLowLevelPrime() {
    while (1) {
        uint64_t candidate = getRandom64();
        int i;
        bool is_prime = true;

        for (i = 0; i < sizeof(first_primes) / sizeof(first_primes[0]); i++) {
            if (candidate == first_primes[i]) {
                return candidate;
            }

            if (candidate % first_primes[i] == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            return candidate;
        }
    }
}

bool trialComposite(uint64_t a, uint64_t evenC, uint64_t to_test, int max_div_2) {
    if (powMod(a, evenC, to_test) == 1) {
        return false;
    }

    int i;
    for (i = 0; i < max_div_2; i++) {
        uint64_t temp = (uint64_t)1 << i;
        if (powMod(a, temp * evenC, to_test) == to_test - 1) {
            return false;
        }
    }

    return true;
}

bool MillerRabinTest(INT256) {
    INT256 k,m,n;
    k = zero;
    m = n;
    while(m.value[0] % 2 != 1){
        m = imod(m, int256_c("2", HEXMODE));
        k = ipls(k, one, NON);
    }
    INT256 a = randint256();
    INT256 b = ipow(a,m,n);
    if(ieq(imod(b,n), one))
        return 1;
    for(INT256 i = zero; ile(i, k); i = ipls(i, one))
    {
        if(ieq(b,isub(n,one, NON)))
            return 1;
        else{
            b = ipow(b,2,n);
        }
    }
    return 0;

}

uint64_t getBigPrime() {
    while (1) {
        uint64_t candidate = getLowLevelPrime();
        if (MillerRabinTest(candidate)) {
            return candidate;
        }
    }
}

void printInt256(INT256 num) {
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
