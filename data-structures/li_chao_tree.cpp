struct Function {
    // define here your function (which may be a line). Any pair of functions must
    // intersect at most once.
    ii dish;
    int id;
    Function(ii dish, int id) : dish(dish), id(id) {}
    double eval(int x) const {
        return 1.0*(customers[x].value.first * dish.first + customers[x].value.second * dish.second) / (customers[x].value.first + customers[x].value.second);
    }
    bool isBetterThan(Function const &o, int x, bool isMin) const {
        // caution when you have a tiebreaker (on this case id)
        //if (fabs(this->eval(x)-o.eval(x)) < 1e-9)
        //    return this->id < o.id;
        // regular template code here
        if (isMin) return this->eval(x) < o.eval(x);
        else return this->eval(x) > o.eval(x);
    }
};

struct LiChaoTree {
    vector<Function> t;
    int n, isMin;

    Function best(Function const &a, Function const &b, int x) {
        return (a.isBetterThan(b, x, isMin)) ? a : b;
    }

    // n is the size. Allows indexing up to n-1
    LiChaoTree(int n, bool isMin) : n(n), isMin(isMin) {
        #warning Change Function init so it defaults to the opposite of your LiChaoTree
        if (isMin) t.assign(4*n, Function({-INF, -INF}, INF));
        else t.assign(4*n, Function({0, 0}, INF));
    }

    void addLine(Function nw) { addLine(nw, 1, 0, n-1); }
    void addLine(Function nw, int v, int l, int r) {
        int m = (l + r) / 2;
        bool lef = nw.isBetterThan(t[v], l, isMin);
        bool mid = nw.isBetterThan(t[v], m, isMin);
        if (mid) swap(t[v], nw);
        if (r == l) return;
        if (lef != mid) addLine(nw, 2 * v, l, m);
        else addLine(nw, 2 * v + 1, m+1, r);
    }

    Function get(int x) { return get(x, 1, 0, n-1); }
    Function get(int x, int v, int l, int r) {
        int m = (l + r) / 2;
        if (r == l) return t[v];
        if (x <= m) return best(t[v], get(x, 2 * v, l, m), x);
        return best(t[v], get(x, 2 * v + 1, m+1, r), x);
    }
};
