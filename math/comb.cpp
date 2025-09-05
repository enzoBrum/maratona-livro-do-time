const int MOD = 1e9+7;

inline int add(int a, int b) {
    a += b;
    if (a >= MOD) a -= MOD;
    return a;
}

inline int mul(int a, int b) { return a * b % MOD; }

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

const int MAXFAC = 1e5;
int fac[MAXFAC+1], ifac[MAXFAC+1];
void setup() {  
    fac[0] = 1;
    for (int i = 1; i <= MAXFAC; i++)
        fac[i] = mul(fac[i-1], i);
    ifac[MAXFAC] = inv(fac[MAXFAC]);
    for (int i = MAXFAC-1; i >= 0; i--)
        ifac[i] = mul(ifac[i+1], i+1);
}
