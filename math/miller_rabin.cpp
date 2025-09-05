/*
Miller Rabin
Checks if a number is prime in O(logn)
*/

using u64 = uint64_t;
using u128 = __uint128_t;

u64 pwr(u64 base, u64 e, u64 mod) {
    u64 result = 1;
    base %= mod;
    while (e) {
        if (e & 1) result = (u128)result * base % mod;
        base = (u128)base * base % mod;
        e >>= 1;
    }
    return result;
}

bool checkComposite(u64 n, u64 a, u64 d, int s) {
    u64 x = pwr(a, d, n);
    if (x == 1 || x == n - 1)
        return false;
    for (int r = 1; r < s; r++) {
        x = (u128)x * x % n;
        if (x == n - 1)
            return false;
    }
    return true;
};

bool isPrime(u64 n) {
    if (n < 2) return false;
    int r = 0;
    u64 d = n - 1;
    while ((d & 1) == 0)
        d >>= 1, r++;

    for (int a : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
        if (n == a) return true;
        if (checkComposite(n, a, d, r)) return false;
    }
    return true;
}
