#include <bits/stdc++.h>

using namespace std;

#define int long long
const int INF = 0x3F3F3F3F;
using ii = pair<int, int>;
using ll = long long;
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()

const bool DEBUG = true;

/*
Exercise:
Given a tree, you can spend an operation to remove a vertex and everyone above him. Remove the maximum
amount of vertices with 'k' operations.

Idea:
If you have a forest and a single query, you know that the best thing you can do is remove the tallest
path (i.e., the path from the vertex with most vertices below it, and everyone below it). I won't prove it,
but that is the optimal greedy strategy (even for several picks).
Therefore, what you will do is grab the vertices from tallest to shortest, and, if it wasn't picked before,
mark is as "visited" and pick everyone below him.
*/

const int BOSS = 0;
const int NO_CHILD = -1;
const int MAXN = 1e5;

vector<vector<int>> adj;
priority_queue<ii> pq;
int depth[MAXN];
int son[MAXN];
int visited[MAXN];

void dfs(int u) {
    // depth[u] starts as 0, I'll increment it later
    for (int next : adj[u]) {
        dfs(next);
        if (depth[next] >= depth[u]) {
            depth[u] = depth[next];
            son[u] = next;
        }
    }
    depth[u]++;
    pq.push({depth[u], u});
}

signed main() {
    cin.tie(0)->sync_with_stdio(0);

    int n, k;
    cin >> n >> k;
    adj.assign(n, {});

    for (int i = 1; i < n; i++) {
        int p;
        cin >> p;
        p--;
        // only putting edges downwards
        adj[p].push_back(i);
    }

    memset(son, NO_CHILD, sizeof(son));
    dfs(BOSS);

    int ans = 0;
    while (k) {
        if (!pq.size()) break;
        auto[depth, node] = pq.top(); pq.pop();
        if (!visited[node]) {
            ans += depth;
            k--;
            while (node != NO_CHILD) {
                visited[node] = 1;
                node = son[node];
            }
        }
    }

    cout << ans << '\n';
    
    return 0;
}
