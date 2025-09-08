/*
p is the priority
The implicit key is the amount of elements smaller than it (also known
as the vertex position on the inorder traversal).
inorder(T) {
    inorder(T->L)
    print(t)
    inorder(T->R)
}
lazy is regular lazy sum, rev is lazy reversal propagation.
*/

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

#warning gotta make sure you call prop() if you invent a new function
template<typename T> struct Treap {
	struct Node {
		Node *l = nullptr, *r = nullptr, *parent = nullptr;
		int p, sz = 1;
		T val, treeSum, lazy = 0;
		bool rev = 0;
		Node(T v) : p(rng()), val(v), treeSum(v) {}
		void prop() {
			if (lazy) {
				val += lazy, treeSum += lazy*sz;
				if (l) l->lazy += lazy;
				if (r) r->lazy += lazy;
			}
			if (rev) {
				swap(l, r);
				if (l) l->rev ^= 1;
				if (r) r->rev ^= 1;
			}
			lazy = 0, rev = 0;
		}
		void update() {
			sz = 1, treeSum = val;
			if (l) l->prop(), sz += l->sz, treeSum += l->treeSum, l->parent = this;
			if (r) r->prop(), sz += r->sz, treeSum += r->treeSum, r->parent = this;
		}
	};

	Node* root;

	Treap() : root(nullptr) {}
	~Treap() {
		vector<Node*> q = {root};
		while (q.size()) {
			Node* x = q.back(); q.pop_back();
			if (!x) continue;
			q.push_back(x->l), q.push_back(x->r);
			delete x;
		}
	}

	int size(Node* x) { return x ? x->sz : 0; }
	int size() { return size(root); }
    // merges treaps l and r and puts the result on i
    // every value on l must be <= every value on r
	void merge(Node* l, Node* r, Node*& i) {
		if (!l || !r) return void(i = l ? l : r);
		l->prop(), r->prop();
		if (l->p > r->p) merge(l->r, r, l->r), i = l;
		else merge(l, r->l, r->l), i = r;
		i->update();
	}

    // splits treap i into treaps l and r such that l has the first v guys.
    // "add" is the sum of 1+size(P->l) for every ancestor P such that 
    // the path to this vertex 'i' went to the right subtree of P.
    // Therefore, the key of 'i' is add+size(i->l) 
	void split(Node* i, Node*& l, Node*& r, int v, int add=0) {
		if (!i) return void(r = l = nullptr);
		i->prop();
		if (add + size(i->l) < v) split(i->r, i->r, r, v, add+size(i->l)+1), l = i;
		else split(i->l, l, i->l, v, add), r = i;
		i->update();
	}
	void push_back(T v) {
		Node* i = new Node(v);
		merge(root, i, root);
	}
	T query(int l, int r) {
		Node *L, *M, *R;
		split(root, M, R, r+1), split(M, L, M, l);
		T ans = M->treeSum;
		merge(L, M, M), merge(M, R, root);
		return ans;
	}
	void update(int l, int r, T s) {
		Node *L, *M, *R;
		split(root, M, R, r+1), split(M, L, M, l);
		M->lazy += s;
		merge(L, M, M), merge(M, R, root);
	}
	void reverse(int l, int r) {
		Node *L, *M, *R;
		split(root, M, R, r+1), split(M, L, M, l);
		M->rev ^= 1;
		merge(L, M, M), merge(M, R, root);
	}
	// done this way to ensure propagation happens accordingly, BUT NEVER TESTED
	int getKey(Node *t) {
        vector<Node*> path;
        root->prop();
        while (t != root) path.push_back(t), t = t->parent;
        int k = 0;
        while (path.size()) {
            t = path.back(); path.pop_back();
            t->prop();
            if (t == t->parent->r) k += 1 + size(t->parent->l);
        }
        k += size(t->l);
		return k;
	}
	// inserts node 'i' at position "key" of the treap
    void insert(Node *i, int key) {
		Node *L, *R;
		split(root, L, R, key);
		merge(L, i, L);
		merge(L, R, root);
    }
	void erase(int key) {
		Node *L, *R, *tmp;
		split(root, L, R, key+1);
		if (L) split(L, L, tmp, key);
		merge(L, R, root);
	}
	void output(Node *t) {
		if (!t) return;
		output(t->l);
		cout << t->val << " ";
		output(t->r);
	}
};
