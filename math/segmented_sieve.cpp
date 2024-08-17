#include <bits/stdc++.h>
using namespace std;
// Same complexity as traditional sieve, but better constants because of cache.
// Also, memory complexity is O(sqrt(n) + S)
vector<int> segmented_sieve(int n) {

  int nsqrt = sqrt(n);

  // Block size
  const int S = max(nsqrt, (int)1e5);

  vector<int> primes, result;
  vector<char> is_prime(nsqrt + 2, true);

  for (int i = 2; i <= nsqrt; ++i) {
    if (is_prime[i]) {
      primes.push_back(i);
      for (int j = i * i; j <= nsqrt; j += i)
        is_prime[j] = false;
    }
  }

  vector<char> block(S);
  for (int k = 0; k * S <= n; ++k) {
    fill(block.begin(), block.end(), true);

    int start = k * S;
    for (auto &p : primes) {
      int start_idx = (start + p - 1) / p; // same as ceil(start/p)
      int j = max(start_idx, p) * p - start;
      for (; j < S; j += p)
        block[j] = false;
    }

    if (k == 0)
      block[0] = block[1] = false;
    for (int i = 0; i < S && start + i <= n; ++i)
      if (block[i])
        result.push_back(start + i);
  }
  return result;
}
