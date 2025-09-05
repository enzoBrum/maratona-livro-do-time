/*
This one implements:
 - Range maximum query
 - Range increment (a[i]++)
 - Range max reset (a[i] = 0 iff a[i] == maximum on WHOLE array)
*/

struct SqrtDecomposition {
    static const int B = 100;

    struct SqrtItem {
        int offset;
        UnionFind uf;
        vector<int> representative; // uf vertex of each position
        vector<int> values;         // value of each uf vertex
        // {a[i], representative[a[i]]}
        deque<ii> orderedElements;

        // returns what is at position i
        inline int getValue(int i) {
            return values[uf.find(representative[i])] + offset;
        }

        void build(vector<int> &a) {
            unordered_map<int, int> comp;
            int compressionIdx = 0;
            offset = 0;
            representative.resize(B);

            for (int i = 0; i < B; i++) {
                if (!comp.count(a[i]))
                    comp[a[i]] = compressionIdx++;
                representative[i] = comp[a[i]];
            }

            uf = UnionFind(compressionIdx);
            values.resize(compressionIdx);
            for (int i = 0; i < B; i++)
                values[comp[a[i]]] = a[i];

            sort(all(a));
            a.erase(unique(all(a)), a.end());
            orderedElements.clear();
            for (int i : a)
                orderedElements.push_back({i, comp[i]});
        }
        
        void incrementAll() { offset++; }
        void incrementRange(int l, int r) {
            // reconstructing the range [L, R] this block represents and building again
            vector<int> a(B); 
            for (int i = 0; i < B; i++)
                a[i] = getValue(i) + (l <= i && i <= r);
            build(a);
        }

        int getMaxAll() { return orderedElements.back().first + offset; }
        int getMaxRange(int l, int r) {
            int ret = 0;
            for (int i = l; i <= r; i++)
                ret = max(ret, getValue(i));
            return ret;
        }

        void resetMaxAll(int globalMax) {
            if (getMaxAll() != globalMax) return;
            // the uf group of "repr" has its value "maxi" reduced to 0 (taking care with offset)
            auto[maxi, repr] = orderedElements.back(); orderedElements.pop_back();

            int valToPush = 0-offset;
            values[uf.find(repr)] = valToPush;

            // merging in case any group already represents that value
            if (orderedElements.size() && orderedElements.front().first == valToPush)
                uf.unionSets(repr, orderedElements.front().second);
            else
                orderedElements.push_front({valToPush, repr});
        }
        void resetMaxRange(int l, int r, int globalMax) {
            vector<int> a(B);
            for (int i = 0; i < B; i++) {
                a[i] = getValue(i);
                if (l <= i && i <= r && a[i] == globalMax)
                    a[i] = 0;
            }
            build(a);
        }
    };

    vector<SqrtItem> t;

    SqrtDecomposition(vector<int> &a) {
        int count = (a.size()+B-1)/B; // block count
        a.resize(count * B);
        t.resize(count);
        for (int i = 0; i < count; i++) {
            vector<int> r(a.begin() + i*B, a.begin() + (i+1)*B);
            t[i].build(r);
        }
    }

    int queryMax(int ql, int qr) {
        int bl = ql/B; int br = qr/B;
        int l = ql%B; int r = qr%B; 
        if (bl == br) return t[bl].getMaxRange(l, r);

        int ret = max(t[bl].getMaxRange(l, B-1), t[br].getMaxRange(0, r));
        for (int b = bl+1; b <= br-1; b++)
            ret = max(ret, t[b].getMaxAll());
        return ret;
    }

    void rangeIncrement(int ql, int qr) {
        int bl = ql/B; int br = qr/B;
        int l = ql%B; int r = qr%B; 
        if (bl == br) return (void)t[bl].incrementRange(l, r);

        t[bl].incrementRange(l, B-1), t[br].incrementRange(0, r);
        for (int b = bl+1; b <= br-1; b++)
            t[b].incrementAll();
    }

    int globalMax() {
        int ret = 0;
        for (SqrtItem &s : t)
            ret = max(ret, s.getMaxAll());
        return ret;
    }

    void resetMax(int ql, int qr) {
        int bl = ql/B; int br = qr/B;
        int l = ql%B; int r = qr%B;

        int global = globalMax();
        if (bl == br) return (void)t[bl].resetMaxRange(l, r, global);
        
        t[bl].resetMaxRange(l, B-1, global);
        t[br].resetMaxRange(0, r, global);
        for (int b = bl+1; b <= br-1; b++)
            t[b].resetMaxAll(global);
    }
};
