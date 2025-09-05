#include <bits/stdc++.h>

using namespace std;

//#define int long long
const int INF = 0x3F3F3F3F;
using ii = pair<int, int>;
using ll = long long;
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()

const bool DEBUG = 0;

/*
Exercise:
Implement the following queries:
 - Range maximum query
 - Range increment (a[i]++)
 - Range max reset (a[i] = 0 iff a[i] == maximum on WHOLE array)

Idea:
You could see it as segtree beats with range mod, but it won't work because range increment
increases potential way too much. Therefore, we will sqrt decompose the whole thing. We need
to implement O(1) query and updates for ranges that are completely contained by the query
range [l, r] and we gotta be able to rebuild a range in case it intersects [l, r].

Let's start by implementing a build() function that does its job given a vector 'a'. Then, every
other partial range update will just make that vector and give it to build().

Maximum query and range increment are easy. The big deal is range max reset. We need to be able
to reset all the values that are equal to the max in O(1) in a range contained in [L, R].
That is why we will use union find - to be able to edit a lot of values at once. Because we are
merging groups of equal VALUES, the uf will be relative to the VALUES.
Therefore, our access to individual elements a[i] won't be through any vector directly. We have
to find what uf group represents the ith position, and then query the value of uf.find(that group).
Clearly, we will also need an additional vector that tells us the group of each index 'i' to
implement that.
Now, when we reset max on a range, we will change the value of the uf group that contains the max.
It may happen that this new value coincides with an already existing value - in that moment, we
make a merge, so futurely we can edit everyone with that same value at once.
To quickly check if there is already a group with that value, we will keep an ordered deque, because
we will always pop back (because we are taking out the max), and this new value will be minimum, and
could only possibly collide with someone on the beggining of the deque.

Complexity
Let B be the size of the block.
 - Range max: n/B + 2*B
 - Range increment: n/B + 2*build()
 - Range reset: n/B + 2*build()

Build is O(B) with some logs of compression, but it is constant heavy. Let's make q * n/B as close
as possible to 1e8 and the rest goes to build.
q = 1e5, n = 1e5, therefore, B = 100 is reasonable.
O(B) for rebuild will be O(100) with some logs of compression and constants of rebuilding UF.

Total:
1e5 * (1e3 light + 2e2 * log (heavy))
*/

struct UnionFind {
    vector<int> parent, size;    
    UnionFind(int n=0) : parent(n), size(n, 1) {
        iota(all(parent), 0ll);
    }
    
    int find(int v) {
        if (v == parent[v]) return v;
        return parent[v] = find(parent[v]);
    }
    
    void unionSets(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return;
        if (size[a] < size[b]) swap(a, b);
        parent[b] = a; // subordinate b to a (smaller to bigger)
        size[a] += size[b];
    }
};

struct SqrtDecomposition {
    static const int B = 100;

    struct SqrtItem {
        int offset;
        UnionFind uf;
        vector<int> representative; // uf vertex of each position
        vector<int> values;         // value of each uf vertex
        // {a[i], representative[a[i]]}
        deque<ii> orderedElements;

        // returns what is at position i
        inline int getValue(int i) {
            return values[uf.find(representative[i])] + offset;
        }

        void build(vector<int> &a) {
            unordered_map<int, int> comp;
            int compressionIdx = 0;
            offset = 0;
            representative.resize(B);

            for (int i = 0; i < B; i++) {
                if (!comp.count(a[i]))
                    comp[a[i]] = compressionIdx++;
                representative[i] = comp[a[i]];
            }

            uf = UnionFind(compressionIdx);
            values.resize(compressionIdx);
            for (int i = 0; i < B; i++)
                values[comp[a[i]]] = a[i];

            sort(all(a));
            a.erase(unique(all(a)), a.end());
            orderedElements.clear();
            for (int i : a)
                orderedElements.push_back({i, comp[i]});
        }
        
        void incrementAll() { offset++; }
        void incrementRange(int l, int r) {
            // reconstructing the range [L, R] this block represents and building again
            vector<int> a(B); 
            for (int i = 0; i < B; i++)
                a[i] = getValue(i) + (l <= i && i <= r);
            build(a);
        }

        int getMaxAll() { return orderedElements.back().first + offset; }
        int getMaxRange(int l, int r) {
            int ret = 0;
            for (int i = l; i <= r; i++)
                ret = max(ret, getValue(i));
            return ret;
        }

        void resetMaxAll(int globalMax) {
            if (getMaxAll() != globalMax) return;
            // the uf group of "repr" has its value "maxi" reduced to 0 (taking care with offset)
            auto[maxi, repr] = orderedElements.back(); orderedElements.pop_back();

            int valToPush = 0-offset;
            values[uf.find(repr)] = valToPush;

            // merging in case any group already represents that value
            if (orderedElements.size() && orderedElements.front().first == valToPush)
                uf.unionSets(repr, orderedElements.front().second);
            else
                orderedElements.push_front({valToPush, repr});
        }
        void resetMaxRange(int l, int r, int globalMax) {
            vector<int> a(B);
            for (int i = 0; i < B; i++) {
                a[i] = getValue(i);
                if (l <= i && i <= r && a[i] == globalMax)
                    a[i] = 0;
            }
            build(a);
        }
    };

    vector<SqrtItem> t;

    SqrtDecomposition(vector<int> &a) {
        int count = (a.size()+B-1)/B; // block count
        a.resize(count * B);
        t.resize(count);
        for (int i = 0; i < count; i++) {
            vector<int> r(a.begin() + i*B, a.begin() + (i+1)*B);
            t[i].build(r);
        }
    }

    int queryMax(int ql, int qr) {
        int bl = ql/B; int br = qr/B;
        int l = ql%B; int r = qr%B; 
        if (bl == br) return t[bl].getMaxRange(l, r);

        int ret = max(t[bl].getMaxRange(l, B-1), t[br].getMaxRange(0, r));
        for (int b = bl+1; b <= br-1; b++)
            ret = max(ret, t[b].getMaxAll());
        return ret;
    }

    void rangeIncrement(int ql, int qr) {
        int bl = ql/B; int br = qr/B;
        int l = ql%B; int r = qr%B; 
        if (bl == br) return (void)t[bl].incrementRange(l, r);

        t[bl].incrementRange(l, B-1), t[br].incrementRange(0, r);
        for (int b = bl+1; b <= br-1; b++)
            t[b].incrementAll();
    }

    int globalMax() {
        int ret = 0;
        for (SqrtItem &s : t)
            ret = max(ret, s.getMaxAll());
        return ret;
    }

    void resetMax(int ql, int qr) {
        int bl = ql/B; int br = qr/B;
        int l = ql%B; int r = qr%B;

        int global = globalMax();
        if (bl == br) return (void)t[bl].resetMaxRange(l, r, global);
        
        t[bl].resetMaxRange(l, B-1, global);
        t[br].resetMaxRange(0, r, global);
        for (int b = bl+1; b <= br-1; b++)
            t[b].resetMaxAll(global);
    }
};

signed main() {
    cin.tie(0)->sync_with_stdio(0);

    int n, k;
    cin >> n >> k;
    vector<int> a(n);
    for (int &i : a) cin >> i;

    SqrtDecomposition sd(a);

    while (k--) {
        char c; int l, r;
        cin >> c >> l >> r;
        l--; r--;
        if (c == 'Q') cout << sd.queryMax(l, r) << '\n';
        else if (c == 'A') sd.rangeIncrement(l, r);
        else sd.resetMax(l, r);
    }
    
    return 0;
}
