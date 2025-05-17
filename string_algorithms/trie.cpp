struct Trie {
    vector<map<char, int>> next;
    vector<int> count;
    vector<int> mostFrequentContinuationCount;
    vector<int> mostFrequentContinuationId;
    vector<int> indexOnOrderedVector;

    Trie() { addNode(); } // you can reserve the vectors here if you need to

    int addNode() {
        next.push_back({});
        count.push_back(0);
        mostFrequentContinuationCount.push_back(-1);
        mostFrequentContinuationId.push_back(-1);
        indexOnOrderedVector.push_back(0);
        return next.size()-1;
    }

    void addWord(string const &s, int idx) {
        int cur = 0; 
        for (char c : s) {
            if (!next[cur].count(c))
                next[cur][c] = addNode();
            cur = next[cur][c];
        }
        count[cur]++;
        indexOnOrderedVector[cur] = idx;
    }

    int getId(string const &s) {
        int cur = 0;
        for (char c : s) {
            if (!next[cur].count(c)) return -1;
            cur = next[cur][c];
        }
        return cur;
    }

    int getCount(string const &s) {
        return count[getId(s)];
    }

    // update 'mostFrequentContinuation' for every prefix of s
    void relax(string const &s) {
        int id = getId(s);
        int currentCount = count[id];
        int cur = 0;
        for (char c : s) {
            cur = next[cur][c];
            if (mostFrequentContinuationCount[cur] < currentCount) {
                mostFrequentContinuationCount[cur] = currentCount;
                mostFrequentContinuationId[cur] = id;
            }
        }
    }
};
