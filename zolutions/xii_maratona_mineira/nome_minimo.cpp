/*
Exercise:
Given a string 's' (|s| <= 1e6), print the smallest new string that can be made by
reversing at most one substring of 's'.

Idea:
You know what is the first position that will belong to the substring, because it is
the first index of s such that s[i] is not the minimum of the characters after it.
Then, its a matter of finding what is the ending position of the substring.
Out of all the positions j > i, we gotta find which string is minimal when going from
s[j] backwards and then appending the non-reversed suffix of 's'. If some strings tie,
we gotta break the tie by checking that non-reversed part with the remaining part of
the longer candidate that tied.
That can be done by building a suffix array of s + reverse(s.substr(start)) and doing
the comparisons with the lcp table.
*/

signed main() {
    cin.tie(0)->sync_with_stdio(0);

    string s; cin >> s;

    int start = -1; int mini = INF;
    for (int i = s.size()-1; i >= 0; i--) {
        if (s[i]-'a' > mini) start = i;
        else mini = min(mini, s[i]-'a');
    }
    // s strictly increasing
    if (start == -1) { cout << s << '\n'; return 0; }

    string r = s.substr(start);
    reverse(all(r));
    string forSa = s+r;
    vector<int> sa = getSuffixArray(forSa);
    SparseTable st(getLcp(forSa, sa));

    vector<int> rank(sa.size());
    for (int i = 0; i < sa.size(); i++)
        rank[sa[i]] = i;

    // complement[i]: sa index of what comes after we reverse the substring forSa[i] represents
    vector<int> complement(sa.size());
    for (int i = 0; i < sa.size(); i++) {
        if (sa[i] < s.size()) continue;
        int compSuffix = 2*s.size() - sa[i]; // works for a tricky reason
        complement[i] = rank[compSuffix];
    }

    auto compareSuffixes = [&](int i, int j, int len) {
        int l = longestCommonPrefix(i, j, st);
        if (l >= len) return 0; // equal
        if (forSa[sa[i]+l] < forSa[sa[j]+l]) return -1;
        return 1;
    };

    int bestSuffix = -1;
    while (sa[++bestSuffix] < s.size());

    for (int i = bestSuffix+1; i < sa.size(); i++) {
        if (sa[i] < s.size()) continue;

        int suffixILen = sa.size()-sa[i];
        int suffixBestSufLen = sa.size()-sa[bestSuffix];
        int cmpSize = min(suffixILen, suffixBestSufLen);
        int overlapCmp = compareSuffixes(bestSuffix, i, cmpSize);

        if (overlapCmp == -1) break;
        if (overlapCmp == 1) { bestSuffix = i; continue; }
        if (suffixILen <= suffixBestSufLen) { // i is the small
            if (compareSuffixes(rank[sa[bestSuffix] + cmpSize], complement[i], suffixBestSufLen-suffixILen) == 1) 
                bestSuffix = i;
        }
        else {
            if (compareSuffixes(complement[bestSuffix], rank[sa[i] + cmpSize], suffixILen-suffixBestSufLen) == 1)
                bestSuffix = i;
        }
    }

    reverse(s.begin()+start, s.begin() + start + r.size()-(sa[bestSuffix] - s.size()));
    cout << s << '\n';
    
    return 0;
}
