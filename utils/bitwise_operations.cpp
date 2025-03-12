// Usefull bitwise functions
//

// __builtin_popcount(x): Counts the number of one's(set bits) in an
// integer(long/long long).
// __builtin_clz(x): Counts the leading number of zeros of the integer(long/long
// long)
// __builtin_ctz(x): Counts the trailing number of zeros of the
// integer(long/long long)
// __builtin_ffs(x): Index of the least significant bit + 1
// __lg(x): Index of most significant bit.
// Generate all subsets of size N
for (int mask = 0; mask < (1 << n); ++mask)
  for (int i = 0; i < n; ++i)
    if ((mask & (1 << i)))
      // inside subset.
      else
          // outside subset

          // Generate all combinations C(n, k). IDK how it works, but it does.
          int n,
          k;
int mask = (1 << k) - 1, r, c;
while (mask <= (1 << n) - (1 << (n - k))) {
  // code here
  c = mask & -mask, r = mask + c, mask = r | (((r ^ mask) >> 2) / c);
}
