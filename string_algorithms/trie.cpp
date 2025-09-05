struct Trie {
    struct Node {
        int count, mostFrequentContinuationCount, mostFrequentContinuationId, indexOnOrderedVector;
        map<char, int> next;
    };
    vector<Node> t;

    Trie() { addNode(); } // you can reserve() here too

    int addNode() {
        t.push_back({0, -1, -1, 0, {}});
        return t.size()-1;
    }

    void addWord(string const &s, int idx) {
        int cur = 0; 
        for (char c : s) {
            if (!t[cur].next.count(c))
                t[cur].next[c] = addNode();
            cur = t[cur].next[c];
        }
        t[cur].count++;
        t[cur].indexOnOrderedVector = idx;
    }

    int getId(string const &s) {
        int cur = 0;
        for (char c : s) {
            if (!t[cur].next.count(c)) return -1;
            cur = t[cur].next[c];
        }
        return cur;
    }

    // update 'mostFrequentContinuation' for every prefix of s
    void relax(string const &s) {
        int id = getId(s);
        int currentCount = t[id].count;
        int cur = 0;
        for (char c : s) {
            cur = t[cur].next[c];
            if (t[cur].mostFrequentContinuationCount < currentCount) {
                t[cur].mostFrequentContinuationCount = currentCount;
                t[cur].mostFrequentContinuationId = id;
            }
        }
    }
};
