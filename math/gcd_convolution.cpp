/*
O(nlogn)
GCD convolution:
d[k] = sum(every i,j such that gcd(i,j)=k) a[i]*b[j]

Idea:
We wanna compute the vector d[k] = sum(every i,j such that gcd(i, j)=k) a[i] * b[j]
Start by computing this arbitrary sum S[g]:
S[g] = sum of every d[k] such that g | k
= sum of a[i] * b[j] such that gcd(i, j) = k and g | k
= sum of a[i] * b[j] such that g | gcd(i, j) (1)
I claim it also equals this:
(sum of b[i] for every i such that g|i) * (sum of c[i] for every i such that g|i) (2)
Proof:
Every product of (1) has the property that g|i and g|j, so, they appear on (2). But also every pair
of (2) has the property of g | gcd(i, j). Therefore, they appear on (1), and (1) == (2).

Both of the parts of the product can be done in nlogn because of harmonic series.
Now, how to use S to build d? Well, since S[g] is the sum of d[k] for k multiple of g, then, I can just
use it as d[g] and subtract values of d[k] (k > g) already computed (in decreasing dp harmonic style).
*/

vector<int> gcdConvolution(vector<int> &a, vector<int> &b) {
    int n = min(a.size(), b.size());
    vector<int> S(n, 0);

    for (int i = 1; i < n; i++) {
        int as = 0;
        int bs = 0;
        for (int j = i; j < n; j += i) {
            as += a[j];
            bs += b[j];
        }
        S[i] = as * bs;
    }

    // here S is d itself
    for (int g = n-1; g >= 1; g--)
        for (int j = 2*g; j < n; j += g) 
            S[g] -= S[j];
    return S;
}