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

// Within main:
adj.assign(LEFT, {});
// Build adj here (remembering only left pointing to right)
match.assign(RIGHT, -1);
int matchingSize = 0;
for (int v = 0; v < LEFT; ++v) {
    used.assign(LEFT, false);
    matchingSize += tryKuhn(v);
}






// With heuristics, main would be something like this:
adj.assign(LEFT, {});
match.assign(RIGHT, -1);

vector<bool> used1(n, false);
for (int v = 0; v < LEFT; ++v) {
    for (int to : adj[v]) {
        if (match[to] == -1) {
            match[to] = v;
            used1[v] = true;
            break;
        }
    }
}
int matchingSize = 0;
for (int v = 0; v < LEFT; ++v) {
    if (used1[v])
        continue;
    used.assign(LEFT, false);
    matchingSize += tryKuhn(v);
}
