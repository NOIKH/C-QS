# Quadratic Sieve implementation in C
Quadratic Sieve C implementation using [128-bit GCC extension](https://gcc.gnu.org/onlinedocs/gcc-4.8.1/gcc/_005f_005fint128.html), without GMP, working on Windows.\
The source code is free and voluntarily obfuscated, if you want to appropriate the airy version you have to ask.

# Description
The ~350 lines C software (integer factorization calculator) computes prime factors of a natural number.

# Algorithms in use
- Miller Rabin primality check
- Pollard Rho factorization
- Quadratic Sieve factorization

# Example output
```c
170141183460469231731687303715506697937  = 13602473 * 230287853 * 54315095311400476747373    took 0.1s
8243928541348384902759309418206720079    = 6193867823865156404384873 * 1330982316023         took 34.3s
159764310524856141862111460773005043181  = 18093538864531617269 * 8829909489847713049        took 30.4s
121379895327603193205768410338836433331  = 13890724788103667879 * 8738197407204819989        took 103.0s
128852603101275256030280062065703483477  = 8097364655504413063 * 15912906060576640579        took 182.4s
```
# Requirements
You must have an integer mode wide enough to hold 128 bits, and be able to include : 
```c
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
```
To run the main (demo) :
```c
#include <stdio.h>
#include <sys/time.h>
```

Thank You.
