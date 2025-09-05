const int MOD1 = 1e9+7;
const int MOD2 = 1e9+9;

mt19937 gen(random_device{}());

int uniform(int l, int r) {
    uniform_int_distribution<int> uid(l, r);
    return uid(gen);
}

template<int MOD> struct Hash {
    inline static int P = uniform(256, MOD - 1); // l should be > alphabet
    vector<ll> h, p;
    Hash(string const &s) : h(s.size()), p(s.size()) {
        p[0] = 1, h[0] = s[0];
        for (int i = 1; i < s.size(); i++)
            p[i] = p[i - 1]*P%MOD, h[i] = (h[i - 1]*P + s[i])%MOD;
	}
    ll operator()(int l, int r) {
        ll hash = h[r] - (l ? h[l - 1]*p[r - l + 1]%MOD : 0);
        return hash < 0 ? hash + MOD : hash;
    }
};
