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
You are given a 16x16 board with white and black cells. White cells have values in [1, 9]. Black cells
may have "vertical" or "horizontal" constraints: a vertical constraint says that the sum of the contiguous
white cells below the black cell must be the sum specified by the constraint. Same for horizontal towards
the right.
You wanna compute the board that differs the less from the given board but that is also a valid solution.
Board difference is the sum of the abs difference of their white cells.
Important: every white cell belongs to exactly 1 horizontal and 1 vertical constraint.

Idea:
Let's create a flow network, where the flow will go through iff there is an arrangement of white cells that
satisfies everything.
First of all, we will reweight the white vertices and the constraints. The exercise wants at least 1 unit
per white cell, so, we simply decrease all of them by 1 (updating constraints) and reduce our problem to 
regular max flow.
Each white cell is represented by its in and out vertices. We (initially) put capacity 8 on its edge
(because the exercise says each white vertex can have a number in [1, 9] (after reweight, in [0, 8])).
Then, we plug each V constraint to the in of the white cells it reaches. We do the same for the H
constraints and the out of each white cell.
We will plug the source to the V constraints, edges with capacity equal to the constraint themselves. Same
for horizontal and sink.
It is clear that if we can make the sum of the V constraints go through it (and given that sum(V) == sum(H)),
then, there is a solution.
Now, to minimize cost: instead of a single white edge of capacity 8, we will put 2 edges, one with capacity
equal to whatever is on the board already (and cost -1 per unit of flow), and another with 8-that and cost
1 per unit of flow.
The solution is assuming you pay for all of the values of the target cells + the min cost flow. The idea is
that whatever flow goes through an edge, but less than the target, will discount its cost. Whatever goes above
it, will increase it again.
This also means you can use this technique to solve any problem where the total flow cost per edge is a
piecewise linear function (concave up).

Complexity: O(F*V*E)
F = 2048 (at most 8 per white board cell, i.e. 8*16*16)
V = 1538 (1 + V + 4*W + H + 1) = (2 + 2*16² + 4*16²)
E = 1792 (V + W + 2*W + W + W + H) = (V + H + 5*W) = (2*16² + 5*16²)
total = 5.644.484.608 = 56e8
But it is the real worst case of ford fulkerson AND spfa...

We will need 2 edges between w(j)in and w(j)out. Therefore, I will make two dummy vertices for each
w(j), called w(j)(a) and w(j)(b), each one holding an edge.

| LAYER 1 | LAYER 2 | LAYER 3 | LAYER 4 | LAYER 5 | LAYER 6 | LAYER 7 |
|         |    v1   |  w1in   |  w1a/b  |  w1out  |    h1   |         |
|         |    v2   |  w2in   |  w2a/b  |  w2out  |    h1   |         |
|         |    v3   |  w3in   |  w3a/b  |  w3out  |    h1   |         |
|    s    |    v4   |  w4in   |  w4a/b  |  w4out  |    h1   |    t    |
|         |    v5   |  w5in   |  w5a/b  |  w5out  |    h1   |         |
|         |    v6   |  w6in   |  w6a/b  |  w6out  |    h1   |         |
|         |    v7   |  w7in   |  w7a/b  |  w7out  |    h1   |         |
|    1    |    V    |    W    |   2*W   |    W    |    H    |    1    |
*/

// O(FVE)
struct MinCostMaxFlow {
    bool spfa(int s, int t) {
        int n = adj.size();
        d.assign(n, INF);
        vector<bool> inqueue(n, false);
        queue<int> q;
        p.assign(n, -1);

        d[s] = 0;
        q.push(s);
        inqueue[s] = true;
        while (!q.empty()) {
            int v = q.front(); q.pop();
            inqueue[v] = false;

            for (int to : adj[v]) {
                int len = cost[v][to];
                if (d[v] != INF && d[v] + len < d[to] && cap[v][to] > 0) {
                    d[to] = d[v] + len;
                    p[to] = v;
                    if (!inqueue[to]) {
                        q.push(to);
                        inqueue[to] = true;
                    }
                }
            }
        }

        return d[t] != INF;
    }

    vector<vector<int>> adj, cap, cost;
    vector<int> p, d;
    int n;

    MinCostMaxFlow(vector<vector<int>> &adj, vector<vector<int>> &cap, vector<vector<int>> &cost) : adj(adj), cap(cap), cost(cost) {
        n = adj.size();
    }

    // {flow, cost}
    ii maxflow(int s, int t) {
        int maxflow = 0;
        int cost = 0;

        while (spfa(s, t)) {
            int increment = INF;
            for (int u = t; p[u] >= 0; u = p[u])
                increment = min(increment, cap[p[u]][u]);

            for (int u = t; p[u] >= 0; u = p[u]) {
                cap[p[u]][u] -= increment;
                cap[u][p[u]] += increment;              
            }
            cost += increment * d[t];
            maxflow += increment;
        }
        return {maxflow, cost};
    }
};

signed main() {
    cin.tie(0)->sync_with_stdio(0);

    int m, n, s;
    cin >> m >> n >> s;

    vector<vector<int>> grid(m, vector<int>(n, 0));
    vector<vector<int>> whiteIdx(m, vector<int>(n, -1));
    vector<ii> whitePosition;
    int whiteCellsSum = 0;

    for (int i = 0; i < m; i++) {
        string line;
        cin >> line;
        for (int j = 0; j < n; j++) {
            grid[i][j] = line[j]-'0';
            if (grid[i][j] != 0) {
                whiteIdx[i][j] = whitePosition.size();
                whitePosition.push_back({i, j});
                grid[i][j]--; // applying -1 white cell correction
                whiteCellsSum += grid[i][j];
            }
        }
    }

    struct Constraint { int i, j, sum; };
    vector<Constraint> v, h;
    while (s--) {
        char c;
        int i, j, sum;
        cin >> c >> i >> j >> sum;
        i--; j--;
        if (c == 'V') v.push_back({i, j, sum});
        else h.push_back({i, j, sum});
    }

    const int W = whitePosition.size();
    const int V = v.size();
    const int H = h.size();
    const int SIZE = 1 + V + W + 2*W + W + H + 1;

    vector<vector<int>> adj(SIZE);
    vector<vector<int>> cap(SIZE, vector<int>(SIZE, 0));
    vector<vector<int>> cost(SIZE, vector<int>(SIZE, 0));

    const int SOURCE = SIZE-2;
    const int SINK = SIZE-1;
    // LAYER 2:   [0, V)
    // LAYER 3:   [V, V+W)
    // LAYER 4 a: [V+W, V+2*W)
    // LAYER 4 b: [V+2*W, V+3*W)
    // LAYER 5:   [V+3*W, V+4*W)
    // LAYER 6:   [V+4*W, V+4*W+H)
    auto vid =    [&](int u) { return u; };
    auto winid =  [&](int u) { return u+V; };
    auto waid =   [&](int u) { return u+V+W; };
    auto wbid =   [&](int u) { return u+V+2*W; };
    auto woutid = [&](int u) { return u+V+3*W; };
    auto hid =    [&](int u) { return u+V+4*W; };

    auto addEdge = [&](int u, int v, int w, int c) {
        adj[u].push_back(v);
        adj[v].push_back(u);
        cost[u][v] = c;
        cost[v][u] = -c;
        cap[u][v] = w;
    };
    
    int vsum = 0;
    for (int i = 0; i < v.size(); i++) {
        // LAYER 2->3, vertical to win
        auto[r, c, sum] = v[i];
        int connected = 0;
        for (int j = r+1; j < m && whiteIdx[j][c] != -1; j++) {
            int idx = whiteIdx[j][c];
            addEdge(vid(i), winid(idx), sum, 0);
            connected++;
        }

        // LAYER 1->2 source to vertical
        int vEdgeCap = v[i].sum - connected;
        addEdge(SOURCE, vid(i), vEdgeCap, 0);
        vsum += vEdgeCap;
    }
    
    for (int i = 0; i < W; i++) {
        // LAYER 3->4, win to wa/b
        addEdge(winid(i), waid(i), INF, 0);
        addEdge(winid(i), wbid(i), INF, 0);

        // LAYER 4->5, wa/b to wout
        auto[x, y] = whitePosition[i];
        int cheapCapacity = grid[x][y];
        int expensiveCapacity = 8-cheapCapacity;
        addEdge(waid(i), woutid(i), cheapCapacity, -1);
        addEdge(wbid(i), woutid(i), expensiveCapacity, 1);
    }

    int hsum = 0;
    for (int i = 0; i < h.size(); i++) {
        // LAYER 5->6, wout to h
        auto[r, c, sum] = h[i];
        int connected = 0;
        for (int j = c+1; j < n && whiteIdx[r][j] != -1; j++) {
            int idx = whiteIdx[r][j];
            addEdge(woutid(idx), hid(i), INF, 0);
            connected++;
        }

        // LAYER 6->7, h to SINK
        int hEdgeCap = h[i].sum - connected;
        addEdge(hid(i), SINK, hEdgeCap, 0);
        hsum += hEdgeCap;
    }

    MinCostMaxFlow mf(adj, cap, cost);
    auto[flow, flowcost] = mf.maxflow(SOURCE, SINK);
    if (vsum == hsum && vsum == flow)
        cout << flowcost + whiteCellsSum << '\n';
    else
        cout << "IMPOSSIBLE\n";

    return 0;
}