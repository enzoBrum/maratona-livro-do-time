// build:
// Vertex* st = new Vertex(0, desiredLastIndex);
struct Vertex {
    int l, r;
    int lazy = 0;
    int mini = 0;
    Vertex *leftChild = nullptr;
    Vertex *rightChild = nullptr;
    const static int NEUTRAL = INF;
 
    Vertex(int l, int r) : l(l), r(r) {}
 
    void destroyTree() {
        if (leftChild) {
            leftChild->destroyTree();
            rightChild->destroyTree();
        }
        delete this;
    }
 
    void merge() {
        if (leftChild)
            mini = min(leftChild->mini, rightChild->mini);
    }
 
    int merge(int a, int b) {
        return min(a, b);
    }
 
    void extend() {
        if (!leftChild && l != r) {
            int mid = (l+r)/2;
            leftChild = new Vertex(l, mid);
            rightChild = new Vertex(mid+1, r);
        }
    }
 
    void push() {
        if (lazy) {
            mini += lazy;
            if (l != r) { // not leaf
                leftChild->lazy += lazy;
                rightChild->lazy += lazy;
            }
            lazy = 0;
        }
    }
 
    int queryMin(int ql, int qr) {
        extend();
        push();
        if (l > qr || r < ql)
            return NEUTRAL;
        if (l >= ql && r <= qr)
            return mini;
        return merge(leftChild->queryMin(ql, qr), rightChild->queryMin(ql, qr));
    }
 
    void incrementRange(int ql, int qr, int delta) {
        extend();
        push();
        if (l > qr || r < ql) return;
        if (l >= ql && r <= qr) {
            lazy += delta;
            push();
            return;
        }
        leftChild->incrementRange(ql, qr, delta);
        rightChild->incrementRange(ql, qr, delta);
        merge();
    }
};