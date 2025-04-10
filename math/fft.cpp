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

/*
About changing summing conditions.
Let's take a look at what a regular FFT condition looks like:
ans[k] = sum(i+j=k)       A[i] * B[j]
       = sum(i=0 until k) A[i] * B[k-i]

If you can write your desired sum condition as
sum(i=0 until r) A[ai] * B[bi]
such that ai+bi=r, then, you run FFT on A and B and your desired value ans[k] is on fft[r].
The deal is that it may happen that the indices of A and B do not sum to 'r'. In that case,
you will build a new vector (say, "A transformed" AT), such that its index, when summed to
B's index, equals 'r'.

Example below for the condition i-j=k:
ans[k] = sum(i-j=k) A[i] * B[j]
       = sum(i=k until n-1) A[i] * B[i-k]
       = sum(i=0 until n-1-k) A[i+k] * B[i]
We have that r = n-1-k. We need the sum of A's index and B's index to be r. Let's transform A.
It should hold for AT's new index 'f' that:
f+i = n-1-k
f = n-1-k-i
So, let's build a vector AT such that its index f = n-1-k-i points to A[i+k]. Therefore:
AT[n-1-k-i] = A[i+k]
AT[n-1-(i+k)] = A[i+k]
So, given A's index 'q', then:
AT[n-1-q] = A[q]

And, after we do the convolution, we have that
ans[k] = fft[n-1-k]
*/

// THIS ONE COMPUTES sum(i-j=k) A[i] * B[j]
vector<int> computeModifiedConvolution(vector<int> &a) {
    /*
    I wanna compute the convolution
    ans[d] = sum(i-j=d) a[i] * b[j]
    where b = a.
    We have seen above that the transformed version 'at' of 'a' should be
    at[n-1-i] = a[i]
    Let's compute that and multiply.
    */
    vector<int> at(a.size());
    for (int i = 0; i < a.size(); i++)
        at[a.size()-1-i] = a[i];
    vector<int> c = multiply(at, a);
    /*
    But since in that computation r was equal to n-1-k, then, we have that ans[k] is not at position k,
    but actually n-1-k. Therefore, we compute the answer vector below doing that index rearrangement.
    Since 'ans' must hold an index equivalent to the maximum difference xj-xi, then, its size must
    be MAX_X+1.
    */
    vector<int> ans(MAX_X+1, 0);
    for (int k = 0; k < ans.size(); k++)
        ans[k] = c[a.size()-1-k];
    return ans;
}
