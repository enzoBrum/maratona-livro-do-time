/*
Mo's Algorithm: used in some very particular range queries that do NOT
have updates.

Idea: sort the queries and try to use the result of some for the next.
It is done like this: you will separate the queries by their left indices
on buckets of size sqrt(vector_length) and then sort them in decreasing right.

Then, when you process them:
    for a given bucket, initialize l = 0 and r as the right of the first query.
    Then, for the next queries on this bucket, r will only go left and l
    will jump between any position of the sqrt bucket.
*/

struct Query {
    int l, r, id;
};

int BUCKET_SIZE = ceil(sqrt(n));
vector<vector<Query>> buckets(BUCKET_SIZE);

for (int i = 0; i < m; i++) {
    int l, r;
    cin >> l >> r;
    l--; r--;
    buckets[l/BUCKET_SIZE].push_back({l, r, i});
}

auto cmpReverseRight = [&](Query &q1, Query &q2) {
    return q1.r > q2.r;
};

vector<int> counter(100000+5, 0);
int l = 0;
int r = -1;
int ans = 0;
vector<int> answers(m);

for (vector<Query> &v : buckets) {
    // set r to first query (increase range)
    if (!v.size()) continue;
    sort(all(v), cmpReverseRight);
    while (r < v[0].r) { // this one grabs v[0].r (inclusive)
        r++;
        if (counter[a[r]] == a[r]) ans--;
        counter[a[r]]++;
        if (counter[a[r]] == a[r]) ans++;
    }

    for (Query &q : v) {
        // move l right, if needed (decrease range)
        while (l < q.l) {
            if (counter[a[l]] == a[l]) ans--;
            counter[a[l]]--;
            if (counter[a[l]] == a[l]) ans++;
            l++;
        }

        // move l left, if needed (increase range)
        while (l > q.l) {
            l--;
            if (counter[a[l]] == a[l]) ans--;
            counter[a[l]]++;
            if (counter[a[l]] == a[l]) ans++;
        }

        // move r left, if needed (decrease range)
        while (r > q.r) {
            if (counter[a[r]] == a[r]) ans--;
            counter[a[r]]--;
            if (counter[a[r]] == a[r]) ans++;
            r--;
        }

        answers[q.id] = ans;
    }
}
