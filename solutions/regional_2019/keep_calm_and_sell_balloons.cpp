#include <bits/stdc++.h>

using namespace std;

#define int long long
const int INF = 0x3F3F3F3F;
using ii = pair<int, int>;
using ll = long long;
#define detachingFromC ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()

/*
Exercise:
You are given a 2*n matrix. You start on some cell and move orthogonally/diagonally without repeating
cells. You wanna know in how many different ways you can traverse the matrix.

Idea:
Let 'back(n)' be the amount of ways you can walk from the corner of a 2*n block to the
other side, but coming back. Clearly, back(n) = 2^n.

Let 'fwd(n)' be the amount of ways you can walk from the corner of a 2*n block to the
other side, but not necessarily coming back. Then:
fwd(n) = back(n) + 2*fwd(n-1) + 4*fwd(n-2)
The first term (back(n)) is simply coming back.
The second term (2*fwd(n-1)) is filling the first 2 tiles and going forward.
The third term (4*fwd(n-2)) is grabbing the first and second column, going back and then forward.
The base cases are:
fwd(0) = 1
fwd(1) = 2

Then, the answer is the following sum s(n):
int ans = 2*fwd(n); // Needed for the 2 corners (so n must be >= 2)
for (int col = 2; col <= n-1; col++) {
    ans += 2 * back(col-1) * fwd(n-col);
    ans += 2 * back(n-col) * fwd(col-1);
}

That is because you are summing, for each column other than the corners, the 2 ways to start on it,
times (the ways to go back left then forward right plus back right and forward left).
That can be further simplified to:
ans += back(col) * fwd(n-col);
ans += back(n-col+1) * fwd(col-1);

And, because each sum appears twice on the summing process:
ans += 2 * back(col) * fwd(n-col);

And, therefore:
ans += back(col+1) * fwd(n-col);

So, we have, for n >= 2, the following sum:
for (int col = 2; col <= n-1; col++)
    ans += back(col+1) * fwd(n-col);

We want to write it recursively, because that sum is O(n). Take a look at that sum for some given n:
s(n) = back(3) * fwd(n-2)
     + back(4) * fwd(n-3)
     + ...
     + back(n) * fwd(1)
And the sum s(n+1) we want is:
s(n+1) = back(3) * fwd(n-1)
       + back(4) * fwd(n-2)
       + ...
       + back(n) * fwd(2) 
       + back(n+1) * fwd(1)
It is kinda clear that:
s(n+1) = 2*s(n) + back(3) * fwd(n-1)
s(n)   = 2*s(n-1) + 8*fwd(n-2)

And that gives us all of our recurrences:
back(n) = 2*back(n-1)                       | back(0) = 1
fwd(n)  = back(n) + 2*fwd(n-1) + 4*fwd(n-2) | fwd(0) = 1, fwd(1) = 2
s(n)    = 2*s(n-1) + 8*fwd(n-2)             | s(2) = 0

The answer for the exercise is 2*fwd(n) + s(n).
Let's build the matrix for the fast power. It should have s(n+1), so the previous phase
must have s(n) and fwd(n-1). 
s(n)      | s(n+1)
fwd(n-1)  |

Since the previous has fwd(n-1), this one should have fwd(n), and fwd(n) demands back(n),
fwd(n-1) and fwd(n-2):
s(n)      | s(n+1)
fwd(n-1)  | fwd(n)
back(n)   |
fwd(n-2)  |

Since the previous has back(n), this one must have back(n+1) (no dependencies). It also must
have fwd(n-2 + 1) = fwd(n-1), also without dependencies. Therefore:
s(n)      | s(n+1)
fwd(n-1)  | fwd(n)
back(n)   | back(n+1)
fwd(n-2)  | fwd(n-1)

And our square matrix is:
            s(n)
            fwd(n-1)
            back(n)
            fwd(n-2)
===================
2 8 0 0     s(n+1)
0 2 1 4     fwd(n)
0 0 2 0     back(n+1)
0 1 0 0     fwd(n-1)

Since n-1 is the smallest argument in there, and it must be >= 0, we will hardcode our
base matrix with:
s(2)    = 0
fwd(1)  = 2
back(2) = 4
fwd(0)  = 1

And that's all.
*/

const bool DEBUG = true;

const int MOD = 1e9 + 7;

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

signed main() {
    detachingFromC;

    int n;
    cin >> n;

    if (n == 1) {
        cout << 2 << endl;
        return 0;
    }

    Matrix adv(4, 4);
    adv[0][0] = 2;
    adv[0][1] = 8;
    adv[1][1] = 2;
    adv[1][2] = 1;
    adv[1][3] = 4;
    adv[2][2] = 2;
    adv[3][1] = 1;

    // This one has s(2) precomputed
    Matrix base(4, 1);
    base[0][0] = 0;
    base[1][0] = 2;
    base[2][0] = 4;
    base[3][0] = 1;

    // Still gotta advance n-2 more times
    Matrix result = adv.pow(n-2) * base;
    int res = result[0][0]; // s(n)
    // Advance once again to get fwd(n)
    result = adv * result;
    int fwd = result[1][0];
    cout << (2*fwd%MOD + res%MOD) % MOD << endl;
    
    return 0;
}
