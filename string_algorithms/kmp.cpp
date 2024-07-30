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

int main() {
    // every time a pattern matches, the lcs value of that position will be 'n' (the pat size)
    
    string text, pat;
    string patThenText = pat + '#' + text; // '#' should not appear on any of the strings
    vector<int> lps = buildLps(patThenText);
    int n = pat.length();
    int m = text.length();
    vector<int> indices;
    for (int i = n+1; i < n+m+1; i++)
        if (lps[i] == n)
            indices.push_back(i - 2*n);
}
