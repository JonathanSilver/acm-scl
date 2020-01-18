#ifndef _NUMBER_THEORY_H
#define _NUMBER_THEORY_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cmath>
#include <ctime>
#include <limits>
#include <vector>
#include <map>
#include <algorithm>

#include "misc.h"


/**
 * Bit Array.
 */
#define bit            char
#define bit_len(n)     (((n) + CHAR_BIT - 1) / CHAR_BIT)
#define bit_arr_idx(i) ((i) / CHAR_BIT)
#define bit_ele_val(i) (1 << ((i) % CHAR_BIT))
#define bit_on(a, i)   ((a)[bit_arr_idx(i)] |= bit_ele_val(i))
#define bit_off(a, i)  ((a)[bit_arr_idx(i)] &= ~bit_ele_val(i))
#define bit_val(a, i)  ((a)[bit_arr_idx(i)] & bit_ele_val(i))


using namespace std;

/**
 * Gets the greatest common divisor (GCD)
 * of the given numbers a and b.
 */
template <class T>
T gcd(T a, T b)
{ return b ? gcd(b, a % b) : a; }

/**
 * Extended Euclidean algorithm.
 * Solves the equation: ax + by = gcd(a, b)
 */
template <class T>
T ext_gcd(T a, T b, T & x, T & y) {
    if (!b) { x = 1; y = 0; return a; }
    T q = ext_gcd(b, a % b, y, x);
    y -= a / b * x;
    return q;
}

/**
 * Solves the equation:
 * tx = 1 (mod p)
 * which is equivalent to solve the equation:
 * tx + py = 1
 * If x does not exist, then -1 will be returned.
 */
template <class T>
T inv(T t, T p) {
    T d, x, y;
    d = ext_gcd(t, p, x, y);
    return d == 1 ? (x % p + p) % p : -1;
}

/**
 * Chinese remainder theorem(CRT)
 * Solves the equations:
 *    x = a1 (mod m1)
 *    x = a2 (mod m2)
 *    ...
 *    x = an (mod mn)
 * where a1, a2, ..., an are any integers,
 * and m1, m2, ..., mn are pairwise coprime.
 */
template <class T>
T CRT(int n, T a[], T m[]) {
    T M = 1, r = 0;
    for (int i = 0; i < n; i++) M *= m[i];
    for (int i = 0; i < n; i++) {
        T w = M / m[i];
        r = (r + w * inv(w, m[i]) * a[i]) % M;
    }
    return (r + M) % M;
}

/**
 * Solves the equations:
 * A[i] * x = B[i] (mod M[i])
 * If x does not exist, then (0, -1) will be returned.
 * If x exists, the minimum x and the interval will be returned.
 */
template <class T>
pair<T, T> modular_linear_equations(int n, T A[], T B[], T M[]) {
    T x = 0, m = 1;
    for (int i = 0; i < n; i++) {
        T a = A[i] * m, b = B[i] - A[i] * x, d = gcd(M[i], a);
        if (b % d != 0) return pair<T, T>(0, -1);
        T t = b / d * inv(a / d, M[i] / d) % (M[i] / d);
        x += m * t;
        m *= M[i] / d;
    }
    x = (x % m + m) % m;
    return pair<T, T>(x, m);
}

/**
 * Gets a value indicating whether the input n is a prime.
 * Complexity: O(n^0.5)
 */
template <class T>
bool is_prime(T n) {
    if (n < 2) return false;
    for (T i = 2; i * i <= n; i++)
        if (n % i == 0) return false;
    return true;
}

/**
 * Gets all the prime factors of the input number n.
 * Complexity:
 * Best:  O(log(n))
 * Worst: O(n^0.5)
 */
template <class T, class U>
void prime_factors(T n, map<T, U> & m) {
    for (T i = 2; i * i <= n; ) {
        if (n % i == 0) {
            m[i]++; n /= i;
        } else i++;
    }
    if (n > 1) m[n]++;
}

/**
 * Euler's totient function
 * Counts the positive integers up to a given integer n
 * that are relatively prime to n.
 */
template <class T>
T euler(T n) {
    T r = n;
    for (T i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            r = r / i * (i - 1);
            while (n % i == 0)
                n /= i;
        }
    }
    if (n > 1) r = r / n * (n - 1);
    return r;
}

/**
 * Sieve of Eratosthenes
 * Generates a list of primes.
 * With N no more than 10000000.
 * Complexity: O(n log(log(n)))
 */
bit valid[bit_len(N)];
void sieve_of_eratosthenes() {
    memset(valid, 0, sizeof(valid));
    for (int i = 2; i * i < N; i++)
        for (int j = i; !bit_val(valid, i) && j * i < N; j++)
            bit_on(valid, i * j);
}

/**
 * Euler's Sieve
 * Generates a list of primes.
 * With N no more than 10000000.
 * Complexity: O(n)
 */
int prime[N], cnt, phi[N], mu[N];
void eulers_sieve() {
    memset(valid, 0, sizeof(valid));
    memset(prime, 0, sizeof(prime));
    memset(phi, 0, sizeof(phi));
    memset(mu, 0, sizeof(mu));
    cnt = 0; phi[1] = 1; mu[1] = 1;
    for (int i = 2; i < N; i++) {
        if (!bit_val(valid, i)) {
            prime[cnt++] = i;
            phi[i] = i - 1;
            mu[i] = -1;
        }
        for (int j = 0; j < cnt && i * prime[j] < N; j++) {
            bit_on(valid, i * prime[j]);
            if (i % prime[j]) {
                phi[i * prime[j]] = phi[i] * (prime[j] - 1);
                mu[i * prime[j]] = -mu[i];
            } else {
                phi[i * prime[j]] = phi[i] * prime[j];
                mu[i * prime[j]] = 0;
                break;
            }
        }
    }
}

template <typename T, typename U, typename V>
auto mod_pow(T a, U n, V p) {
    decltype(a + n + p) b = a % p, r = 1;
    while (n) {
        if (n & 1) r = r * b % p;
        b = b * b % p;
        n >>= 1;
    }
    return r;
}

/* p must be a prime */
template <typename T>
T C(T n, T m, T p) {
    if (m > n) return 0;
    T a = 1, b = 1;
    for (T i = n - m + 1; i <= n; ++i) a = a * i % p;
    for (T i = 1; i <= m; ++i) b = b * i % p;
    return a * mod_pow(b, p - 2, p) % p;
}
/* p < 100,000 */
template <typename T>
T lucas(T n, T m, T p) {
    if (m > n) return 0;
    T r = 1;
    for ( ; m; n /= p, m /= p)
        r = r * C(n % p, m % p, p) % p;
    return r;
}

/* n! mod p^k, pk = p^k */
template <typename T>
T fac(T n, T p, T pk) {
    if (n <= 1) return 1;
    T r = 1;
    if (n >= pk) {
        for (T i = 2; i < pk; i++)
            if (i % p) (r *= i) %= pk;
        r = mod_pow(r, n / pk, pk);
    }
    for (T i = 2; i <= n % pk; i++)
        if (i % p) (r *= i) %= pk;
    return r * fac(n / p, p, pk) % pk;
}
template <typename T>
T ext_C(T n, T m, T p, T pk) {
    T a = fac(n, p, pk);
    T b = inv(fac(m, p, pk), pk);
    T c = inv(fac(n - m, p, pk), pk);
    T k = 0;
    for (T i = n; i; i /= p) k += i / p;
    for (T i = m; i; i /= p) k -= i / p;
    for (T i = n - m; i; i /= p) k -= i / p;
    return a * b % pk * c % pk * mod_pow(p, k, pk) % pk;
}
/* p is not guaranteed to be a prime */
template <typename T>
T ext_lucas(T n, T m, T p) {
    T r = 0, tmp = p;
    for (T i = 2; i * i <= tmp; i++) {
        if (tmp % i == 0) {
            T pk = 1;
            while (tmp % i == 0) {
                pk *= i;
                tmp /= i;
            }
            (r += ext_C(n, m, i, pk) * (p / pk) % p * inv(p / pk, pk) % p) %= p;
        }
    }
    if (tmp > 1) (r += ext_C(n, m, tmp, tmp) * (p / tmp) % p * inv(p / tmp, tmp) % p) %= p;
    return r;
}

/**
 * Baby Step Giant Step (BSGS)
 * Finds the smallest non-negative
 * integer x such that
 * a ^ x = b (mod p)
 * where a > 0, b > 0, p > 0
 * AND (a, p) = 1.
 * Complexity: O(p^0.5)
 */
template <typename T>
T BSGS(T a, T b, T p) {
    T t = ceil(sqrt(p)); map<T, T> m;
    for (T j = 0, x = b % p; j <= t; j++, x = x * a % p) m[x] = j;
    for (T i = 1, y = mod_pow(a, t, p), x = y; i <= t; i++, x = x * y % p)
        if (m.find(x) != m.end()) return i * t - m[x];
    return -1;
}
/**
 * Extended Baby Step Giant Step
 * Finds the smallest non-negative
 * integer x such that
 * a ^ x = b (mod p)
 * where a > 0, b > 0, p > 0.
 */
template <typename T>
T ext_BSGS(T a, T b, T p) {
    // NOTE: Without the following for-loop,
    //       you will get WRONG ANSWER!
    for (T i = 0, x = 1 % p, y = b % p; i < 50; i++, x = x * a % p)
        if (x == y) return i;
    if (gcd(a, p) == 1) return BSGS(a, b, p);
    T n = 0, t = 1, d;
    while ((d = gcd(a, p)) != 1) {
        if (b % d) return -1;
        b /= d; p /= d; n++;
        // NOTE: The following line DOES NOT make sense!
        t = t * (a / d) % p;
    }
    T r = BSGS(a, b * inv(t, p), p);
    return r == -1 ? -1 : r + n;
}

/**
 * x^2 = a (mod n)
 * n is a prime
 * Complexity: O(log^2 n)
 */
template <typename T>
T mod_sqr(T a, T n) {
    T b, k, i, x;
    if (n == 2) return a % n;
    if (mod_pow(a, (n - 1) / 2, n) == 1) {
        if (n % 4 == 3) x = mod_pow(a, (n + 1) / 4, n);
        else {
            for (b = 1; mod_pow(b, (n - 1) / 2, n) == 1; b++);
            i = (n - 1) / 2;
            k = 0;
            do {
                i /= 2;
                k /= 2;
                if ((mod_pow(a, i, n) * mod_pow(b, k, n) + 1) % n == 0)
                    k += (n - 1) / 2;
            } while (i % 2 == 0);
            x = mod_pow(a, (i + 1) / 2, n) * mod_pow(b, k / 2, n) % n;
        }
        if (x * 2 > n) x = n - x;
        return x;
    }
    return -1;
}

template <typename T>
bool g_test(const vector<T> & v, T g, T p) {
    for (int i = 0; i < v.size(); i++)
        if (mod_pow(g, (p - 1) / v[i], p) == 1)
            return false;
    return true;
}
/* p must be a prime. */
template <typename T>
T primitive_root(T p) {
    T t = p - 1, g = 1; vector<T> v;
    for (T i = 2; i * i <= t; i++)
        if (t % i == 0) {
            v.push_back(i);
            while (t % i == 0) t /= i;
        }
    if (t != 1) v.push_back(t);
    while (true) {
        if (g_test(v, g, p)) return g;
        g++;
    }
}

/**
 * Finds all the x (0 <= x < p) such that
 * x ^ n = a (mod p)
 * where p is a prime.
 * Complexity: O(p^0.5)
 */
template <typename T>
vector<T> nth_root(T n, T a, T p) {
    vector<T> r;
    if (!a) {
        r.push_back(0);
        return r;
    }
    T g = primitive_root(p);
    T m = BSGS(g, a, p);
    if (m == -1) return r;
    T A = n, B = p - 1, C = m, x, y;
    T d = ext_gcd(A, B, x, y);
    if (C % d) return r;
    x = x * (C / d) % B;
    T delta = B / d;
    for (T i = 0; i < d; i++) {
        x = ((x + delta) % B + B) % B;
        r.push_back(mod_pow(g, x, p));
    }
    sort(r.begin(), r.end());
    r.erase(unique(r.begin(), r.end()), r.end());
    return r;
}

/**
 * Quick multiply (a * b % mod).
 * Complexity: O(log(n))
 */
template <class T>
T quick_mult(T a, T b, T mod) {
    b %= mod; T r = 0, t = a % mod;
    while (b) {
        if (b & 1) {
            r += t;
            if (r >= mod) r -= mod;
        }
        t <<= 1;
        if (t >= mod) t -= mod;
        b >>= 1;
    }
    return r;
}

/**
 * Quick power (a ^ n % mod).
 * Complexity: O(log(n))
 */
template <class T>
T quick_pow(T a, T n, T mod) {
    T r = 1, t = a % mod;
    while (n) {
        if (n & 1) r = quick_mult(r, t, mod);
        t = quick_mult(t, t, mod);
        n >>= 1;
    }
    return r;
}

/**
 * Uses a ^ (n - 1) = 1 (mod n) to check
 * whether n is a prime.
 * Returns false if n may be a prime.
 */
template <class T>
bool witness(T a, T n, T x, T t) {
    T r = quick_pow(a, x, n), last = r;
    for (T i = 1; i <= t; i++) {
        r = quick_mult(r, r, n);
        if (r == 1 && last != 1 && last != n - 1)
            return true;
        last = r;
    }
    if (r != 1) return true;
    return false;
}
/**
 * Times of tests.
 */
const int S = 8;
/**
 * Returns true if n may be a prime.
 * Complexity: O(S log(n))
 */
template <class T>
bool miller_rabin(T n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;
    T x = n - 1, t = 0;
    while ((x & 1) == 0) { x >>= 1; t++; }
    srand(time(NULL));
    for (int i = 0; i < S; i++) {
        T a = rand() % (n - 1) + 1;
        if (witness(a, n, x, t)) return false;
    }
    return true;
}

/**
 * Gets the greatest common divisor (GCD)
 * of the given numbers a and b.
 */
template <class T>
T _gcd(T a, T b) {
    T t;
    while (b) {
        t = a;
        a = b;
        b = t % b;
    }
    if (a >= 0) return a;
    return -a;
}

/**
 * Finds a prime factor.
 */
template <class T>
T pollard_rho(T x, int c) {
    T i = 1, k = 2;
    srand(time(NULL));
    T x0 = rand() % (x - 1) + 1, y = x0;
    while (true) {
        i++;
        x0 = (quick_mult(x0, x0, x) + c) % x;
        T d = _gcd(y - x0, x);
        if (d != 1 && d != x) return d;
        if (y == x0) return x;
        if (i == k) { y = x0; k += k; }
    }
}

/**
 * Uses Miller-Rabin & Pollard-Rho to find
 * all the prime factors of the given integer n.
 * (Expected) Complexity: O(n^(1/4))
 */
template <class T, class U>
void find_prime_factors(T n, map<T, U> & m, int k = 107) {
    if (n == 1) return;
    if (miller_rabin(n)) { m[n]++; return; }
    T p = n; int c = k;
    while (p >= n) p = pollard_rho(p, c--);
    find_prime_factors(p, m, k);
    find_prime_factors(n / p, m, k);
}

/**
 * Number-theoretic transform (NTT)
 * p = r * 2^k + 1
 * 23068673 = 11 * 2^21 + 1
 * 104857601 = 25 * 2^22 + 1
 * 167772161 = 5 * 2^25 + 1
 * 469762049 = 7 * 2^26 + 1
 * 998244353 = 119 * 2^23 + 1
 * 1004535809 = 479 * 2^21 + 1
 */
template <typename T>
vector<T> NTT(const vector<T> & a, T p, int op) {
    int n = a.size(); T g = primitive_root(p); vector<T> v(n);
    for (int i = 0; i < n; i++) v[rev(i, n)] = a[i];
    for (int s = 1; (1 << s) <= n; s++) {
        int m = (1 << s); T wm = mod_pow(g, (p - 1) / m, p);
        if (op == -1) wm = mod_pow(wm, p - 2, p);
        for (int k = 0; k < n; k += m) {
            T w = 1;
            for (int j = 0; j < (m >> 1); j++) {
                T t = w * v[k + j + (m >> 1)] % p;
                T u = v[k + j] % p;
                v[k + j] = (u + t) % p;
                v[k + j + (m >> 1)] = ((u - t) % p + p) % p;
                w = w * wm % p;
            }
        }
    }
    if (op == -1) {
        T inv = mod_pow(n, p - 2, p);
        for (int i = 0; i < n; i++)
            v[i] = v[i] * inv % p;
    }
    return v;
}

/**
 * Fast Walsh-Hadamard transform (FWT)
 * p must be a prime.
 */
void FWT(vector<int> & a, int p, int op) {
    const int inv2 = mod_pow(2LL, p - 2, p);
    const int n = a.size();
    for (int i = 1; i < n; i <<= 1) {
        for (int m = i << 1, j = 0; j < n; j += m) {
            for (int k = 0; k < i; k++) {
                int x = a[j + k], y = a[i + j + k];
                // xor:
                if (op == 1) {
                    a[j + k] = (x + y) % p;
                    a[i + j + k] = (x + p - y) % p;
                } else {
                    a[j + k] = 1LL * (x + y) * inv2 % p;
                    a[i + j + k] = 1LL * (x + p - y) * inv2 % p;
                }
                // and:
                if (op == 1) a[j + k] = (x + y) % p;
                else a[j + k] = (x + p - y) % p;
                // or:
                if (op == 1) a[i + j + k] = (y + x) % p;
                else a[i + j + k] = (y + p - x) % p;
            }
        }
    }
}

#endif // _NUMBER_THEORY_H
