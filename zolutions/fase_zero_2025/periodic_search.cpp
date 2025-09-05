/*
Exercise:
Given a trie, compute the maximum period out of all the periods of strings from
the root to any other node. The period must be counted only if it is not the
string itself (to avoid a period that isn't even a repetition of itself).

Idea:
Let's build a KMP with rollbacks and dfs the tree. The only problem is that some
additions may be up to linear in regular KMP, so let's use an automaton that tells
us what the lps becomes when your current lps is 'i' and you append the character 'c'.
Store that in aut[i][c]. Clearly, we can compute the regular lps vector as
aut[lastLps][ith letter]
but now everything is O(1) instead of a possible O(n) worst case.
*/

vector<vector<pair<char, int>>> adj;
int maxPeriod = 0;
stack<pair<int*, int>> history;
vector<int> lps;
vector<vector<int>> aut;
string s = "";

void change(int &x, int val) {
    history.push({&x, x});
    x = val;
}

void rollback() {
    auto[ptr, val] = history.top(); history.pop();
    *ptr = val;
}

void kmpAdd(char letter, int curLps) {
    s += letter;
    int i = curLps;
    if (s.size() != 1) {
        for (int c = 0; c < 26; c++) {
            if (i > 0 && 'a' + c != s[i])
                change(aut[i][c], aut[lps[i-1]][c]);
            else
                change(aut[i][c], i + ('a' + c == s[i]));
        }
    }
    change(lps[s.size()-1], aut[i][letter-'a']);
}

void kmpRemove(int stSize) {
    s.pop_back();
    while (history.size() != stSize)
        rollback();
}

void dfs(int u, int curLps) {
    int k = s.size() - curLps;
    if (k && s.size() % k == 0 && s.size() != k)
        maxPeriod = max(maxPeriod, k);

    for (auto[c, next] : adj[u]) {
        int sz = history.size();
        kmpAdd(c, curLps);
        int nextLps = aut[curLps][c-'a'];
        dfs(next, nextLps);
        kmpRemove(sz);
    }
}

signed main() {
    cin.tie(0)->sync_with_stdio(0);

    int n;
    cin >> n;
    vector<int> p(n-1);
    for (int &i : p) cin >> i;
    string s;
    cin >> s;

    adj.assign(n, {});
    for (int i = 0; i < n-1; i++)
        adj[p[i]-1].push_back({s[i], i+1});

    lps.assign(n, 0);
    aut.assign(n, vector<int>(26, 0));
    dfs(0, 0);

    cout << maxPeriod << '\n';
    
    return 0;
}