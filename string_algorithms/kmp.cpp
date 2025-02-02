/*
Some useful KMP properties:
k = i + 1 - pi[i] gives you the size of the minimum string that, when concatenated several
times with itself, plus a prefix of itself, gives n. If you want to build the original string
only by concatenating another string several times (i.e., without a prefix of itself), then
k must divide n.
if (n % k == 0) ans = k;
else ans = n;

For pattern matching, build the string pat+"#"+text and then whenever pi[i] = pat.size(), 
it means that a pattern matched i - (pat.size()-1) positions before.
*/

vector<int> buildLps(string &s) {
    int n = s.length();
    vector<int> lps(n);
    for (int i = 1; i < n; i++) {
        int j = lps[i-1];
        while (j > 0 && s[i] != s[j])
            j = lps[j-1];
        if (s[i] == s[j])
            j++;
        lps[i] = j;
    }
    return lps;
}
