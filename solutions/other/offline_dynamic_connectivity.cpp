/*
Offline dynamic connectivity. O(q log q log n)
You wanna know how many connected components a graph has and insert/delete edges efficiently. What you
will do is the following: keep something similar to a "segment tree" of the queries. Then, read the
queries and keep track of the timespan of an edge. Update the segment tree and say "edge (u,v) lasted
between the lth and rth query", that is, push back the edge (u, v) on each of the logn segments that
range is composed of. Then, do a dfs from the root to each of the 'q' leaves of the segment tree. On
the way down, add the edges to your graph, and when coming back, remove them. When you reach the leaf,
you have exactly all the edges the query should have in that moment. Print the amount of connected
components and you are done.

On this exercise specifically, you wanna know how many edges you need to insert onto A until it contains B.
Say the result graph is a graph made by adding onto A every edge (x,y) such that (x,y) was on B but (x,y) were
not connected on A. The answer would be the difference between the amount of connected components on A and on
that resulting graph. But since adding the edges (x,y) that did not hold that property is neutral, then, that
resulting graph has the same connected components as the graph AUB (adding everyone from B onto A). So now, the
answer becomes the difference in connected components of A and AUB, which is easier to compute.
We have a list of edges that should be on graphs A and B for each query on the segment tree. You simply add A's
edges onto A and AUB, and B's edges to AUB. Print their difference in components and that's it.
*/

#include <bits/stdc++.h>

using namespace std;

#define int long long
const int INF = 0x3F3F3F3F;
using ii = pair<int, int>;
using ll = long long;
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()

const bool DEBUG = 0;

struct RollbackUnionFind {
    vector<int> parent, size;
    int components;
    stack<pair<int*, int>> history;
    
    RollbackUnionFind(int n=0) {
        parent.resize(n);
        size.assign(n, 1);
        iota(all(parent), 0);
        components = n;
    }
    
    int find(int v) {
        if (v == parent[v])
            return v;
        return find(parent[v]);
    }

    void change(int &x, int newVal) {
        history.push({&x, x}); // x's current state
        x = newVal;
    }

    void rollback() {
        auto[ptr, val] = history.top(); history.pop();
        *ptr = val;
    }
    
    void unionSets(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return;
        if (size[a] < size[b]) swap(a, b);
        change(parent[b], a);
        change(size[a], size[a]+size[b]);
        change(components, components-1);
    }
};

struct Edge {
    char c;
    int x, y;
    Edge(char c, int x, int y) : c(c), x(x), y(y) {}
};

class SegmentTree {
    struct Segment {
        int l, r;
        vector<Edge> edg;
    };

    vector<Segment> t;
    RollbackUnionFind A, AUB;

    void build(int v, int tl, int tr) {
        t[v] = {tl, tr, {}};
        if (tl != tr) {
            int mid = (tl + tr) / 2;
            build(2*v, tl, mid);
            build(2*v+1, mid+1, tr);
        }
    }

    void addEdge(int v, int l, int r, Edge &e) {
        if (t[v].l > r || t[v].r < l)
            return;
        if (t[v].l >= l && t[v].r <= r) {
            t[v].edg.push_back(e);
            return;
        }
        addEdge(2*v, l, r, e);
        addEdge(2*v+1, l, r, e);
    }

    void dfs(int v) {
        int Astate = A.history.size();
        int AUBstate = AUB.history.size();
        for (Edge &e : t[v].edg) {
            AUB.unionSets(e.x, e.y);
            if (e.c == 'A')
                A.unionSets(e.x, e.y);
        }
        if (t[v].l == t[v].r) {
            // at leaf. Compute answer.
            ans[t[v].l] = A.components - AUB.components;
        }
        else {
            dfs(2*v); dfs(2*v+1);
        }

        while (A.history.size() != Astate) A.rollback();
        while (AUB.history.size() != AUBstate) AUB.rollback();
    }

public:
    vector<int> ans;
    SegmentTree(int sz, int v) {
        t.resize(4*sz);
        ans.resize(sz);
        build(1, 0, sz-1);
        A = RollbackUnionFind(v);
        AUB = RollbackUnionFind(v);
    }

    void addEdge(int l, int r, Edge e) {
        addEdge(1, l, r, e);
    }

    void dfs() {
        dfs(1);
    }
};

signed main() {
    cin.tie(0)->sync_with_stdio(0);

    int n, q;
    cin >> n >> q;

    vector<map<ii, int>> pos(2);
    SegmentTree st(q, n);

    for (int cur = 0; cur < q; cur++) {
        char c;
        int x, y;
        cin >> c >> x >> y;
        x--; y--;
        if (x > y) swap(x, y);
        int id = c-'A';
        if (pos[id].count({x, y})) {
            st.addEdge(pos[id][{x, y}], cur-1, {c, x, y}); // cur-1 because its last appearance is on the previous query
            pos[id].erase({x, y});
        }
        else {
            pos[id][{x, y}] = cur;
        }
    }

    // add every remaining edge to q-1
    for (int i = 0; i < 2; i++)
        for (auto[edg, idx] : pos[i])
            st.addEdge(idx, q-1, {char(i+'A'), edg.first, edg.second});
    
    st.dfs();
    for (int i : st.ans)
        cout << i << '\n';

    return 0;
}
