#include <bits/stdc++.h>

using namespace std;

#define int long long
const int INF = 0x3F3F3F3F;
using ii = pair<int, int>;
using ll = long long;
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()

/*
Exercise:
You wanna compose a string 's' as a sum of several strings 'm' of length <= D
such that s = m1^j1 + p(m1) + m2^j2 + p(m2) + ... + mk^jk + p(mk)
where p(r) is a (possibly empty) prefix of the string r.

Idea: suppose you have a string s and you want to build it by concatenating some
other string p several times + a prefix of p. The minimum length p can have is
given by KMP's n - lps[n-1] for a string of length n.

With that in mind, we will greedily build the lps list while the value
k = i + 1 - lps[i]
is within the acceptable length (k <= d). The moment k exceeds that value,
we increment the answer and start a new kmp from scratch.
*/

signed main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    string s;
    int d;
    cin >> s >> d;
    int n = s.size();

    vector<int> lps = {0};
    int ops = 1;
    int sz = n;
    int i = 0;
    int offset = 0;
    while (++i < sz) {
        int j = lps[i-1];
        while (j > 0 && s[offset+i] != s[offset+j])
            j = lps[j-1];
        if (s[offset+i] == s[offset+j])
            j++;
        lps.push_back(j);

        int k = i + 1 - lps[i];
        if (k > d) { 
            // Starting new kmp here (removing the first i letters of the string, i.e.,
            // everyone before the current position)
            ops++;
            lps = {0};
            sz -= i;
            offset += i;
            i = 0;
        }
    }

    cout << ops << '\n';
    
    return 0;
}
