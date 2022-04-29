#include "qs.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// fill the given array with prime factors of n, result will be zero terminated (convenience function).
positive_number * exec(positive_number n, void * memory, positive_number *array) {
    positive_number a, b; size_t s ;
    do  if (n < 4)
            *array++ = n, n = 1;
        else if (n & 1) {
            for (b = n >> 1, a = (b + n / b) >> 1; a < b; b = a, a = (b + n / b) >> 1);
            if (b * b == n) {
                s = exec(b, memory, array) - array ;
                memcpy(array + s, array, s * sizeof(positive_number)) ;
                array += s + s ;
                n = 1;
            } else {
                if (is_prime(n, 48)) // number of Miller-Rabin tests.
                    *array++ = n, n = 1;
                else {
                    a = factor(n, memory); // call the factor function.
                    if (a == 1 || a == n) *array++ = n, n = 1;      // fail.
                    else array = exec(a, memory, array), n /= a;    // success.
                }
            }
        } else
            for (; !(n & 1); *array++ = 2, n >>= 1);
    while (n > 1);
    *array = 0 ;
    return array ;
}

// provided for convenience, take a string and return a 128-bit unsigned integer.
__uint128_t from_string_128_bits(const char *str) {
    __uint128_t res = 0;
    for (; *str; res = res * 10 + *str++ - '0');
    return res;
}

// provided to print 128-bit unsigned integers.
static char *to_string_128_bits(__uint128_t num) {
    static char s[40];
    __uint128_t mask = -1;
    size_t a, b, c = 1, d;
    strcpy(s, "0");
    for (mask -= mask / 2; mask; mask >>= 1) {
        for (a = (num & mask) != 0, b = c; b;) {
            d = ((s[--b] - '0') << 1) + a;
            s[b] = "0123456789"[d % 10];
            a = d / 10;
        }
        for (; a; ++c, memmove(s + 1, s, c), *s = "0123456789"[a % 10], a /= 10);
    }
    return s;
}

int main(void){
    // allocate memory for the factorizer.
    void * memory = malloc(1 << 25);
    // allocate memory to store 128 factors.
    positive_number * factors = calloc(128, sizeof(positive_number)), n = 0, mask = -1;
    unsigned sr = (size_t)factors; sr = -sr / 1105; printf("srand at %u :\n\n", sr); srand(sr);
    for (int wrapper = 16, count = 0; wrapper < 120; ++wrapper)
        for (int n_bits = wrapper, j; n_bits <= 125; ++n_bits) {
            // generate a random number of ~ n_bits bits.
            for (size_t k = 0; k < sizeof(positive_number); ((char *) &n)[k++] = rand());
            n &= mask >> (8 * sizeof(positive_number) - n_bits); n += !(n & 1);
            printf("%5d. (%3d bits) %40s = ", ++count, n_bits, to_string_128_bits(n));
            fflush(stdout);
            // fill the "factors" array with the prime factors.
            exec(n, memory, factors);
            // iterate over the factors (zero terminated array).
            for (j = 0; factors[j + 1]; ++j) {
                printf("%s * ", to_string_128_bits(factors[j]));
                assert(n % factors[j] == 0);
                assert(is_prime(factors[j], 72));
            }
            printf("%s\n", to_string_128_bits(factors[j]));
        }
    // release memory.
    free(factors);
    free(memory);
}

// You can put it into a primes.c file then compile + execute :
// gcc -O3 -std=c99 -Wall -pedantic primes.c ; ./a.out ;
