struct Matrix {
    int l, c;
    vector<vector<ll>> m;
    Matrix(int l, int c) : l(l), c(c) {
        m.assign(l, vector<ll>(c, 0));
    }

    Matrix operator* (Matrix &o) {
        Matrix res(l, o.c);
        for (int i = 0; i < l; i++)
            for (int j = 0; j < o.c; j++)
                for (int k = 0; k < c; k++)
                    res[i][j] = (res[i][j] + m[i][k] * o[k][j]) % MOD;
        return res;
    }

    vector<ll>& operator[](int idx) {
        return m[idx];
    }

    Matrix pow(ll e) {
        if (e == 0) return identity(l);
        if (e == 1) return *this;
        Matrix squared = (*this) * (*this);
        if (e % 2 == 0) return squared.pow(e/2);
        return squared.pow(e/2) * (*this);
    }

    Matrix identity(int sz) {
        Matrix mm(sz, sz);
        for (int i = 0; i < sz; i++)
            mm[i][i] = 1;
        return mm;
    }
};
