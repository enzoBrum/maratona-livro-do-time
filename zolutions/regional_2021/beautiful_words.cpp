/*
Exercise:
You are given a string 'a' and a set "strs" of strings. You wanna find the minimum "score" out
of all the cyclic shifts of 'a'.
The score of a shift is the maximum substring of it that appears as a substring of a word in
the set "strs".

Idea:
Binary search the smallest substring size such that every shift has a substring of that size
in the set of words. It is binary searchable because, if a word has a substring of size 'sz'
in the set, then, it also has words of size sz'<sz. Then, the answer is that smallest size.
To compute if a given size is valid, you can hash every substring of size 'sz' of the set,
and, for each position 'i' of 'a', check if the hash a(i, i+sz-1) appears on the set. To
handle the cyclic stuff, you can build the string a+a and check everything up to position
i E [0, n-1].
Since, implicitly, you are doing (1e5)^2 comparisons when adding a hash to a set, you have
to hash every substring twice and compare 2 values.
*/

// Bring Hash here
const int MOD1 = 1e9+7;
const int MOD2 = 1e9+9;

signed main() {
    cin.tie(0)->sync_with_stdio(0);

    int n, m;
    cin >> n >> m;
    string a; cin >> a; a += a;
    vector<string> strs(m);
    for (string &s : strs) cin >> s;

    Hash<MOD1> hasha1(a);
    Hash<MOD2> hasha2(a);

    vector<Hash<MOD1>> mod1hashes;
    vector<Hash<MOD2>> mod2hashes;
    mod1hashes.reserve(m);
    mod2hashes.reserve(m);
    for (int i = 0; i < m; i++) {
        mod1hashes.emplace_back(strs[i]);
        mod2hashes.emplace_back(strs[i]);
    }

    auto test = [&](int sz) {
        // constant of std::set was way too high
        vector<int> hashes1, hashes2;
        for (int si = 0; si < m; si++) {
            for (int i = 0; i+sz-1 < strs[si].size(); i++) {
                hashes1.push_back(mod1hashes[si](i, i+sz-1));
                hashes2.push_back(mod2hashes[si](i, i+sz-1));
            }
        }

        sort(all(hashes1));
        sort(all(hashes2));

        vector<int> onset(a.size(), 0);
        for (int i = 0; i+sz-1 < a.size(); i++) {
            int m1 = hasha1(i, i+sz-1);
            int c1 = lower_bound(all(hashes1), m1) - hashes1.begin();
            if (c1 < hashes1.size() && hashes1[c1] == m1) {
                int m2 = hasha2(i, i+sz-1);
                int c2 = lower_bound(all(hashes2), m2) - hashes2.begin();
                if (c2 < hashes2.size() && hashes2[c2] == m2)
                    onset[i] = 1;
            }
        }

        // the first window [0, s.size()-sz] belongs to the first shift
        // every successive window belongs to the next shift
        int sum = 0;
        const int WINDOW_SZ = n-sz+1;
        for (int i = 0; i < WINDOW_SZ; i++) sum += onset[i];
        if (!sum) return false;

        for (int i = WINDOW_SZ; i-WINDOW_SZ < n; i++) {
            sum += onset[i];
            sum -= onset[i-WINDOW_SZ];
            if (!sum) return false;
        }

        return true;
    };

    int low = 0;
    int high = n;

    while (low < high) {
        int mid = (low+high)/2+1;
        if (test(mid)) low = mid;
        else high = mid-1;
    }
    cout << low << '\n';

    return 0;
}