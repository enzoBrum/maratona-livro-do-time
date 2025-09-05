struct Function {
    // define here your function (which may be a line). Any pair of functions must
    // intersect at most once.
    bool isDefaultLine = false;
    int slope, starty;
    Function() : isDefaultLine(true) {}
    Function(int slope, int starty) : slope(slope), starty(starty) {}
    // LiChao itself doesn't call eval, so you can do anything here
    __int128 eval(int x) const {
        return (__int128)x*slope + starty;
    }
    bool isBetterThan(Function const &o, int x, bool isMin) const {
        if (o.isDefaultLine) return true; // everyone overrides default lines
        if (isDefaultLine) return false;
        if (isMin) return this->eval(x) < o.eval(x);
        else return this->eval(x) > o.eval(x);
    }
};

struct LiChaoTree {
    struct Vertex {
        Function f;
        int l, r, isMin;
        Vertex *leftChild = nullptr;
        Vertex *rightChild = nullptr;

        Vertex(int l, int r, int isMin) : f(), l(l), r(r), isMin(isMin) {}

        void extend() {
            if (!leftChild && l != r) {
                int mid = (l+r)/2;
                leftChild = new Vertex(l, mid, isMin);
                rightChild = new Vertex(mid+1, r, isMin);
                leftChild->f = rightChild->f = f;
            }
        }

        void addLine(Function nw) {
            extend();
            int m = (l+r) / 2;
            bool lef = nw.isBetterThan(f, l, isMin);
            bool mid = nw.isBetterThan(f, m, isMin);
            if (mid) swap(f, nw);
            if (r == l) return;
            if (lef != mid) leftChild->addLine(nw);
            else rightChild->addLine(nw);
        }

        Function best(Function const &a, Function const &b, int x) {
            return (a.isBetterThan(b, x, isMin)) ? a : b;
        }

        Function get(int x) {
            extend();
            int m = (l + r) / 2;
            if (r == l) return f;
            if (x <= m) return best(f, leftChild->get(x), x);
            return best(f, rightChild->get(x), x);
        }
    };

    int n;
    Vertex *root;
    LiChaoTree(int n, bool isMin) : n(n) {
        root = new Vertex(0, n-1, isMin);
    }
    void addLine(Function nw) { root->addLine(nw); }
    Function get(int x) { return root->get(x); }
};
