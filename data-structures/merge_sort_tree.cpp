/*
Very simple merge sort tree that implements "count numbers < tgt" on range.
No updates.
Build: O(n log n)
Query: O(log2 n)
The best use for this is querying distinct elements on a range: build a new vector
"prev", such that prev[i] is the previous index where an element equal to a[i] appeared.
The first appearance of an a[i] gets prev[i] == -INF.
Now, querying distinct elements on a range is the same as counting how many have their first
appearance outside of the range [l, r], which is the same as counting how many have prev[i] < l.
*/

class MergeSortTree {
    struct Segment {
        int l, r;
        vector<int> v;
    };
    vector<Segment> t;

    void build(vector<int> &a, int v, int tl, int tr) {
        t[v] = {tl, tr, vector<int>(tr-tl+1, a[tl])};
        if (tl != tr) {
            int mid = (tl + tr) / 2;
            build(a, 2*v, tl, mid);
            build(a, 2*v+1, mid+1, tr);
            merge(all(t[2*v].v), all(t[2*v+1].v), t[v].v.begin());
        }
    }

    int countLessThan(int v, int l, int r, int tgt) {
        if (t[v].l > r || t[v].r < l)
            return 0;
        if (t[v].l >= l && t[v].r <= r)
            return lower_bound(all(t[v].v), tgt) - t[v].v.begin();
        return countLessThan(2*v, l, r, tgt) + countLessThan(2*v+1, l, r, tgt);
    }

public:
    MergeSortTree(vector<int> &a) {
        t.resize(4*a.size());
        build(a, 1, 0, a.size()-1);
    }
    int countLessThan(int l, int r, int t) {
        return countLessThan(1, l, r, t);
    }
};
