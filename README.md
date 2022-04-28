# Quadratic Sieve implementation in C
Quadratic Sieve C99 implementation using [128-bit GCC extension](https://gcc.gnu.org/onlinedocs/gcc-4.8.1/gcc/_005f_005fint128.html), without GMP, also working on Windows.\
The source code is free and voluntarily obfuscated, if you want to appropriate the airy version you have to ask.

# Description
The ~350 lines C integer factorization software computes prime factors of a natural number up to 127-bit.\
This software is classified as a game, it's not well suited for professional use, sure because it's not laboriously tested.

# Algorithms in use
- First is tried a **Miller Rabin** primality test
- Then is fired a ~ 3 seconds **Pollard Rho** factorization
- Finally is fired a **Quadratic Sieve** factorization

This combination of 3 algorithms is fast on average, there is no known infinite loop.

# factor (positive_number, void *)

**factor** function is expecting 2 arguments, a number to factor and 33 Megabyte of malloced memory.\
**factor** function try to return to you one factor of the given number if it's not a prime.\
The function is presented in  `main.c`, which is a demo.

# Basic use
```c
#include "qs.c"
#include <stdio.h>

int main(void){
    void * memory = malloc(1 << 25);
    
    positive_number n = from_string_128_bits("108291528056611062333982283963");
    
    while(n > 1 && !is_prime(n, 64)) {
        positive_number res = factor(n, memory) ;
        printf("%s * ", to_string_128_bits(res)); fflush(stdout);
        n /= res ;
    }
    printf("%s", to_string_128_bits(n));
    free(memory);
}
```
Functions `from_string_128_bits` and `to_string_128_bits` are provided in `main.c`.

# Example output
```c
170141183460469231731687303715506697937  = 13602473 * 230287853 * 54315095311400476747373    took 0.1s
8243928541348384902759309418206720079    = 6193867823865156404384873 * 1330982316023         took 34.3s
159764310524856141862111460773005043181  = 18093538864531617269 * 8829909489847713049        took 30.4s
121379895327603193205768410338836433331  = 13890724788103667879 * 8738197407204819989        took 103.0s
128852603101275256030280062065703483477  = 8097364655504413063 * 15912906060576640579        took 182.4s
```
# Requirements
You must have an integer mode wide enough to hold 128 bits, and be able to : 
```c
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
```
Also if you want to run the `main.c` you must be able to :
```c
#include <stdio.h>
#include <sys/time.h>
```
# Compilation
You can download the files `qs.c` and `main.c` in the same directory then compile + execute :
```sh
gcc -O3 -std=c99 -Wall -pedantic main.c -lm ;
./a.out ;
```
Other possible usage after compilation :
```sh
./a.out 170141183460469231731687303715506697937 ;
```
Thank You.
