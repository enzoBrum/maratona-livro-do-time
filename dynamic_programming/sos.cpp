/*
O(n * 2^n) - k is the amount of bits of the masks
s[x][i] = s[x][i-1] + s[x^(1<<i)][i-1]     if (x & (1<<i))
        = s[x][i-1]                        otherwise
'a' must be s[x][0], that is, the value you want to compute when the mask matches EXACTLY x.
*/
vector<int> sos(vector<int> a, int k, bool inv) {
    for (int i = 0; i < k; i++)
        for (int x = 0; x < a.size(); x++)
            if (x & (1<<i)) a[x] += (inv ? -1 : 1) * a[x^(1<<i)];
    return a;
}
