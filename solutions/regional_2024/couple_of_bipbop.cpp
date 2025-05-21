#include <bits/stdc++.h>

using namespace std;

#define int long long
const int INF = 0x3F3F3F3F;
using ii = pair<int, int>;
using ll = long long;
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()

const bool DEBUG = 0;

/*
Exercise:
Given a sequence of dance moves, determine the expected lcp of (i, j), given that (i, j) are randomly
and uniformly chosen.

Idea:
Build the suffix array and the lcp array of the input. Now, we have that solving the original problem is
almost the same as getting the expected range min of the lcp array (since the range min is the lcp of two
indices), except that we can't represent equal (i, j) on lcp array. Therefore, we must get the expected range
min PLUS the expected lcp of a pair of indices (i, i) (that is, j == i). The expected lcp of equal indices
is obviously 1+2+...+n, so we just need to focus on the expected range min.

That can be done easily:
For any given range, you can find its minimum, and you know that every path that goes through it will
have that minimum. Therefore, you sum that amount of paths and then continue the process with the left
and right ranges (that do not include this min index). That is O(n * c), where 'c' is the cost of range
min query (log(n) with segtree).
*/

class SegmentTree {
    struct Segment {
        int min, pos, l, r;
    };

    ii NEUTRAL = {INF, INF};
    vector<Segment> t;

    void build(vector<int> &a, int v, int tl, int tr) {
        t[v] = {a[tl], tl, tl, tr};
        if (tl != tr) {
            int mid = (tl + tr) / 2;
            build(a, 2*v, tl, mid);
            build(a, 2*v+1, mid+1, tr);
            merge(v);
        }
    }

    void merge(int v) {
        if (t[2*v].min <= t[2*v+1].min) {
            t[v].min = t[2*v].min;
            t[v].pos = t[2*v].pos;
        }
        else {
            t[v].min = t[2*v+1].min;
            t[v].pos = t[2*v+1].pos;
        }
    }

    ii merge(ii &a, ii &b) {
        return min(a, b);
    }

    ii query(int v, int l, int r) {
        if (t[v].l > r || t[v].r < l)
            return NEUTRAL;
        if (t[v].l >= l && t[v].r <= r)
            return {t[v].min, t[v].pos};
        
        ii left = query(2*v, l, r);
        ii right = query(2*v+1, l, r);
        return merge(left, right);
    }

public:
    SegmentTree(vector<int> &a) {
        t.resize(4*a.size());
        build(a, 1, 0, a.size()-1);
    }

    ii query(int l, int r) {
        return query(1, l, r);
    }
};

const int alphabet = 1e5+1;
vector<int> sortCyclicShifts(vector<int> &s) {
    int n = s.size();
    vector<int> p(n), c(n), cnt(max(alphabet, n), 0);
    for (int i = 0; i < n; i++)
        cnt[s[i]]++;
    for (int i = 1; i < alphabet; i++)
        cnt[i] += cnt[i-1];
    for (int i = 0; i < n; i++)
        p[--cnt[s[i]]] = i;
    c[p[0]] = 0;
    int classes = 1;
    for (int i = 1; i < n; i++) {
        if (s[p[i]] != s[p[i-1]])
            classes++;
        c[p[i]] = classes - 1;
    }
    vector<int> pn(n), cn(n);
    for (int h = 0; (1 << h) < n; ++h) {
        for (int i = 0; i < n; i++) {
            pn[i] = p[i] - (1 << h);
            if (pn[i] < 0)
                pn[i] += n;
        }
        fill(cnt.begin(), cnt.begin() + classes, 0);
        for (int i = 0; i < n; i++)
            cnt[c[pn[i]]]++;
        for (int i = 1; i < classes; i++)
            cnt[i] += cnt[i-1];
        for (int i = n-1; i >= 0; i--)
            p[--cnt[c[pn[i]]]] = pn[i];
        cn[p[0]] = 0;
        classes = 1;
        for (int i = 1; i < n; i++) {
            pair<int, int> cur = {c[p[i]], c[(p[i] + (1 << h)) % n]};
            pair<int, int> prev = {c[p[i-1]], c[(p[i-1] + (1 << h)) % n]};
            if (cur != prev)
                ++classes;
            cn[p[i]] = classes - 1;
        }
        c.swap(cn);
        if (c[p[n-1]] == n-1) break;
    }
    return p;
}

vector<int> getSuffixArray(vector<int> &s) {
    s.push_back(0);
    vector<int> ans = sortCyclicShifts(s);
    s.pop_back();
    ans.erase(ans.begin());
    return ans;
}

vector<int> getLcp(vector<int> &s, vector<int> const &sa) {
    int n = s.size();
    int k = 0;
    vector<int> lcp(n-1, 0), rank(n, 0);

    for (int i = 0; i < n; i++)
        rank[sa[i]] = i;

    for (int i = 0; i < n; i++, k?k--:0) {
        if (rank[i] == n-1) {
            k = 0;
            continue;
        }
        int j = sa[rank[i]+1];
        while (i+k<n && j+k<n && s[i+k] == s[j+k])
            k++;
        lcp[rank[i]] = k;
    }
    return lcp;
}

int e = 0;

void getExpected(int l, int r, SegmentTree &st) {
    if (l > r) return;
    auto[mini, idx] = st.query(l, r);
    // Every path that goes through idx is dominated by mini.
    // A path can start on [l, idx] and end on [idx, r].
    int amount = (idx - l + 1) * (r - idx + 1);
    e += 2 * mini * amount; // Times 2 because both i,j and j,i are valid indices with this range min
    getExpected(l, idx-1, st);
    getExpected(idx+1, r, st);
}

signed main() {
    cin.tie(0)->sync_with_stdio(0);

    int n;
    cin >> n;

    vector<int> v(n);
    for (int &i : v)
        cin >> i;

    if (n == 1) {
        cout << "1/1\n";
        return 0;
    }
    
    vector<int> lcp = getLcp(v, getSuffixArray(v));
    SegmentTree st(lcp);
    getExpected(0, lcp.size()-1, st);

    // Sum of all paths that start on equal indices (n + n-1 + n-2 + ... + 1)
    e += n*(n+1)/2;

    int d = n*n; // all possible pairs of indices
    int g = gcd(e, d);
    e /= g;
    d /= g;
    cout << e << '/' << d << '\n';

    return 0;
}
