#ifndef QS_C
#define QS_C

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#define CINT_SCALE (1 << 12)

typedef __uint128_t positive_number ;

typedef struct {
    positive_number values [2];
    char * factors_vect;
} smooth_number_t;

static const size_t cint_exponent = 31;
static const long long int cint_base = 1LL << cint_exponent;

typedef struct {
    long long int data[CINT_SCALE];
    size_t index;
} cint;

void cint_down2i(cint *number) {
    if (number->index == CINT_SCALE) return;
    number->data[CINT_SCALE - 1] >>= 1;
    for (size_t i = CINT_SCALE - 2; i >= number->index; --i) {
        number->data[i + 1] |= (number->data[i] & 1) << (cint_exponent - 1);
        number->data[i] >>= 1;
    }
    if (number->data[number->index] == 0) ++number->index;
}

void cint_up2i(cint *number) {
    if (number->index == CINT_SCALE) return;
    for (size_t i = number->index; i < CINT_SCALE; ++i) {
        number->data[i - 1] |= number->data[i] >> (cint_exponent - 1);
        number->data[i] = (number->data[i] << 1) & ~cint_base;
    }
    if (number->data[number->index - 1]) --number->index;
}

static inline void cint_init(cint *num, positive_number value) {
    memset(num, 0, sizeof(cint));
    for (num->index = CINT_SCALE; value; num->data[--num->index] = (long long int)(value % cint_base), value /= cint_base);
}

int cint_compare(const cint *lhs, const cint *rhs) {
    int res = (int) (rhs->index - lhs->index);
    if (res == 0 && lhs->index != CINT_SCALE)
        for (size_t i = rhs->index; !(res = (int) (lhs->data[i] - rhs->data[i])) && ++i < CINT_SCALE;);
    return res;
}

void cint_mul(const cint *lhs, const cint *rhs, cint *res) {
    memset(res, 0, sizeof(cint));
    if (lhs->index == CINT_SCALE || rhs->index == CINT_SCALE)
        res->index = CINT_SCALE;
    else {
        res->index = 1 + lhs->index + rhs->index - CINT_SCALE;
        size_t i, j, k = res->index, l;
        for (i = rhs->index; i < CINT_SCALE; ++i, ++k) {
            for (l = k, j = lhs->index; j < CINT_SCALE; ++l, ++j)
                res->data[l] += lhs->data[j] * rhs->data[i];
            for (j = CINT_SCALE; --j >= res->index;) {
                res->data[j - 1] += res->data[j] / cint_base;
                res->data[j] %= cint_base;
            }
        }
        res->index -= res->data[res->index - 1] != 0;
    }
}

void cint_subi(cint *lhs, const cint *rhs) {
    if (lhs->index < CINT_SCALE && rhs->index < CINT_SCALE) {
        if (lhs->index < rhs->index || lhs->data[lhs->index] >= rhs->data[rhs->index]) {
            long long int a = 0;
            size_t i = CINT_SCALE;
            for (; --i >= rhs->index;)
                (a = (lhs->data[i] -= rhs->data[i] + a) < 0) ? (lhs->data[i] += cint_base) : 0;
            for (; a; lhs->data[i] -= a, (a = lhs->data[i] < 0) ? lhs->data[i] += cint_base : 0, --i);
            for (; lhs->index < CINT_SCALE && !lhs->data[lhs->index]; ++lhs->index);
        } else cint_init(lhs, 0);
    }
}

void cint_addi(cint *lhs, const cint *rhs) {
    long long int a = 0;
    size_t i = CINT_SCALE;
    for (; --i >= rhs->index;)
        (a = (lhs->data[i] += rhs->data[i] + a) > cint_base - 1) ? (lhs->data[i] -= cint_base) : 0;
    for (; a; lhs->data[i] += a, (a = lhs->data[i] > cint_base - 1) ? lhs->data[i] -= cint_base : 0, --i);
    if (1 + i < lhs->index) lhs->index = 1 + i;
}

void cint_div(const cint *lhs, const cint *rhs, cint *q, cint *r) {
    cint_init(r, 0);
    long long int a = cint_compare(lhs, rhs);
    cint_init(q, 0);
    if (a) {
        if (a > 0) {
            for (size_t i = lhs->index; i < CINT_SCALE; ++i) {
                for (a = cint_base; a >>= 1;) {
                    cint_up2i(r);
                    r->data[CINT_SCALE - 1] = a & lhs->data[i] ? r->data[CINT_SCALE - 1] | 1 : r->data[CINT_SCALE - 1] & ~1;
                    r->index -= r->data[r->index - 1] != 0;
                    if (cint_compare(r, rhs) >= 0) {
                        cint_subi(r, rhs);
                        q->data[i] |= a;
                    }
                }
            }
            q->index = lhs->index + CINT_SCALE - rhs->index - 1 ;
            q->index += q->data[q->index] == 0;
        } else *r = *lhs ;
    } else q->data[q->index = CINT_SCALE - 1] = 1;
}

void cint_sqrt(const cint *number, cint *root) {
    static long long int factor = 0;
    if (factor == 0) factor = 1 + ((long long int) floor(sqrt((double) cint_base)));
    if (CINT_SCALE == number->index)
        cint_init(root, 0);
    else {
        cint approx = {0}, r;
        approx.index = (CINT_SCALE + number->index) >> 1;
        approx.data[approx.index] = 1 + (long long int) floor(sqrt((double) number->data[number->index]));
        if (((number->index + CINT_SCALE) & 1) == 0 && (approx.data[approx.index] *= factor) >= cint_base)
            approx.data[approx.index] = cint_base - 1;
        cint_div(number, &approx, root, &r);
        cint_addi(root, &approx);
        cint_down2i(root);
        if (root->index != approx.index || approx.index != CINT_SCALE - 1 || approx.data[approx.index] - root->data[root->index] >= 2)
            for (; cint_compare(root, &approx) < 0; approx = *root, cint_div(number, &approx, root, &r), cint_addi(root, &approx), cint_down2i(root));
    }
}

static positive_number multiplication_modulo(positive_number a, positive_number b, const positive_number mod) {
    positive_number res = 0, tmp; // we avoid overflow in modular multiplication.
    for (b %= mod; a; a & 1 ? b >= mod - res ? res -= mod : 0, res += b : 0, a >>= 1, (tmp = b) >= mod - b ? tmp -= mod : 0, b += tmp);
    return res % mod;
}

static int is_prime(positive_number n, unsigned k) {
    positive_number a = 0, b, c, d, e, f, g;
    int h, i;
    if ((n == 1) == (n & 1)) return n == 2;
    if (n < 4669921) // fast constexpr for small primes.
        return ((n & 1) & ((n < 6) * 42 + 0x208A2882) >> n % 30 && (n < 49 || (n % 7 && n % 11 && n % 13 && n % 17 && n % 19 && n % 23 && n % 29 && (n < 961 || (n % 31 && n % 37 && n % 41 && n % 43 && n % 47 && n % 53 && n % 59 && n % 61 && n % 67 && (n < 5041 || (n % 71 && n % 73 && n % 79 && n % 83 && n % 89 && n % 97 && n % 101 && n % 103 && n % 107 && (n < 11881 || (n % 109 && n % 113 && n % 127 && n % 131 && n % 137 && n % 139 && n % 149 && n % 151 && n % 157 && (n < 26569 || (n % 163 && n % 167 && n % 173 && n % 179 && n % 181 && n % 191 && n % 193 && n % 197 && n % 199 && (n < 44521 || (n % 211 && n % 223 && n % 227 && n % 229 && n % 233 && n % 239 && n % 241 && n % 251 && n % 257 && (n < 69169 || (n % 263 && n % 269 && n % 271 && n % 277 && n % 281 && n % 283 && n % 293 && n % 307 && n % 311 && (n < 97969 || (n % 313 && n % 317 && n % 331 && n % 337 && n % 347 && n % 349 && n % 353 && n % 359 && n % 367 && (n < 139129 || (n % 373 && n % 379 && n % 383 && n % 389 && n % 397 && n % 401 && n % 409 && n % 419 && n % 421 && (n < 185761 || (n % 431 && n % 433 && n % 439 && n % 443 && n % 449 && n % 457 && n % 461 && n % 463 && n % 467 && (n < 229441 || (n % 479 && n % 487 && n % 491 && n % 499 && n % 503 && n % 509 && n % 521 && n % 523 && n % 541 && (n < 299209 || (n % 547 && n % 557 && n % 563 && n % 569 && n % 571 && n % 577 && n % 587 && n % 593 && n % 599 && (n < 361201 || (n % 601 && n % 607 && n % 613 && n % 617 && n % 619 && n % 631 && n % 641 && n % 643 && n % 647 && (n < 426409 || (n % 653 && n % 659 && n % 661 && n % 673 && n % 677 && n % 683 && n % 691 && n % 701 && n % 709 && (n < 516961 || (n % 719 && n % 727 && n % 733 && n % 739 && n % 743 && n % 751 && n % 757 && n % 761 && n % 769 && (n < 597529 || (n % 773 && n % 787 && n % 797 && n % 809 && n % 811 && n % 821 && n % 823 && n % 827 && n % 829 && (n < 703921 || (n % 839 && n % 853 && n % 857 && n % 859 && n % 863 && n % 877 && n % 881 && n % 883 && n % 887 && (n < 822649 || (n % 907 && n % 911 && n % 919 && n % 929 && n % 937 && n % 941 && n % 947 && n % 953 && n % 967 && (n < 942841 || (n % 971 && n % 977 && n % 983 && n % 991 && n % 997 && n % 1009 && n % 1013 && n % 1019 && n % 1021 && (n < 1062961 || (n % 1031 && n % 1033 && n % 1039 && n % 1049 && n % 1051 && n % 1061 && n % 1063 && n % 1069 && n % 1087 && (n < 1190281 || (n % 1091 && n % 1093 && n % 1097 && n % 1103 && n % 1109 && n % 1117 && n % 1123 && n % 1129 && n % 1151 && (n < 1329409 || (n % 1153 && n % 1163 && n % 1171 && n % 1181 && n % 1187 && n % 1193 && n % 1201 && n % 1213 && n % 1217 && (n < 1495729 || (n % 1223 && n % 1229 && n % 1231 && n % 1237 && n % 1249 && n % 1259 && n % 1277 && n % 1279 && n % 1283 && (n < 1661521 || (n % 1289 && n % 1291 && n % 1297 && n % 1301 && n % 1303 && n % 1307 && n % 1319 && n % 1321 && n % 1327 && (n < 1852321 || (n % 1361 && n % 1367 && n % 1373 && n % 1381 && n % 1399 && n % 1409 && n % 1423 && n % 1427 && n % 1429 && (n < 2053489 || (n % 1433 && n % 1439 && n % 1447 && n % 1451 && n % 1453 && n % 1459 && n % 1471 && n % 1481 && n % 1483 && (n < 2211169 || (n % 1487 && n % 1489 && n % 1493 && n % 1499 && n % 1511 && n % 1523 && n % 1531 && n % 1543 && n % 1549 && (n < 2411809 || (n % 1553 && n % 1559 && n % 1567 && n % 1571 && n % 1579 && n % 1583 && n % 1597 && n % 1601 && n % 1607 && (n < 2588881 || (n % 1609 && n % 1613 && n % 1619 && n % 1621 && n % 1627 && n % 1637 && n % 1657 && n % 1663 && n % 1667 && (n < 2785561 || (n % 1669 && n % 1693 && n % 1697 && n % 1699 && n % 1709 && n % 1721 && n % 1723 && n % 1733 && n % 1741 && (n < 3052009 || (n % 1747 && n % 1753 && n % 1759 && n % 1777 && n % 1783 && n % 1787 && n % 1789 && n % 1801 && n % 1811 && (n < 3323329 || (n % 1823 && n % 1831 && n % 1847 && n % 1861 && n % 1867 && n % 1871 && n % 1873 && n % 1877 && n % 1879 && (n < 3568321 || (n % 1889 && n % 1901 && n % 1907 && n % 1913 && n % 1931 && n % 1933 && n % 1949 && n % 1951 && n % 1973 && (n < 3916441 || (n % 1979 && n % 1987 && n % 1993 && n % 1997 && n % 1999 && n % 2003 && n % 2011 && n % 2017 && n % 2027 && (n < 4116841 || (n % 2029 && n % 2039 && n % 2053 && n % 2063 && n % 2069 && n % 2081 && n % 2083 && n % 2087 && n % 2089 && (n < 4405801 || (n % 2099 && n % 2111 && n % 2113 && n % 2129 && n % 2131 && n % 2137 && n % 2141 && n % 2143 && n % 2153)))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))));
    for (b = c = n - 1, h = 0; !(b & 1); b >>= 1, ++h);
    for (; k--;) {
        for (g = 0; g < sizeof(positive_number); ((char *) &a)[g++] = (char)rand()); // random number
        do for (d = e = 1 + a % c, f = n; (d %= f) && (f %= d););
        while (d > 1 && f > 1);
        for (d = f = 1; f <= b; f <<= 1);
        for (; f >>= 1; d = multiplication_modulo(d, d, n), f & b && (d = multiplication_modulo(e, d, n)));
        if (d == 1) continue;
        for (i = h; i-- && d != c; d = multiplication_modulo(d, d, n));
        if (d != c) return 0;
    }
    return 1;
}

positive_number mod_pow(positive_number x, positive_number exp, positive_number n) {
    positive_number res = 1;
    for (x %= n;; exp >>= 1, x = multiplication_modulo(x, x, n))
        if (exp) {
            if (exp & 1) res = multiplication_modulo(res, x, n);
        } else break;
    return res;
}

static inline void *mem_straight(void *ptr) {
    char *res __attribute__((aligned(512)));
    res = (char *) ptr + (512 - (uintptr_t) ptr) % 512;
    return res;
}

positive_number factor_rho(const positive_number n, const size_t scale) {
    size_t a = -1, b = 2 ;
    positive_number c, d = 1 + rand(), e, f;
    c = d %= n;
    do {
        if (++a == b) {
            // timeout given by the scale argument.
            if (a >= scale) return n ;
            d = c, b <<= 1, a = 0;
        }
        c = multiplication_modulo(c, c, n);
        for (++c, c *= c != n, e = n, f = c > d ? c - d : d - c; (f %= e) && (e %= f););
    } while ((f |= e) == 1);
    return f;
}

positive_number factor(const positive_number number, void *memory) {
    positive_number a, b, c; // the number 12 is a parameter to the quadratic sieve.
    size_t d, e = 16, f, g, h, i, j, k, l, m;
    cint r, s, t, u, v;
    unsigned long * m_roots, * mr_ptr ;
    smooth_number_t *x_squared, *smooth_numbers;
    char *M_1, *M_2, *T, *base_reset_fact;
    if (number < 4)
        return number; // number isn't factorisable.
    for (b = number >> 1, a = (b + number / b) >> 1; a < b; b = a, a = (b + number / b) >> 1, ++e);
    b = number - a * a;
    if (b) ++a;
    else
        return a ; // number is a perfect square.
    if (is_prime(number, e))
        return number; // number is prime.
    for(f = 10; f <= 18; ++f) {
        c = factor_rho(number, 1 << f);
        if (c != number && c != 1)
            return c ; // number is factored by rho.
    }
    // begin quadratic sieve factorization.
    long double fp = logl((long double) number);
    d = 1 + (unsigned long) ceill(expl(sqrtl(fp * logl(fp)) / 2));
    //
    m = 1 << 12 ; // the number 12 is a parameter to the sieve.
    d >> 13 || (d = 1 << 13) ; // the number 13 is a parameter to the sieve.
    //
    m_roots = mem_straight(memory), mr_ptr = m_roots;
    for (l = 2; l < d; l += 1 + (l & 1))
        if (is_prime(l, 20) && mod_pow(number % l, (l - 1) >> 1, l) == 1) {
            if (number % l) {
                if (3 == (l & 3))
                    f = mod_pow(number, (l + 1) >> 2, l);
                else {
                    for (f = l - 1, i = 0; !(f & 1); f >>= 1, ++i);
                    for (g = 2; 1 + mod_pow(g, (l - 1) >> 1, l) != l; ++g);
                    for (g = mod_pow(g, f, l), f = mod_pow(number, (f + (h = 1)) >> 1, l), k = mod_pow(number, l - 2, l); h;
                         h ? (f = multiplication_modulo(f, (j = i - h - 1) ? mod_pow(g, 1 << j, l) : g, l)) : 0)
                        for (h = 0, j = multiplication_modulo(mod_pow(f, 2, l), k, l);
                             j != 1; ++h, j = mod_pow(j, 2, l));
                }
                *mr_ptr++ = f ;
                *mr_ptr++ = l - f ;
            } else
                *mr_ptr += 2;
            *mr_ptr++ = l ;
        }
    d = (mr_ptr - m_roots) / 3;
    j = d + 5;
    k = 0;
    {
        // memory management
        f = j * d;
        M_1 = mem_straight(m_roots + 3 * d);
        M_2 = mem_straight(M_1 + f);
        memset(M_1, 0, f + f);
        T = mem_straight(M_2 + f);
        x_squared = mem_straight(T + j);
        smooth_numbers = mem_straight(x_squared + m);
        char *ptr = base_reset_fact = mem_straight(smooth_numbers + j);
        e = d + (32 - d) % 32;
        l = e * m;
        for (g = 0; g < m; x_squared[g].factors_vect = ptr, ptr += e, ++g);
        for (g = 0; g < j; smooth_numbers[g].factors_vect = ptr, ptr += e, ++g);
    }
    for (f = h = (size_t) a; k < j; f = h) {
        for (e = 0; e < m; x_squared[e].values[1] = (x_squared[e].values[0] = h) * h - number, ++h, ++e);
        memset(base_reset_fact, 0, l);
        for (e = 0, g = 0, mr_ptr = m_roots; e < d; ++e, mr_ptr += 3)
            do {
                b = f - *(mr_ptr + g);
                b = (b / *(mr_ptr + 2) + (b % *(mr_ptr + 2) != 0)) * *(mr_ptr + 2) + *(mr_ptr + g) - f;
                for (a = b; a < m; a += *(mr_ptr + 2)) {
                    for (i = 0; !(x_squared[a].values[1] % *(mr_ptr + 2)); x_squared[a].values[1] /= *(mr_ptr + 2), ++i);
                    x_squared[a].factors_vect[e] = (char) (i & 1);
                    if (x_squared[a].values[1] == 1 && k < j) {
                        smooth_numbers[k].values[1] = (smooth_numbers[k].values[0] = x_squared[a].values[0]) * x_squared[a].values[0] - number;
                        memcpy(M_1 + k * d, x_squared[a].factors_vect, d);
                        M_2[k * (1 + d)] = (char) (k < d);
                        ++k;
                    }
                }
            } while ((g ^= *mr_ptr != *(mr_ptr + 1)));
    }
    for (e = 0, f = 0; f < d; ++f)
        for (g = e; g < j; ++g)
            if (M_1[g * d + f]) {
                if (g != e) {
                    memcpy(T, M_1 + g * d, d);
                    memcpy(M_1 + g * d, M_1 + e * d, d);
                    memcpy(M_1 + e * d, T, d);
                    memcpy(T, M_2 + g * d, d);
                    memcpy(M_2 + g * d, M_2 + e * d, d);
                    memcpy(M_2 + e * d, T, d);
                }
                for (g = e + 1; g < j; ++g)
                    if (M_1[g * d + f]) {
                        char *lhs = M_1 + g * d, *rhs = M_1 + e * d;
                        for (h = 0; h < d; lhs[h] = (char) (lhs[h] ^ rhs[h]), ++h);
                        lhs = M_2 + g * d;
                        rhs = M_2 + e * d;
                        for (h = 0; h < d; lhs[h] = (char) (lhs[h] ^ rhs[h]), ++h);
                    }
                ++e;
                break;
            }
    for (e = j, f = 0; !memchr(M_1 + d * --e, 1, d); ++f);
    cint_init(&v, number);
    for (e = f, a = 1; e && (a == 1 || a == number); --e) {
        cint_init(&r, a = 1);
        for (f = 0, g = (j - e + 1) * d; f < d; ++f)
            if (M_2[g + f]) {
                a = multiplication_modulo(a, smooth_numbers[f].values[0], number);
                cint_init(&s, smooth_numbers[f].values[1]);
                cint_mul(&r, &s, &u);
                r = u ;
            }
        cint_sqrt(&r, &t);
        cint_div(&t, &v, &u, &r);
        for (c = 0, f = 6; f > 2; c |= r.data[CINT_SCALE - --f], c <<= cint_exponent);
        c |= r.data[CINT_SCALE - 1];
        for (b = a > c ? a - c : c - a, a = number; b; c = b, b = a % b, a = c);
    }
    return a ;
}

#endif
