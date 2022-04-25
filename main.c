#include "qs.c"
#include <stdio.h>
#include <sys/time.h>

double microtime() {
    struct timeval time; gettimeofday(&time, 0);
    return (double) time.tv_sec + (double) time.tv_usec / 1e6;
}

__uint128_t from_string_128_bits(const char *str) {
    __uint128_t res = 0;
    for (; *str; res = res * 10 + *str++ - '0');
    return res;
}

static char *to_string_128_bits(__uint128_t num) {
    __uint128_t mask = -1;
    size_t a, b, c = 1, d;
    char *s = malloc(2);
    strcpy(s, "0");
    for (mask -= mask / 2; mask; mask >>= 1) {
        for (a = (num & mask) != 0, b = c; b;) {
            d = ((s[--b] - '0') << 1) + a;
            s[b] = "0123456789"[d % 10];
            a = d / 10;
        }
        for (; a; s = realloc(s, ++c + 1), memmove(s + 1, s, c), *s = "0123456789"[a % 10], a /= 10);
    }
    return s;
}

positive_number exec_worker(positive_number num, void * memory) {
    positive_number res = factor(num, memory);
    if (res == num || res == 1) {
        char * str = to_string_128_bits(num);
        printf("%s * ", str);
        free(str);
        fflush(stdout);
        return res ;
    }
    return exec_worker(res, memory) * exec_worker(num / res, memory);
}

positive_number exec(const char *str, void *memory, int time) {
    double t;
    if (time) t = microtime();
    printf("%s = ", str);
    fflush(stdout);
    positive_number res = exec_worker(from_string_128_bits(str), memory);
    printf("1");
    if (time) printf(" took %.1fs", microtime() - t);
    putchar('\n');
    return res;
}

int main(int argc, char *argv[]) {
    void *memory = malloc(1 << 25);
    if (argc == 2)
        exec(argv[1], memory, 0);
    else {
        exec("170141183460469231731687303715506697937", memory, 1);
        exec("8243928541348384902759309418206720079", memory, 1);
        exec("159764310524856141862111460773005043181", memory, 1);
        exec("121379895327603193205768410338836433331", memory, 1);
        exec("128852603101275256030280062065703483477", memory, 1);
    }
    free(memory);
}
