/*
Time complexity: O(N * MAXK * MAXK), where MAXK is the number of bits
(each of the K buckets relax up to N numbers, with each XOR costing O(K) operations).
Recovering the answer is O(N * N * MAXK), because there is a N*MAXK matrix with masks of size N.

XOR Gauss
We want something relative to the XOR of some subset of a given set.
Say we want to maximize the XOR.
If we process the numbers in decreasing order of MSB, then, we can easily solve the case where
there is only 1 number with the current MSB: if the cumulative XOR has the bit unset, we use
the dude, else, we don't. The problems arise when we are working with "buckets" of numbers 
that share the MSB.
Ideally, we would like a set where there is only 1 number with each MSB of the original set, but
also that every possible XOR of the original set is still achievable. That set is called the "basis"
of a "span", and that set is what XOR Gauss computes. This set is a linearly independent set that is
able to generate the full span through linear combinations of its elements.

Algorithm:
Put the numbers in buckets of MSB.
Iterate through them in decreasing order {
    grab any number of the current bucket and erase it
    XOR every number of the current bucket and put them in the corresponding buckets
    put the current number in the answer set
    (xoring the current numbers is valid because, if they end up entering in the basis, I can
    recover them by doing xor with the current erased number. If they don't, its because they
    reached 0 some moment later in time, and 0 always "belongs" to the basis (by simply not
    xoring anything), and those numbers are therefore recoverable).
}

"Proof" of correctness:
You can see the algorithm as saying "I need a few guys to be combinable to end up as everyone on this
bucket 'i' and below". When we grab someone on the current bucket and xor everyone else, we are already
solving the 'i'th bucket and recursively asking to solve for the 'i-1'th bucket. The base case would be some
bucket with i < 0, that is, some bit-less numbers, which are already doable by simply not xor-ing anything.

I say we are solving the 'i'th bucket because every number that did not enter the set is buildable by
xoring whoever I sent to a forward bucket with the dude I added on the set from the 'i'th bucket.
*/

const int MAXK = 1501;
const int MAXN = 1500;
const int ZERO_BUCKET = MAXK;
using bs = bitset<MAXK>;
using codebs = bitset<MAXN>; // encoding is up to n

inline int msb(bs &b) {
    for (int i = b.size()-1; i >= 0; i--)
        if (b.test(i))
            return i;
    return ZERO_BUCKET;
}

// encodings[k][i]: indices that xor compose nums[i] when it was on the kth bucket
// i.e., nums[i] went to buckets[k] after some xor operations. encodings[k][i] stores that.
vector<vector<codebs>> encodings;

vector<bs> xorGaussBasis(vector<bs> &nums) {
    int n = nums.size();
    int k = nums[0].size();
    encodings.assign(k+1, vector<codebs>(n, 0));
    // {number, index on nums}
    vector<vector<pair<bs, int>>> buckets(k+1); // k+1 because of ZERO_BUCKET

    for (int i = 0; i < n; i++) {
        int bucket = msb(nums[i]);
        buckets[bucket].push_back({nums[i], i});
        encodings[bucket][i].set(i);
    }

    vector<bs> basis;
    for (int i = k-1; i >= 0; i--) {
        if (buckets[i].empty()) continue;
        auto[cur, curIdx] = buckets[i].back(); buckets[i].pop_back();
        for (auto&[b, idxB] : buckets[i]) {
            bs newbs = cur ^ b;
            int bucket = msb(newbs);
            buckets[bucket].push_back({newbs, idxB});
            // encoding it had before XOR encoding of bucket captain
            encodings[bucket][idxB] = encodings[i][idxB] ^ encodings[i][curIdx];
        }
        basis.push_back(cur);
    }

    return basis;
}

bitset<MAXK> maxXor(vector<bitset<MAXK>> &basis) {
    bitset<MAXK> ret = basis[0];
    int curBit = basis[0].size()-1;
    for (int i = 1; i < basis.size(); i++) {
        while (curBit >= 0 && !basis[i].test(curBit))
            curBit--;
        if (curBit < 0) break;
        if (!ret.test(curBit))
            ret ^= basis[i];
    }
    return ret;
}

signed main() {
    vector<bitset<MAXK>> nums; // init numbers here
    vector<bitset<MAXK>> basis = xorGaussBasis(nums);
    bitset<MAXK> maxi = maxXor(basis);
    unsigned long long ans = maxi.to_ullong();
}
