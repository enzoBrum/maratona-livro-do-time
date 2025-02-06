const double PI = 4*atan(1);
using cd = complex<double>;

void fft(vector<cd> &a) {
    int n = a.size();
    if (n == 1) return; // because a_0*z^0 = a_0 (evaluating at any point returns a_0)
    vector<cd> a0(n/2), a1(n/2);
    for (int i = 0; i < n/2; i++) {
        a0[i] = a[2*i];
        a1[i] = a[2*i+1];
    }
    fft(a0); fft(a1);
    double alpha = 2*PI/n;
    cd omega(cos(alpha), sin(alpha));
    cd omega_i = 1;

    for (int i = 0; i < n/2; i++) {
        a[i] = a0[i] + omega_i * a1[i];
        a[i+n/2] = a0[i] - omega_i * a1[i]; // -omega_i*a1[i] == (-1)*omega_i*a1[i] == omega_(n/2)*omega_i*a1[i] == omega_(i+n/2)*a1[i]
        omega_i *= omega;
    }
}

void ifft(vector<cd> &a) { // DFT(DFT(c0, c1, c2, ..., c(n-1))) = (nc0, nc(n-1), nc(n-2), ..., nc1)
    int n = a.size();
    fft(a);
    reverse(a.begin()+1, a.end());
    for (cd &i : a)
        i /= n;
}

vector<int> multiply(vector<int> &a, vector<int> &b) {
    vector<cd> fa(all(a)), fb(all(b));
    int n = 1;
    while (n < a.size() + b.size())
        n *= 2;
    fa.resize(n); fb.resize(n);
    fft(fa); fft(fb);

    for (int i = 0; i < n; i++)
        fa[i] *= fb[i];

    ifft(fa);
    vector<int> res(n);
    for (int i = 0; i < n; i++)
        res[i] = round(fa[i].real());
    return res;
}
