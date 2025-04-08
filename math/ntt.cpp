const int MOD = 998244353;

inline int add(int a, int b) {
    a += b;
    if (a >= MOD) a -= MOD;
    return a;
}

inline int mul(int a, int b) {
    return a * b % MOD;
}

int pwr(int a, int b) {
    int r = 1;
    while (b) {
        if (b & 1) r = mul(r, a);
        a = mul(a, a);
        b >>= 1;
    }
    return r;
}

int inv(int x) {
    return pwr(x, MOD-2);
}

void ntt(vector<int> &a, bool rev) {
    int n = a.size();
    vector<int> b = a;
    int g = 1;
    while (pwr(g, MOD / 2) == 1)
        g++;
    if (rev)
        g = inv(g);
    for (int step = n / 2; step; step /= 2) {
        int w = pwr(g, MOD / (n / step));
        int wn = 1;
        for (int i = 0; i < n / 2; i += step) {
            for (int j = 0; j < step; j++) {
                int u = a[2 * i + j];
                int v = mul(wn, a[2 * i + j + step]);
                b[i + j] = add(u, v);
                b[i + n / 2 + j] = add(u, MOD - v);
            }
            wn = mul(wn, w);
        }
        swap(a, b);
    }
    if (rev) {
        int n1 = inv(n);
        for (int &x : a)
            x = mul(x, n1);
    }
}

vector<int> multiply(vector<int> a, vector<int> b) {
    int n = 1;
    while (n < a.size() + b.size())
        n *= 2;
    a.resize(n); b.resize(n);
    ntt(a, false); ntt(b, false);

    for (int i = 0; i < n; i++)
        a[i] = mul(a[i], b[i]);

    ntt(a, true);
    return a;
}

// discards everything after lastIndex
vector<int> pwr(vector<int> a, int b, int lastIndex) {
    vector<int> r(a.size(), 0); r[0] = 1; // neutral polynomial
    while (b) {
        if (b & 1) {
            r = multiply(r, a);
            r.resize(lastIndex+1);
        }
        a = multiply(a, a);
        a.resize(lastIndex+1);
        b >>= 1;
    }
    return r;
}
