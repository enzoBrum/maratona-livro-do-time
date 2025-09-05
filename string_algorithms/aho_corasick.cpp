// Linear time
// To find all patterns that are on a string, you have to walk output links
// after adding the ith character of the text (that is, walk output links after
// calling v = go(v, s[i])). That will make it quadratic, but sometimes you can
// compress data as matchAcc or a hashed frequency vector.

// link: points to the longest proper suffix that also belongs to the trie
// exit: points to the longest link that is the end of a word on the trie
// matchAt: matches exactly at the vertex
// matchAcc: matchAt + all matches going up suffix links
#warning this one is set up for lowercase characters only
struct AhoCorasick {
    const static int K = 26;
    struct Vertex {
        array<int, K> next, go;
        int link = -1, exit = -1, matchAt = 0, matchAcc = 0;
        Vertex() { next.fill(-1); go.fill(-1); }
    };
    vector<Vertex> t;

    AhoCorasick(vector<string> &pats) : t(1) {
        t.reserve(2001);
        for (string &s : pats) addString(s);
        queue<int> q; q.push(0);
        for (int i = 0; i < K; i++)
            t[0].go[i] = max(0, t[0].next[i]);
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (int c = 0; c < K; c++) {
                int j = t[v].next[c];
                if (j == -1) continue;
				int l = t[v].link;
				while (l != -1 && t[l].next[c] == -1) l = t[l].link;
                t[j].link = l == -1 ? 0 : t[l].next[c];
                t[j].exit = t[t[j].link].matchAt ? t[j].link : t[t[j].link].exit;
                if (t[j].exit != -1) t[j].matchAcc += t[t[j].exit].matchAcc;
                for (int k = 0; k < K; k++)
                    t[j].go[k] = t[j].next[k] != -1 ? t[j].next[k] : t[t[j].link].go[k];
                q.push(j);
            }
        }
    }

    void addString(string const& s) {
        int v = 0;
        for (char ch : s) {
            int c = ch - 'a';
            if (t[v].next[c] == -1) {
                t[v].next[c] = t.size();
                t.emplace_back();
            }
            v = t[v].next[c];
        }
        t[v].matchAt++; t[v].matchAcc++;
    }

    int go(int v, char c) { return t[v].go[c-'a']; }
};
