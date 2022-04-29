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

This combination of 3 algorithms is fast on average, the factor function has no known infinite loop.

# factor (positive_number, void *)

**factor** function is expecting 2 arguments : a number to factor + 64 MB of malloced memory.\
**factor** function is intended to return to you **1**, **N** or a factor of the given number.\
The **factor** function is presented in  `main.c`, which is a demo.

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
You can normally [Try it Online](https://tio.run/##vTxdb@M4ku/5FexedMaK7USftjzu9GCxuIcDZu9hd96CrCHbsq1EltOSnO75yP30m6siKVWRktLpw@ICJJHJYrFY3yxS3kz3m82ff8mKTX7epuJjVW/zbH19@HTB28qs2Ntt26yozbZjUh@w5eIv23SXFan423/@1y@rf/7trz//hxh54uNH4fnOxUX961MKAGK1OgMKz49XtXg6VVmdPaer4nxcp6VYEhjMft7U4vcLAT823HOSn9NK3Pn3S9m/OSSluBK7ZFOfymr1nG7q5cWLqI6nU33Qg1bQdFHVSZ1txOZUVLWost9SIGID5KzSr0@nIi1qcSsCb2nC5adir/4ApAJfJ1UKoN7PP@P6DAxDazCxbJM6uSNG6XVoirJim37FBSBiwPd8yrZqku3pS@FnI3wWV2pdjsaf7cRItUw/SQTi9paJwhFlWp/LQk3UAFpkiKnw7sWnT7AyBbc7lWLUUCU4PgD1l9AGsMasSzGdZg1NnakyMcYZ/rglWlX7vbgUnoPMHJnyAIqc5QAyg9SXDhckkEHcPfLEdcR4bNJ88cKZfH76d7DYZp3FpUx8ZEOXQNJrXJtqrnXX/3386uE72qcD3P/vVrHfzk9JlwMS77JTW1BW5OgSiLlZkdXE3Em/aTecOKbHKq1HEtCdSPM47eRox2Fchv5WIAZTJbKlkP2SekkpKYMYGVbpjOQA8YGM3JkoJOLmljXKFbbOYHM6PiVlOlLeQq0tP1QTwRvKQ9WqEXwuwX3B9DipGEEfMTRvP@gloggkOKiuuLxkAOKdoX6t1G3VI/xL8W5kTC2RNaowVZD6o@PgbKCXpqZqqpSq4zos6zme829yYtI8ppUlamgZEjXyga3dMD3xxx9slZZZtnwB5L16IgHSHFz67/2wHrgtNvVYGBKzMXFPPhEPE/GIMmjxTUS@NEVly6jHN0zgzyN3EO3gHAY/wjTwL2c4Hjo4csTx4BgY2pVKmef3YqyxyM8P9xBUuUosu9M/WCY3nT5gTKDlLm2qzTkflG8b3/K2e3EjLHc0MPpefLgdAn256D4xsU75lLwd6QHLchs/aKh3dV43weFVGzeVlcsCrars7@Ks6mBgY0DbSUoEdC8533bQgHtbBmYykoh2tbb@9hiKIX4Z7ttptcA7Ahsl0tcavubW0CywqQTCPwbnnyzIMfe84kebUk1GshQ2/mSiJjba9RSDM@AEE5nD9E7zikzf9YpEWp7t0qVeKY9DQVGqk@v0al2y3b5F6wbF@oo43yDGHvGN@8T3ibER85COKKf9ohwQ4XhAhD3TDM5iixOtCr05ulgmFi5X5e4zK6xts@c3h7XPbXxrBENSLknGXWkZ6YScAYW7tFB8JhS4nMT2GglwyO2NFUwN8tfjTZ/TlihaMqW/BapVCt4H31ItE@rSWfZCDO1CQPji0hTsT4PAf4DMfhzsheTWG5ycgkHjNgcCgf2DrDYEVipxoRm5Q/xoeSLjSMkF3PfzmRYP2X/SD/hy8XqL@ekz6bmR0hhMM/IbTyz7MYCBNgR@NnZYHS93VcJ0aDMalW62R9s7TKkGnmWJ1eeyNkxR7Rxam4M9f8N9vQkxDU1VCcg3ohybNim5FgD9wMjaIohdfjqVI0nEaHs6r3PwZOTUeLLKlnJr7f/ItzLPAISTZVvpqFxb8vRUnpBJv7svoDnEZdVx3fCQzzy2JsYto9cZKaXAsdw3y3/D6l/ZbztMt5ElI2v/PDaSH1kCaLY5o2HSrm61jKStEfMNLX1tZUb8WJqyQEffaNSlGgi8lrK55D6M4rLuVLA2hK7YSK00eYFNbCNnyBASPOPzO9swGETPCqdCIVf@jOaRCaLv9CU2piczVoQ505KU70r1voFZk0EmTbrM4YmPtlu7NgBbyzp7yrMN9J6K1fG0PeenkQ2VdIsK6yZUdzCets5AjVFtkyF5qI9PS3FzI76kIpFe6PSclmAHX8AihKQhsUm7prRmjRsUgJLpDUY0D8LYGuUAjSgomOYn@XeqmmTCgp/Bu67VJx1kIRcCWoCotUPj1zAaW/noNY6Fxs5WXXyQpBj1GUgHqtVTmR3TDieLiTgXVbYv0q14HOJToriELJ6I7USkE7GbiP2yLXUcJphMtR6gQAtXdg7PaPJN3UzILp9gC9C8cDZbLHyABwnsEvT5KMj061Mp@VsdkzwXkvzqmu3yJL6RngD@jCSymQMb2tAHt@N@9d34r34c@9IjFsCZQLkdOekC7WuErXNsxAfPa5@C9ol6F82T3/b6ixbhYua1GIMWUdAOD9u2sB0etr1R2xa108zaEbN5O03khjTPvIWYt@Pn7fi4bYvbtgUtx6XVurRcl6byvDimuTx3QXwieJ8QBox9rJWGhfQYEWxEM/qzaEZy8WY0zYwQzplsCGFMCBfskWDZyhcktTCMfFqjTxrgMyHTGn2fKQEB0Bp9ErMfsUda42zhsTX6tEZ/RlhIsP6cJqc1@jFTwfYxcFvYQK1EaeZ8wWYMSHgBKXfAVJaEF4TssaUuIGUNSFsDpqQgdM9nU5LIAhJZQGsISEEDklNIGhqS8oVkh6FPi/TiaM4sMAyYuZG9kaBCZoW0tDCi2cn8QhJSyFYJyhAyYwxpaWFMaEgbw0ULEJHFRbS0yPfokQBCWqUPjtIlxkbMgRDlEZEbkU5FpFMR6VRElEYL5oPIQoKZ57u0yhkJZUbqNiOlmnmslXwZyWMWskcaFhJjQ38WslXOSN9mzDty90gAtLQZ6deMhDB32SOtMgKjnHHH2s4zJ@ufkybNSZPmtIg5mfyc5DEnSuczNuViHjEjmTMXTkKZkznEpCgx@anYZ4/k8InomMWouRssmLuLaREx8TgmymNid0xKFROPY3JJMQs3Mcky9kGYtMoF6cyCFrEgdi/Iwy7IES1IZxak8wsiesHschH6MbPLBSn@gihfELkL0o4Fj48s4Lks4rkea@ceyJ35XIcgqvIIyxCwkOiymOiyoOjO@DMbO2PwMY/VC9c3ozVDwMKgy1bI474R3lnS4/kMxmcwEVt5AJ6QGSx0skGMeo9E4fGI7cVsQkasz3IUnycdHlt5uIjmzI48Frk9Fq9hIQwZS1B8JgI/Ys9zBsMyDR171eTghXgCAZ0MkInAZ2wP2KoCJgIWviGXIjyBz@D9OQ95fsAnD5jSBCxjYvHXCxjbgwXLzZj8Q8bBkCV4IXMlvhsFYczYzsKsF/KkL2QImIaHTEdCxvaQ6UgY8@eARV7QUZ4psoDrhTHPONkkC55@svyTrTZiOhKFrD1kKw9BX7m2R2wlEVtJxETAoi88M5iYjWU6wsKsN2M5uR/FsZGVz5jcWBT2WOz1ZkyBZkyeM2YFs4g988SbJzzzOIq4h5sxbzRjrJ7xlTC2zxmxc0bUnImABVlvzlKfwI18l7N9zhRrzkQwZyKYMxueM1azCAvPbBfB2B6zFDoIIN3nTobFWi9mShMzomJmjjHThZjpQsxMk8VVeGYZWASpDLfzmFG8YBSz4AqbILb1CfiOiLUzz7dgprmYk6lBJj/jWS50skEx31QxxMZmi7Y1Lnk7cO0ee6ZNjss8HFjajIdy6GTI2K7LjRjiGX9mMGz75LL9kxuzyWO@NXSj2OWTs5V4jHq2HfZZiPTZhthnO2LfY3tEL2Rjo8D5d/0seXnqVmzwdhIWFSfiIMvi4h20Y@FkiZUqVXoajw@OcVr3OJ0apz6yea/G74FD@gaHVS4ClKORuq7niMvEuduPx/IiDrY5ZVJsR0AfFnvw@XTUxeV2lu1JTbSFMamuUSd4YWcidriMJXZ9uBU7eXlltMPnrbNkpdcvhyxPEeqTkJze4YNj3chA9Dt59gD/Pt6Ktfz/8bYLiR3qCEzgoP4S5VZWxgoHibwEniJpw9CphraL51tdOducijorzmnPLZLDUmTTKeLfYs148xaiOrPgwLYm5/LbYLpNncj01FJXT6cvnSri1249Nv361G0sXq/E8huJX1GsxXKJiBoF/Tq4UCDgq1yosU4Yah/S6WZVM1Sz9mOUt5Qkzs5J17pMk8feK1I9N@KujulxVdVlku0P9Ug1PdV0aiztBAlZrZK6LrP1uU5Xq9EoyWVRdhR5fqsl@lpXY1uABo9vAAIse4RXbqFlVcsOB3fxnt9Lpc18ddCyKg@nUX8JvZiY12mrTZK3F/h0G9aIpx4Wpm@Fr48AbTxYPdYGrdyAKiXrw2/o2SqhX2g3YJ55j8cJWsfaFik4kjo7pqdzLfYwWSHWv4r6kCoqRVLuz8e0qK87apBghV0vpa1Om8egSO0G1yT9wsS@RUOnrptBPdrIonlh@5TxGDvwmGuDxggcRldXKBe3AX@1FT/B/6nAej8eAoCdK1eXKreXKhfYuL2XxukB0B8S6JY8mV7d7hXpD0leH/y0qnwqfx08JVAnBNKzI/91M2hnVgHnnpIyOaY1tNQn2f/5nGxLaS9Vlj6n11ybtoof3kydNEzkCYO6aDcR@UQc6cKEAPLAVOuJOAOdqr090pCnmlfiuMLzJ4CCx3KFdqPvzJi3x8XV11UFZJUpzH9ldFb8FvrfV6AL8NeHv7/ALx4zrsC60nqF3Fxa12oxl3DsgwvVJZmlobKq@KE5us6qZJ2n12YQ13B4uqtUEVqbs19xI4@M8OAXuj5iNMMhySuAGPBTrSEM/RRGXDX3EHARa7xMndCRtb2SRJirQFmn5S7dgKOQzLxuUbXHUI1epc6bGKNOf1p2jGTcdnXg9mK8zrLjbgHNkXm1Zjb5psLOCobK/tRMYFjyo2een2rKNvZC1RSgZuBxYJ5rFkYBbp3us8LW8ka@v7FDRKmk6qRd7PDwLz/tc30pwTyA16Q3TnRk6DlkDWmW5xjecnl8n48A25XCtntyHBC934SSmxt1JVdiku9viDcbLjPXrdSkQObI2xZX4HSRBd9G1tCkjBU9Kg@d2vlMGgu@bQBZ0oAXZf2lyMEAtvhv3NxwUIeQhtxbXQR34rvSqTbpjab5A3oaGCsvoCmLybVf7ckqaFDfxSCECOSBKNISOE7vNR/Ua5MIScBYE@AjAd0LQtY9ks7JP2LNVfKfNcn/Tif/O0r@s4FLSm3WD5xFbjYE7nuYA7aTozXuv4WLIdnhyEn/2ncoPdyx4BFyM4u6bW3DIimKQZAiD962gu3PT4oj/eEaiJH3nfF9jENz9ZARKxX8Qa71R4xLuSMvOTqDE8oVH9QB@sPgtM0MML2vl9gva@PnQfoq5PdBI9doHjSaHgTdm2RXyqDGY7khWr7aj0y2YV7MqGANQxv0zQEmQjudQjcy0mOnjY2j7woU5IPME8ciUh8fKSUjEwDfo9yFOCZFsk8xAbzgRvYAfnFLM0NA77gb7YXGIkBYhwP7NjCOHxuRRb/rIDMFF5Xb7P6li8G3QNpUxAb9BQAfGKCZp9jQhAYsl99ckpmMcqRW/mKjsCYwZ0@1OEYB7kK2uO8ImLxzuYm/ahK2virCcUlrvdvfX/NX/AAKSJwIrUjpxPItNq6HpcWO70H4cmG4TDRafZ3WwZs9jwq/7OpURVJFQ2qvJ72/Vm8y3smLjqO@HvdeobySHqfxZ9KoKUNjSmWJSypYbjMlVS5nr/51YqakdIuuI217gR@B6ciMPVjzs5Z@YiquGisdiz53v1Z55w2H8x3UlDXoiNmIN3AdZIANa8yBvmc5fGV5rTLfI/6DpRionNfiYxsUSTgJF5tFrcPlawDeWLO@FlUNFFxDQTHaYhnSdmkUpOycYoASTFIwo5E6@9pFZctaHi11HeyWOts3uXuPcuzvabV7OK6Bim@eftUO9RE972SYVRPDL9s/4FLvEIN8GWDrcK4iW14bOh4//h/vYbfb8L34120bA98ZiuE5TtfjaHvdKV3cNda5c7q@Lm19HfquTiIKvLvDTS965d39UDK6R6LSIdXQYoDNrZLEXkliiGeG1DSs@pB@x0AN@8s3wSVZ/neQ5XOy/O8gy/8mWS8XgzluirIeltX3ycyM23jN/taUDb6d0ralZn4znJRCsqw3S4fq7sBtRDf8C18UgAeHnw/0/TQktbweBi0Z6P8zpV1xQQTsCV5U4jVHtrb60NrqO9SWQ6mVeAvrmU4hqHpSX6QJ22/3XD5PzA19i3Wn6jUexmd5QV55crwJTi8YOPgql2G6DHWpMdj5wI7ygRG@kJmqXSUyv9fbkGb6oJlDWpkM7miSiR1bdiwSWOvvvDqj1lK9gsO7HxqLbycjG3D45bkHCvOgsxh6kZReQJFIaqfn5YFLvOf@jOitNwYkqzfkymdyh4075w2WZcvr7stL0@nuHkvBWGI23vPnEw8N9u6tuWXRD1@fg21rgmVj2J1iGVkXApvi2nopa2RrVa/H07W1Atg4PYdBCTDrxfwqkezU@daQN327SFJVaSm/XeQCb3tneS6rQKpwn5Rl8qv4ktUHVfBrvu1DnHZYGoeUF1QN@mHQOhW/peVJ1Gl5zIqkTrfXncr2lUi/ppu@K@@qnK33iN1jqitJyGt17mV7FCLa0wq6z25akcImd7swdUHnXG3tRg287NaVqPTbqfq@peBLMJYVyLIuMGAtnUqfYaODltwDvWjYpLky1XLqGpYOm6p7jFV5@QhWDHMNHBY7PXg0gluBm82qB8BiIjub6092qO48EWHsOKyMC6r1d1CotJz@I1lnBShUVVfXvbHjVUG@oRiXtFVppEPXM2WhdJNoO9Dvpe3OxYbqw30rsgOD00tdWw3ZJVnej0zSqxiuJZ7YEkdsN/hqokZWnTebtOph0tA7iT0VIn2w/k7r/ZKT70@kvlMQ0zl1wY7xrxqSXWGccTWq@SL9y1N5es626VZOtzkV4GSytNhAdK6TR1i2UC5KJMWWfJ3nx9N1VtMhEjjldJ@W1xf8e4125UmWSGD4CltgRNW8sihTtIoOefk4/c6Rcc8CYZe6B/9eCc/FzW8li2RT8YP7Q/9XcvAl1id0mkU9SD8IrPnOI0lgferQzwkF@7DesZTDqrvQ1UGHQx@T6lGeABsvo@sjQSErzDrLgznRSYBzeO@@5/mPRIHvOOH/G4yZ@KT@anWwqy6J/rYb0CGEUoUEFdM2GORszyZri6Pqbjpd32u@6i/HGdtv3lZ3a8wx37ueH4TRbB4v3t9t5QVa6ysyElkAu8HzqJ6DYXrvXZ74gmEdT8/pCB2bJ88tNw4eNdozJWomdOY3eNTVF5Wr9itqjklWyKsFjlquEd4wQwP3ctqoL@ryI42rN7ah2@jV7PeLb/14Wphkscpg5fcmcAc8C41yTP9VkI6fZFFAqvlu9P4DWsp7sOWuIuO3zuDpxi4/V4cRJCenM8@npD/DiThbCW0/zvYaza5M0@Y4ChOjP/9ns8uTffXnND/@Lw) with DigitalOcean, and ask [Number Empire](https://numberempire.com/numberfactorizer.php?number=9999999999999999999999999999999991) for verifications. Thank You.

Shortest link to this page : [bit.ly/C-Factor](https://bit.ly/C-Factor)
