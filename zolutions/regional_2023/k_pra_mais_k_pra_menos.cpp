/*
Exercise:
You are given the coefficients of the polynomials t(x) and p(x). Compute t(x+k) + p(x-k).

Idea:
Let's count how many times the term x^t appears on the whole expansion of the polynomial at (x+k). 
Remembering binomial expansion:
(x+k)^n = (n choose 0)*x^n*k^0 + (n choose 1)*x^(n-1)*k^1 + ... + (n choose n)*x^0*k^n

The term x^t will appear on the expansion of x^i as its jth term iff i-j=t.
So, our desired vector becomes
c[t] = sum(i-j=t) (i choose j) * k^j * a[i];
     = sum(i-j=t) i! * k^j * a[i] / (j! * t!)
     = 1/t! * sum(i-j=t) i! * k^j * a[i] / j!        // ALWAYS TAKE OUT CONSTANTS OF THE SUM, WHENEVER POSSIBLE
     = 1/t! * sum(i-j=t) A[i] * B[j]
where
A[i] = i! * a[i]
B[i] = k^i * / i!
And that can be trivially computed by reversing A (btw, reversing B is not the same).
Now, given the frequency of x^t on the expansion, we do that for the two polynomials and simply
sum the frequencies of each x^i for every i.
*/

// Bring subtraction convolution here
signed main() {
    cin.tie(0)->sync_with_stdio(0);

    int n, k;
    cin >> n >> k;
    vector<int> t(n+1), p(n+1);
    for (int &i : t) cin >> i;
    for (int &i : p) cin >> i;
    if (k < 0) k += MOD;

    // Precomputing everything
    vector<int> fac(n+1), ifac(n+1), A(n+1), B(n+1), kpow(n+1);
    fac[0] = ifac[0] = 1;
    kpow[0] = 1;
    for (int i = 1; i <= n; i++) {
        fac[i] = mul(fac[i-1], i);
        ifac[i] = inv(fac[i]);
        kpow[i] = mul(kpow[i-1], k);
    }

    //A[i] = i! * a[i]
    //B[i] = k^i * / i!
    for (int i = 0; i <= n; i++) {
        A[i] = mul(fac[i], t[i]);
        B[i] = mul(kpow[i], ifac[i]);
    }

    vector<int> c1 = subtractionConvolution(A, B);
    for (int t = 0; t <= n; t++)
        c1[t] = mul(c1[t], ifac[t]); // multiply by the dude out of the sum (1/t!)

    // Now doing the same but for 'p'. Don't forget k swapped signs.
    k = add(0, MOD-k); // k = -k
    for (int i = 1; i <= n; i++)
        kpow[i] = mul(kpow[i-1], k);
    
    //A[i] = i! * a[i], but for p
    //B[i] = k^i * / i! with the new k
    for (int i = 0; i <= n; i++) {
        A[i] = mul(fac[i], p[i]);
        B[i] = mul(kpow[i], ifac[i]);
    }
    
    // Good old fft
    vector<int> c2 = subtractionConvolution(A, B);
    for (int t = 0; t <= n; t++)
        c2[t] = mul(c2[t], ifac[t]);

    // now we have the frequency of each dude. Print their sum
    for (int i = 0; i <= n; i++)
        cout << add(c1[i], c2[i]) << " \n"[i==n];
    return 0;
}
