vector<int> getSuffixArray(string s) {
	s += (char)30; // smaller than both '$' and ' '
	int n = s.size(), E = max(n, 256);
	vector<int> p(n), c(n);
	for (int i = 0; i < n; i++) p[i] = i, c[i] = s[i];

	for (int k = 0; k < n; k ? k *= 2 : k++) {
		vector<int> pn(p), cn(n), cnt(E);
		for (int i = 0; i < n; i++) pn[i] = (pn[i]-k+n)%n, cnt[c[i]]++;
		for (int i = 1; i < E; i++) cnt[i] += cnt[i-1];
		for (int i = n-1; i >= 0; i--) p[--cnt[c[pn[i]]]] = pn[i];
		for (int i = 1, r = 0; i < n; i++)
            cn[p[i]] = r += (
                c[p[i]] != c[p[i-1]]
                || c[(p[i]+k)%n] != c[(p[i-1]+k)%n]
            );
		c.swap(cn);
		if (c[p[n-1]] == n-1) break;
	}
    p.erase(p.begin());
    return p;
}

// O(|pat| * log(|text|))
// Every interval [l, r] is either completely contained in another, or doesn't intersect at all.
ii search(vector<int> &sa, string &text, string &pat) {
    auto cmp1 = [&](int idx, string const &pat) {
        return text.compare(idx, pat.size(), pat) < 0;
    };
    auto cmp2 = [&](int idx, string const &pat) {
        return text.compare(idx, pat.size(), pat) > 0;
    };
    int l = lower_bound(all(sa), pat, cmp1) - sa.begin();
    int r = lower_bound(rall(sa), pat, cmp2) - sa.rbegin();
    if (l == sa.size() || text.compare(sa[l], pat.size(), pat) != 0) return {-1, -1};
    return {l, sa.size()-1-r};
}

/*
Kasai lcp construction algorithm. lcp[i] is the longest common prefix of the suffix at sa[i] and its next suffix.
Claim: given the indices i and j, the lcp of the suffixes at positions i and j of sa is
lcp(i, j) = min(lcp[i], lcp[i+1], ..., lcp[j-1]) (no proof here)

Algorithm idea: let i and j be some suffix (other than the last) and its next suffix. It is clear that the lcp of the suffixes (i+1) and (j+1) is exactly lcp[rank[i]]-1 (given that lcp[rank[i]] > 0 and that the suffixes i+1 and j+1 exist).
But since the (i+1)th suffix comes before the (j+1)th suffix, and given the claim above, then, the lcp of all the suffixes between the position of the (i+1)th suffix and the (j+1)th suffix is at least lcp[rank[i]]-1, that is:
lcp[rank[i+1]] and lcp[rank[i+1]+1] and lcp[rank[i+1]+2] and ... and lcp[rank[j+1]-1] all are >= lcp[rank[i]]-1
In particular,
lcp[rank[i+1]] >= lcp[rank[i]]-1

The algorithm below iterates through the suffixes in regular string order and keeps a variable "k" that indicates what would be the lcp of the previous suffix in STRING ORDER minus 1.
Then, we bruteforce the answer for the ith suffix but starting the comparison from the kth character.

Edge cases:
 - rank[i] == n-1: its lcp shouldn't be calculated, and setting k to 0 is neutral (complexity is still O(n) even though k = 0 means bruteforcing).
 - lcp[rank[i]-1] == 0: even though the inequality still holds, we set k to 0 instead of lcp[rank[i]-1]-1 so our search space never becomes negative.
*/

vector<int> getLcp(string &s, vector<int> &sa) {
    int n = s.size();
    int k = 0;
    vector<int> lcp(n-1, 0), rank(n, 0);

    for (int i = 0; i < n; i++)
        rank[sa[i]] = i;

    for (int i = 0; i < n; i++, k?k--:0) {
        if (rank[i] == n-1) {
            k = 0;
            continue;
        }
        int j = sa[rank[i]+1];
        while (i+k<n && j+k<n && s[i+k] == s[j+k])
            k++;
        lcp[rank[i]] = k;
    }
    return lcp;
}

/*
Different substrings:
Let's count the total amount of substrings and then subtract the substrings we counted repeatedly. The total amount of substrings is:
n + (n-1) + (n-2) + ... + 1 (amount of prefixes starting at each letter)
= n*(n+1)/2

Now, let's go through each suffix in SA order and check how many prefixes of the current suffix were already counted before.
Suppose the current suffix differs from the previous suffix at the ith character of the string. This means that the first 'i' prefixes of the current suffix were already counted (because they match the previous suffix).
But what about the remaining prefixes that are not the first 'i' ones? Are they unique?
They are, because the prefix of length 'i+1' of the current suffix is strictly greater than any prefix of length 'i+1' counted before (this is because we are iterating in the suffix array order, and, therefore, in increasing lexicographical order). That argument applies to prefixes of length 'i+2', 'i+3', and so on. Therefore, the only repeated substrings of the current suffix are the first 'i' ones. With that we have that the answer is:
n*(n+1)/2 - (sum of all i for every suffix other than the first)

And how can we calculate 'i'? Well, since 'i' is the first index that mismatches, it means that the first 'i' characters matched. That number is, by definition, the lcp of the current suffix. Therefore, the answer is:
n*(n+1)/2 - accumulate(all(lcp), 0ll);
*/

int differentSubstringCount(vector<int> &lcp) {
    int n = lcp.size()+1;
    return n*(n+1)/2 - accumulate(all(lcp), 0ll);
}

/*
Longest Repeated Substring:
The suffix array indirectly stores data about substring similarity. That is because the most similar substrings
are in consecutive positions of the suffix array, and, because the lcp array is about consecutive suffixes, then it
says a lot about substring similarity in general.
Say the lcp at some position is 'i'. That means two suffixes share 'i' characters, i.e., that substring of length 'i' happens at least twice.
That is why the longest repeated substring is the one that is pointed by the maximum lcp.
*/

pair<int, int> longestRepeatedSubstring(vector<int> &sa, vector<int> &lcp) {
    int lrsIndexAtLcp = max_element(all(lcp)) - lcp.begin();
    // (lrsIndexAtLcp)th suffix in SA order has the maximum lcp value
    int len = lcp[lrsIndexAtLcp];
    int idx = sa[lrsIndexAtLcp];
    return {idx, len};
}

// Indices are from SA and not regular string indices. For regular string indices, input rank[i] and rank[j] instead.
int longestCommonPrefix(int i, int j, SparseTable &st) {
    if (i > j) swap(i, j);
    return st.query(i, j-1);
}
