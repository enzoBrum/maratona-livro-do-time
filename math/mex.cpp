struct MexSet {
    map<int, int> inSet;
    set<int> notInSet;

    // n should be the maximum possible mex (aka set size)
    MexSet(int n) {
        for (int i = 0; i <= n; i++)
            notInSet.insert(i);
    }

    void insert(int elem) {
        inSet[elem]++;
        if (notInSet.count(elem))
            notInSet.erase(elem);
    }

    void erase(int elem) {
        int count = --inSet[elem];
        if (!count)
            notInSet.insert(elem);
    }

    int mex() {
        assert(notInSet.size() > 0);
        return *notInSet.begin();
    }
};
