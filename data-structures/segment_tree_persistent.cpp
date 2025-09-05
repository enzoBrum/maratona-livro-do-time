/*
Persistent segment tree
Each update makes log(n) vertex copies and returns the new root of the modified tree.
About computing kth element on range:
First, build n prefix segment trees, each one storing the frequency of every number on
the prefix.
Then, if you wanted to compute the kth element on a PREFIX, you would simply do the
regular traversal where you go to the left children if there are >= k guys there, or go
to the right one but trying to find the (k-leftCount)th element.
Now, to extend that idea but to kth on range, instead of grabbing "left" as the count on
the prefix, you simply grab "left" as the prefix count on the frequency tree corresponding
to "r" minus the frequency tree of "l-1". Refer to code for better understanding.
*/

struct Vertex {
    Vertex *l, *r;
    int sum = 0;
    Vertex(int val) : l(nullptr), r(nullptr), sum(val) {}
    Vertex(Vertex *l, Vertex *r) : l(l), r(r) {
        if (l) sum += l->sum;
        if (r) sum += r->sum;
    }
};

Vertex* build(vector<int> const &a, int tl, int tr) {
    if (tl == tr) return new Vertex(a[tl]);
    int mid = (tl + tr) / 2;
    return new Vertex(build(a, tl, mid), build(a, mid+1, tr));
}

Vertex* update(Vertex* v, int tl, int tr, int pos, int delta) {
    if (tl == tr) return new Vertex(v->sum+delta);
    int mid = (tl + tr) / 2;
    if (pos <= mid) return new Vertex(update(v->l, tl, mid, pos, delta), v->r);
    return new Vertex(v->l, update(v->r, mid+1, tr, pos, delta));
}

int findKth(Vertex* vl, Vertex *vr, int tl, int tr, int k) {
    if (tl == tr) return tl;
    int mid = (tl + tr) / 2;
    int leftCount = vr->l->sum - vl->l->sum;
    if (leftCount >= k) return findKth(vl->l, vr->l, tl, mid, k);
    return findKth(vl->r, vr->r, mid+1, tr, k-leftCount);
}

int querySum(Vertex *v, int tl, int tr, int l, int r) {
    if (tl > r || tr < l) return 0;
    if (tl >= l && tr <= r) return v->sum;
    int mid = (tl + tr) / 2;
    return querySum(v->l, tl, mid, l, r) + querySum(v->r, mid+1, tr, l, r);
}

struct PersistentSegmentTree {
    vector<Vertex*> pref;
    int tl, tr;
    PersistentSegmentTree() {}
    PersistentSegmentTree(vector<int> &a, int maxVal) : pref(a.size()+1), tl(0), tr(maxVal+1) {
        int n = a.size();
        pref[0] = build(vector<int>(tr), tl, tr);
        for (int i = 1; i <= n; i++)
            pref[i] = update(pref[i-1], tl, tr, a[i-1], 1);
    }
    int findKth(int l, int r, int k) {
        return ::findKth(pref[l], pref[r+1], tl, tr, k);
    }
    // return amount of elements small <= a[i] <= big on range [l, r]
    int countOnRange(int l, int r, int small, int big) {
        return querySum(pref[r+1], tl, tr, small, big) - querySum(pref[l], tl, tr, small, big);
    }
};
