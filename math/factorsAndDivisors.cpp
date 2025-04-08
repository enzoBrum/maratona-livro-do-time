vector<int> factors(int n) {
    vector<int> f;
    for (int i = 2; i*i <= n; i++) {
        while (n % i == 0) {
            f.push_back(i);
            n /= i;
        }
    }
    if (n > 1) f.push_back(n);
    return f;
}

vector<vector<int>> divsSieve(int n) {
    vector<vector<int>> divisors(n+1);
    for (int i = 1; i <= n; i++)
        for (int j = i; j <= n; j += i)
            divisors[j].push_back(i);
    return divisors;
}
