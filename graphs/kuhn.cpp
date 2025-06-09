/*
Time complexity: O(VE)
It is very improvable through heuristics. For example, run a loop and match simple edges (i.e., some
simple edge that matches a dude on the left to a dude on the right). Keep track of what left vertices
were used. Then, run Kuhn on the unvisited vertices. If you really feel like it, you can even random
shuffle the heuristics order.

Konig's theorem: the cardinality of a minimum vertex cover of a bipartite graph is the cardinality of
its maximum cardinality bipartite matching.
No proof here.
An algorithm to get the vertex cover itself given a matching is the following:
For every edge in the matching that goes from L->R, if the vertex on R is an ending of ANY alternating
path, then, add it to the vertex cover. Else, add the vertex on L to the vertex cover. Do this for every
edge on the matching and its done.
Remembering that an alternating path is a path that alternates between unmatched and matched edges.
*/

// REGULAR KUHN
// adjacency of LEFT vertices
vector<vector<int>> adj;
// match[i] is the idx of the left vertex matched to the ith right vertex (match.size() == RIGHT)
vector<int> match;
vector<bool> used;

bool tryKuhn(int v) {
    if (used[v])
        return false;
    used[v] = true;
    for (int to : adj[v]) {
        if (match[to] == -1 || tryKuhn(match[to])) {
            match[to] = v;
            return true;
        }
    }
    return false;
}

int getMatching() {
    int matchingSize = 0;
    for (int v = 0; v < adj.size(); ++v) {
        used.assign(adj.size(), false);
        matchingSize += tryKuhn(v);
    }
    return matchingSize;
}

signed main() {
    adj.assign(LEFT, {});
    match.assign(RIGHT, -1);
    int m = getMatching();
}



// HEURISTICS VERSION (everything else is equal)
int getMatching() {
    vector<bool> used1(adj.size(), false);
    int matchingSize = 0;
    for (int v = 0; v < adj.size(); ++v) {
        for (int to : adj[v]) {
            if (match[to] == -1) {
                match[to] = v;
                used1[v] = true;
                matchingSize++;
                break;
            }
        }
    }
    for (int v = 0; v < adj.size(); ++v) {
        if (used1[v])
            continue;
        used.assign(adj.size(), false);
        matchingSize += tryKuhn(v);
    }
    return matchingSize;
}