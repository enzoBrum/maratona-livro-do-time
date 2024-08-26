struct Matrix {
    int n;
    vector<vector<ll>> m;
    Matrix(int n) : n(n) {
        m.assign(n, vector<ll>(n, 0));
    }

    Matrix operator* (Matrix &o) {
        Matrix res(n);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                for (int k = 0; k < n; k++)
                    res[i][j] = (res[i][j] + m[i][k] * o[k][j]) % MOD;
        return res;
    }

    vector<ll>& operator[](int idx) {
        return m[idx];
    }

    Matrix pow(ll e) {
        if (e == 1) return *this;
        Matrix squared = (*this) * (*this);
        if (e % 2 == 0) return squared.pow(e/2);
        return squared.pow(e/2) * (*this);
    }
};
