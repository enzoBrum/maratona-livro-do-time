class LazySegmentTree {
    struct Segment {
        int min, count, l, r, lazy;
    };

    ii NEUTRAL = {INF, 1};
    vector<Segment> t;

    void build(vector<int> &a, int v, int tl, int tr) {
        t[v] = {INF, 1, tl, tr, 0};
        if (tl == tr)
            t[v].min = a[tl];
        else {
            int mid = (tl + tr) / 2;
            build(a, 2*v, tl, mid);
            build(a, 2*v+1, mid+1, tr);
            merge(v);
        }
    }

    void merge(int v) {
        if (t[2*v].min < t[2*v+1].min) {
            t[v].min = t[2*v].min;
            t[v].count = t[2*v].count;
        }
        else if (t[2*v].min > t[2*v+1].min) {
            t[v].min = t[2*v+1].min;
            t[v].count = t[2*v+1].count;
        }
        else {
            t[v].min = t[2*v].min;
            t[v].count = t[2*v].count + t[2*v+1].count;
        }
    }

    ii merge(ii &a, ii &b) {
        if (a.first < b.first)
            return {a.first, a.second};
        else if (a.first > b.first)
            return {b.first, b.second};
        return {a.first, a.second + b.second};
    }

    void push(int v) {
        if (t[v].lazy) {
            t[v].min += t[v].lazy; // t[v].sum += delta * (t[v].r - t[v].l + 1)
            if (t[v].l != t[v].r) { // not leaf
                t[2*v].lazy += t[v].lazy;
                t[2*v+1].lazy += t[v].lazy;
            }
            t[v].lazy = 0;
        }
    }

    ii query(int v, int l, int r) {
        push(v);
        if (t[v].l > r || t[v].r < l)
            return NEUTRAL;
        if (t[v].l >= l && t[v].r <= r)
            return {t[v].min, t[v].count};
        
        ii left = query(2*v, l, r);
        ii right = query(2*v+1, l, r);
        return merge(left, right);
    }

    void update(int v, int l, int r, int delta) {
        push(v);
        if (t[v].l > r || t[v].r < l)
            return;
        if (t[v].l >= l && t[v].r <= r) {
            t[v].lazy += delta;
            push(v);
            return;
        }
        update(2*v, l, r, delta);
        update(2*v+1, l, r, delta);
        merge(v);
    }

public:
    LazySegmentTree(vector<int> &a) {
        t.resize(4*a.size());
        build(a, 1, 0, a.size()-1);
    }

    ii query(int l, int r) {
        return query(1, l, r);
    }

    void update(int l, int r, int delta) {
        update(1, l, r, delta);
    }
};
