// precomputes factorials and inverse factorials
// O(n log mod) needed for the inverse factorials

tuple<int, int, int> extendedGcd(int a, int b) {
    if (b == 0) return make_tuple(a, 1, 0);
    auto[q, w, e] = extendedGcd(b, a%b);
    return make_tuple(q, e, w-e*(a/b));
}

int multiplicativeInverse(int n, int mod) {
    auto[g, x, y] = extendedGcd(n, mod);
    return (x % mod + mod) % mod;
}

int mod;
vector<int> fac, ifac;
int comb(int n, int k) {
    return fac[n] * ifac[k] % mod * ifac[n-k] % mod;
}

int main() {
    const int MAXK = 5000 + 10;
    fac.resize(MAXK);
    ifac.resize(MAXK);
    fac[0] = ifac[0] = 1;
    for (int i = 1; i < MAXK; i++) {
        fac[i] = fac[i-1] * i % mod;
        ifac[i] = multiplicativeInverse(fac[i], mod);
    }
}
