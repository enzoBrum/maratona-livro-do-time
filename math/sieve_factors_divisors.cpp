vector<bool> sieve(int n) {
    vector<bool> isPrime(n+1, true);
    isPrime[0] = isPrime[1] = false;
    for (int i = 2; i * i <= n; i++)
        if (isPrime[i])
            for (int j = i * i; j <= n; j += i)
                isPrime[j] = false;
    return isPrime;
}

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

vector<int> divisors(int n) {
    vector<int> d;
    for (int i = 1; i*i <= n; i++) {
        if (n % i == 0) {
            d.push_back(i);
            if (i*i != n)
                d.push_back(n/i);
        }
    }
    return d;
}

vector<vector<int>> divsSieve(int n) {
    vector<vector<int>> divisors(n+1);
    for (int i = 1; i <= n; i++)
        for (int j = i; j <= n; j += i)
            divisors[j].push_back(i);
    return divisors;
}
