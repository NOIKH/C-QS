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

This combination of 3 algorithms is fast on average, there is no known infinite loop in the factor `qs.c` file.

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
You can normally [try it online](https://tio.run/##vTzbbuO4ku/5CnYPOmPFdlp3y@NODw4G@7DAnAV2z7wFOYZsy7YSWU7LcjpzyX76zlaRlKpISZn04mADdEehisW6V7FIez3drdd/fpdvy022Ff/5j@VPF9/BU15m6o@L7/JyXZw3mfh0qjdFvrrefzbGqrzc2WObvKzNsUNa73GkRf7Tv//HL8t//PS3n/9NjDzx6ZPwfOfiov71MUM6lsszoPD8ZFmLx@Mpr/OnbFmeD6usEgsCg9XP61r8fiHgx4Z7SotzdhK3/t1Cvl/v00pciW26ro/VafmUrevFxYs4HY7Heq8nLWHo4lSndb4W62N5qsUp/y0DItZAzjJ7fjyWWVmLGxF4CxOuOJY79R9AKvBVesoA1Pv5Z@TPwDDEg4llk9bpLQlK86EpykFhz8gAIgZ8T8d8oxbZHL@Wfj7CZ3Gl@HI0/nwrRmpk@lkiEDc3TBWOqLL6XJVqoQbQIkNMhXcnPn8GzhTc9liJUUOV4PgA1F/AGMAaqy7EdJo3NHWWysUYV/jjhmhV43fiUngOCnNk6gMochYDyAxSXzpSkEAGcXcoE9cR47FJ88ULF/L58V8hYlt0lpRy8YlNXQBJr0ltqqXW5f/b5NUjd/RPB6T/361hv12eki4HNN4Vp/agvCwwJJBw8zKvSbiTftduJHHIDqesHklAdyLd47iVsx2HSRnetwoxhCqRLYR8L6mXlJIxiJHhlc5IThAfyMmdiUIiPt6wQclhGwzWx8NjWmUjFS0Ub8X@NBF8oNqfWjOCvysIX7A8LipG8I4EWrR/aBZRBRIcTFdcXjIA8c4wv1brtukR/oV4NzKWlsgaU5gqSP2n4@BqYJempWqqlKkjH5b3HM7FX0pi0jxmJ0vVMDKkapQD491wPfHHH4xLyy1buQDyXjuRAFkBIf33flgPwhZbeiwMjdmYeCSfiPuJeEAdtPgmoliYqrJ11BMbJvDfAw8Q7eQCJj/AMvCrYDjuOzgKxHHvGBhaTqXOizsx1ljk3/d3kFS5SSy6y99bLjed3mNOIHYXNtXmmvcqto1v@Nid@CiscDQw@058uBkCfbnoPjG1TvmSfBzpAc9ymzhomPfpvGqSw6s@bhor1wV6VdX/iouqg4HNAWsnLRHQnZR8@4Im3Nk6MIuRVLTc2vbb4yiG@mW6b5fVCu8obJTKWGvEmhvDssCnUkj/mJx/tCDHPPKKH2xKNRnpQtj404la2BjXSwyugAtMZA3Tu8wrOn3XqxLpeXZIl3alIg4lRWlOrtNrdelm8xarG1TrK@p8gxp71DfuU99nJkasQzqqnParckCF4wEV9iwzuIqtTvQqjOYYYplauF5VuM@ttLbJn96c1r60@a1RDGm5Ih13tWWUE3IFVO7CQvGFUCA7qR01UpCQ25srmBkUr@ebvqAtUbRkyngLVKsSvA@@pVoW1JWz6IUY2oWA8sWlqdgfB4H/AJ39MPgWiltvcHFKBk3YHEgE9g@K2lBYpdSFbuQOyaOVicwjFVdw388XYh6q/7Qf8OXi9RHzry9k50ZJYwjNqG88sejHAA7aEPjF2GF1otxVBcuhz2hUetiebe8wpRl4lieevlS14Ypq59D6HOz5G@nrTYjpaKpLQLER9diMSc21ABgHRtYWQWyL47EaSSJGm@N5VUAko6DGi1XGyo21/6PYyiIDEE6ebZWjkrf08bE6opB@d1/AckjK6sV1I0O@8thaGLeMXmem1ALHctew/wbuX9lvO8y2USQja/88Noof2QJotjmjYdKubrSOpK@R8A0rfY0zI38sTF1goG8s6lJNBFlL3VzyGEZ5Wb9UsDaE7thIqzRlgUNsI2foEAo84@93tmMwiB4Op0IhV/GM1pEFou/0FTZmJDM4wpppQcZ3pd6@QViTQSFNusLhhY/2W7s3AFvLOn8s8jW8PZbLw3FzLo4jGyrtNhVWTaruYDxunIEeo9omQ/FQHx4X4uNH8TUTqYxCx6esAj/4Ch4hJA2pTdo1lTUr3KAAlCxvMKN5kMZWqAcYREXBMj/K/6dqSBYs@DdE15X6SydZqIWAFiBq5dD8FczGUT57hXNhsLNVFx8kKUZ/BsqB0/Kxyg9ZR5LlRJzLU74rs414GJJTqqSEIp6IzURkE7GdiN2ibXXsJ1hMtRGgRA9Xfg7P6PJN30zIVz7BlmB5YRzP5z7Agwa2KcZ8VGT2/FhJ@Z4OaVEISf7pmu3yJL6RXgD@G0lksQMb2tCHsOM@@27yNz9JfBkRS5BMoMKOXHSO/jXC0RkO4oPntU9B@0Rv582T37715y3Ceey1GIMWUdBOD9uxsJ0etm@jdixql4nbGfGsXSZyQ1pn1kLM2vmzdn7SjiXt2JzYcYlbl9h1aSnPSxJay3PnJCeC9wlhwMTHRmlaSI8RwUa0oh9HMenFi2mZmBDOmG4IYUII5@yRYBnnc9JaGEY@8eiTBfhMycSj7zMjIADi0Sc1@xF7JB7jucd49IlHPyYspFh/RosTj37CTLB9DNwWNlCcKMuczdmKASkvIOMOmMmS8oKQPbbUBWSsAVlrwIwUlO75bElSWUAqC4iHgAw0ID2FZKEhGV9Ifhj6xKSXRDPmgWHA3I38jRQVMi8k1sKIVif3C0lJIeMSjCFkzhgSa2FCaMgaw3kLEJHHRcRa5Hv0SAAhcelDoHRJsBELIER5RORGZFMR2VRENhURpdGcxSDykCD2fJe4jEkpMZlbTEYVe2yUYhnpIw7ZI00LSbChH4eMy5jsLWbRkYdHAiDWYrKvmJQwc9kjcRmBU8Y8sLbrzMj7Z2RJM7KkGTExI5efkT5mROksZkvOZxFzkhkL4aSUGblDQoaSUJxKfPZIAZ@ITliOmrnBnIW7hJhISMYJUZ6QuBMyqoRknFBISli6SUiXiQ/KJC7nZDNzYmJO4p5ThJ1TIJqTzczJ5udE9Jz55Tz0E@aXczL8OVE@J3LnZB1znh9ZwnNZxnM9Ns4jkBv73IYgq/IMyxCwlOiynOiypOjG/JnNjRl8wnP13PXNbM0QsDToMg553jfSOyt6PJ/B@AwmYpwHEAmZw8JLNolR75EqPJ6xvYQtyIj1WY3i86LDY5yH82jG/Mhjmdtj@RoYYchYgeIzFfgRe54xGFZp6NyrFocoxAsIeMkAmQp8JvaAcRUwFbD0DbUU4Ql8Bu/PeMrzA754wIwmYBUTy79ewMQezFltxvQfMgmGrMALWSjx3SgIEyZ2lma9kBd9IUPALDxkNhIysYfMRsKEPwcs84KN8kqRJVwvTHjFyRaZ8/KT1Z@M24jZSBSy8ZBxHoK9cmuPGCcR4yRiKmDZF54ZTMLmMhthadaLWU3uR0liVOUx0xvLwh7LvV7MDChm@oyZF8QRe@aFNy94ZkkU8QgXs2gUM1HHnBMm9hkjdsaImjEVsCTrzVjpE7iR73Kxz5hhzZgKZkwFM@bDMyZqlmHhme0imNgTVkIHAZT7PMiwXOslzGgSRlTC3DFhtpAwW0iYa7K8Cs@sAouglOF@njCK54xillxhE8S2PgHfEbFxFvnmzDXnM3I1qORjXuXCSzYp4ZsqhtjYbNG2xqVoB6HdY8@0yXFZhANPi3kqh5cMGdt1uRFDHPNnBsO2Ty7bP7kJWzzhW0M3Sly@OOPEY9Sz7bDPUqTPNsQ@2xH7HtsjeiGbGwXOv@pnwdtTN2KNt5OwqTgRe9kWF@9gHBsnC@xUqdbTeLx3jNO6h@nUOPWRwzs1fwcS0jc4rHYRoByN1HU9R1ymzu1uPJYXcXDMqdJyMwL6sNmDz8eDbi63q2yOaqENzMl0jzrFCzsTsUU2Fvjqw43Yyssroy0@b5wFa71@3edFhlCfhZT0Fh8c60YGot/Kswf49elGrOTvTzddSHyhjsAETupvUW5kZ6x0kMhLkCmSNgydaWi7eb7RnbP1sazz8pz13CLZL0Q@nSL@DfaM128hqrMKTmx7ci6/DabH1IlMTy91@Xj82ukiPnf7sdnzY3ewfL0Ty28kPqNay8UCETUG@jzIKBDwLBk1@ISp9iGdHlY9Q7VqP0Z5S0ni7Jx0raosfei9ItVzI@7qkB2Wp7pK892@Hqmhx5pOjaWfICHLZVrXVb4619lyORqlhWzKjiLPb61EX@tqfAvQ4PENQIBnj/DKLYwsa/nCwV285/dSaQtfHbQsq/1x1N9CLyfmddrTOi3aC3x6DHvEUw8b0zfC10eANh7sHmuHVmFAtZL14Te82SilX@gwYJ55j8cpesfKVikEkjo/ZMdzLXawWClWv4p6nykqRVrtzoesrK87ZpBih12z0nanzWNQpHaNPMm4MLFv0dCp63rQjtayaV7aMWU8xhd4zLVGZwQJY6grVYhbQ7zaiB/h91Rgvx8PAcDPVajLVNjLVAhswt5LE/QA6A8JdEORTHO3fUX7Q5rXBz@tKR@rXwdPCdQJgXEb5VRnj1Lj8oq4OjzAv2N1gjCRJwfqAl17Z06eg8Ka4H/1RJxhcTXenlPIo8orcVjioRJAwWO1RGfQS5tXwsXV8/L05ZxWGSx@Zbw88avlf1@CguF/oPLqF/iHZ4dLcJmsXqKIFtZdWSwQHPs0Qr2SuU1D5afy@@Y8Oj@lqyK7NjOzhsMjW2VfMNoc6IqP8hwIT3Ph1SdMUTglfQUQs3im1c7QT2HGVXO5AJlY4Q3plM6hbU5SYXIBA49Ztc3WoFMpzOsWVXu21BhL5rxJMOpIpxXHSCZjV2djL8E7Klvu6@hjLFQ1q0nb2loZTjmVWgm8Rf7pmYeimrK1zahaAswMwgisc81yI8Ctsl1eCpDAppKh/pRnT1mj39/YyaA0UnV8LrboA8VxV@ibBuapuia9iYwjw86hFMjyosCcVcgz@WIE2K4Utu2j44DqfadFgSbgy4p1Q27naMfQDoOhiuck7dWTxotuGkCWjfEGqr8QBRjhBn@Nm6sD6nTPkH1rD8UEamYZrZq6QUv5g4BXMFfe7FJWW@iA1ZOuaVLfjRuECORJI9ISOE7v/Rm0LZMIScBYE@AjAd2bN9YFjc6ROmItVFWdN1X1VlfVW6qq84HbP205DZJFaTYE7nqEA/ZboEfs/goXQ7LFmZN@3reoPdwK4Nlss4q6xmzDIilKQFB7Dl5jgn3Fj0oi/XkQiJEXifGDDvvmTh8jVtrqveT1B0wMhSNvDzqDC0qO9@pk@n5w2WYFWN7XLPbr2vi5l/EC5b3XyDWae42mB0H3itaVcqjxWO40Fq@@RyHbMC9mZLamoQ/65gQToV2nYEQY6bnTxscxfgQK8l4WYGMRqT8fqNYhF4AIqMKFOKRlusuwsrrgTnYPsWlDK0NS7YQbHYXGIkBYhwP7NjDOHxvRXX@IQGZrF43bfP1LF4NvgbTlgA36CwDeM0CzVrChCc1Yljn8VpAsKFQstcoIG4u1hklApjUyCrDC32BNHzCVF3KDfCVXXwxv0vEtMX27u7vmH6IDQCB0IrRFZRMryNjo7heWXL4F4cuFETvRe/WFVQfvzjwo/PJVp@@QKRqyHpayu2v1ccFbeZtw1PfGvVNYr2T0aWKbdHCqmJiBWXqTxlbYcslU@NmpX538KYndYBjJ2rcgksAMasZGp/lZyZgxFVeNx45FX@hfqTrwI4eDfD@Wl5KsQbzm6qAAbFhjDYxDi@F7wStViSoNpMiNgc15LV22OZL0k3LNWQQ7XMUG4Edr1deSrIGC2ynYRtuUQtoujcaPXWIMUII1CxY40nJfuxBs@cyDZbGDr6XZ9i3u3qEq@9@0Bj6c5sDK14@/6vj6gIF4MiyqiRGm7R@IsLeIQV663zhcqiiW16aOxw//x/vO7XZ3J/5506bEd4ZheI7TjTvaZbfKFreNg26dbsTL2oiHEaxTl4LsbnEfihF6ezdUm@6QqGzINLQaYL@pNLFTmhiSmaE1Dav@yL5hoob95S/BJVn@N5Dlc7L8byDL/0uyXi4GS94MdT2sq2/TmZnD8Tr7jakb/BRIO5aZ5c5wjQq1s9477U@3e@4jeuCfeCEfHhzeh@/7aUhqZT0MWjHQ/2dKu@qCJNiTv6iVas5sffW@9dV3aC37ShvxBviZTiGvetJepAvbn6K5fJqYe@wW61a1UDxM0fIiuorkeOOaLvI7@JEpw3UZ6kpjsEuCLZUEI/zgY6Y2mSj83mhDlumDZQ5ZZTq4wUkndm7Zskxg8d/5iIri5fQKDu9uaC5@ChjFgNMvzz1QWAqdxdAHNumDHhJJ7fRc0r/E@@RPiN66mS9FvaZQHssNN26k19j@rK67HxKaTrd32HLFVq7xeXq@8NBk785aW/bh8GNqsItNsT0Lm1Vs1@reXNPvWi1k22ql@uJ4irVSAGun59AlFbJD@11WbvLtxZ/md3IcO1/T8aav80hPp6ySX@dxgder86KQzXHVKU@rKv1VfM3rvWrGNV@vIY5b7EVD@Qs2B@9h0ioTv2XVUdRZdcjLtM42151W8pXInrN13x1z1T/We8fuudCVJOS1xvKi7SiL9niALpCb7qSwyV0wLF3SwVLb01ETL7v9JmrLdjqyb2nGEozlDrLlCgJYyejS5@EYqaX0wEAaMWmpTLWeuh6m86d6PcaOuXwEd8ZtYf/prNODRyO4EbgDPfUAWEJkh2H9VQ/1hCciTByHtVjBtP4OBpVV0/9KV3kJBnWqT9e9SeRVRb6hSZe2HWOkQ/c5F@pjBrKP/PVYPeC5VIpNerDwdQqEbZRDpLw/3S8wrbHU1lh/Sixx35Iu3vxJvp72jz6OfqeNd8El5E@k0VJK0hVyyQ6/rxq6XWGcDDX29SKDxGN1fMo3IAdcbn0sIVLkWbmGXFunDxkIRsUZkZYbilyen0xXeU2nNBBis11WXV/wbwPaVkfZ/IDpSxyBGafmg36y4DrR0Sifpz@pY9xOQNiFfoP/XwnPxd3sSXbApuJ79/v@L7LgLNZHVHNZD9IPptl8U5AksD526OeEgpFbn0yU0063oatTCIc@pKcHeW5qHJrpgzQh28e6ZoM10dPBw9@773k1I1HgJ4Pw90fMgPik/tfmYHdSUv0dMWBDCKU6AypDrTFl2eFJNg5Hp9vpdHWn5aq/UmZs2/PpdoUV43vX84MwimfJ/P3tRl47tb5YIpWtrY944NNznEqfFpfnpOBdh@NTNsLo5MmDwbWDZ3n2SqlaCSPyRzxL6suxygR6Uqv8spdDmpfykN5RIjDyFtZgECCOa/WVV36k8fcmLYxWvdb@PvHwLqs/96MkxMunoZf4eP02ns18d66VSx6sHFh@@wCPqnFoNFv6L1R0gh8L7dLst6P3H9Bz3oNvdw0bv7sFjzK2xfm0H4GkjmdeLcmAhgtxMRPafpztZZRtlWXN2RPo5M8//2e9LdLd6c9pcfhf) with DigitalOcean, Thank You.

Shortest link to this page : [bit.ly/C-Factor](https://bit.ly/C-Factor)
