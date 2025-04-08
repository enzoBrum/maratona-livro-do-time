struct Trie {
    vector<map<char, int>> next;
    vector<int> count, val, mostFrequentCount, indexOfMostFrequent;

    Trie() { 
        next.reserve(1e6);
        count.reserve(1e6);
        val.reserve(1e6);
        mostFrequentCount.reserve(1e6);
        indexOfMostFrequent.reserve(1e6);
        addNode(); 
    }

    int addNode() {
        next.push_back({});
        count.push_back(0);
        val.push_back(0);
        mostFrequentCount.push_back(0);
        indexOfMostFrequent.push_back(0);

        return next.size()-1;
    }

    void addWord(string const &s, int v = 0) {
        int cur = 0; 
        for (char c : s) {
            if (!next[cur].count(c))
                next[cur][c] = addNode();
            cur = next[cur][c];
        }
        count[cur]++;
        val[cur] = v;
    }

    int getId(string const &s) {
        int cur = 0;
        for (char c : s) {
            if (!next[cur].count(c)) return -1;
            cur = next[cur][c];
        }
        return cur;
    }

    void relaxValue(string const &s, int count, int idx) {
        int cur = 0;
        for (char c : s) {
            cur = next[cur][c];
            if (count > mostFrequentCount[cur]) {
                mostFrequentCount[cur] = count;
                indexOfMostFrequent[cur] = idx;
            }
        }
    }
};
