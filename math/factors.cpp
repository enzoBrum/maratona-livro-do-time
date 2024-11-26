vector<int> factors(int n) {
    vector<int> f;
    for (int i = 2; i*i <= n; i++) {
        while (n % i == 0) {
            f.push_back(i);
            n /= i;
        }
    }
    if (n) f.push_back(n);
    return f;
}
