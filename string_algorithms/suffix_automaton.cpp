/*
Linear time
Number of different substrings is the amount of paths on the automaton.
d[i] = number of paths starting on vertex 'i'
The recurrence is
d[i] = 1 + (d[next] for each next)
'1' corresponds to the empty string.
Then, the answer would be d[0]-1.
But we can also do it like this: each vertex matches substrings of size [minlen(v), len(v)].
The size of that range is:
len(v)-minlen(v)+1
= len(v) - (len(link(v))+1) + 1
= len(v) - len(link(v)) - 1 + 1
= len(v) - len(link(v))
Don't forget each substring of 's' is matched in one and only one vertex of the SAM.

Smallest cyclic shift: its the smallest |s|-sized path on the SAM of s+s.
*/

struct SuffixAutomaton {
    struct State {
        int len, link;
        map<char, int> next;
    };
    vector<State> st;
    int last = 0;
    SuffixAutomaton(string const &s) {
        st.push_back({0, -1});
        for (char c : s) addLetter(c);
    }

    void addLetter(char c) {
        int cur = st.size();
        st.push_back({st[last].len+1});
        int p = last;
        while (p != -1 && !st[p].next.count(c)) {
            st[p].next[c] = cur;
            p = st[p].link;
        }
        if (p == -1) st[cur].link = 0;
        else {
            int q = st[p].next[c];
            if (st[p].len + 1 == st[q].len) st[cur].link = q;
            else {
                int clone = st.size();
                st.push_back({st[p].len+1, st[q].link, st[q].next});
                while (p != -1 && st[p].next[c] == q) {
                    st[p].next[c] = clone;
                    p = st[p].link;
                }
                st[q].link = st[cur].link = clone;
            }
        }
        last = cur;
    }

    // longest common prefix of 'p' and any substring of 's'
    int lcp(string &p) {
        int pi = 0;
        int v = 0;
        while (pi < p.size()) {
            if (!st[v].next.count(p[pi])) break;
            v = st[v].next[p[pi]];
            pi++;
        }
        return pi;
    }

    int distinctSubstringCount() {
        int ans = 0;
        for (int i = 1; i < st.size(); i++)
            ans += st[i].len - st[st[i].link].len;
        return ans;
    }
};

/*
Greedily traverse smallest edges of the SAM of s+s.
The reason it works is simple: the smallest cyclic shift is a substring of (s+s), and is,
therefore, reachable in a path starting from the root (that follows from the fact that the
SAM accepts every substring).
*/
string smallestCyclicShift(string const &s) {
    SuffixAutomaton sa(s+s);
    int v = 0;
    string ans = "";
    for (int i = 0; i < s.size(); i++) {
        auto[c, next] = *sa.st[v].next.begin();
        ans += c;
        v = next;
    }
    return ans;
}
