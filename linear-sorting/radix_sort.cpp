#include <bits/stdc++.h>

using namespace std;
using ll = long long;
using ull = unsigned long long;

// If numbers are too large, MAYBE increase base.
// Once I had to use 2^15 to get AC
// Some numbers are using ll. In practice, this is only needed for very large
// bases.

// int base = 32768;
const int base = 512; // IDK a good value

int get_digit(int a, int divisor) { return a / divisor % base; }

bool cmp(int a, int b, int divisor) {
  return get_digit(a, divisor) < get_digit(b, divisor);
}

void counting_sort(vector<int> &vec, vector<int> &output, int divisor) {
  int l = INT32_MAX, r = 0;

  vector<int> aux(vec.begin(), vec.end());
  for (auto &v : aux) {
    v = get_digit(v, divisor);
    l = min(l, v);
    r = max(r, v);
  }

  vector<int> f(r - l + 1);

  for (auto &v : aux)
    ++f[v - l];

  for (int i = 1; i < f.size(); ++i)
    f[i] = f[i - 1] + f[i];

  for (ll i = vec.size() - 1; i >= 0; --i) {
    int d = aux[i];
    output[f[d - l] - 1] = vec[i];
    f[d - l]--;
  }
}

void radix_sort(vector<int> &vec) {
  auto [il, ir] = minmax_element(vec.begin(), vec.end());
  int l = *il, r = *ir;

  int num_digits = 1;
  int tmp = r;
  while (tmp >= base) {
    num_digits++;
    tmp = tmp / base;
  }

  vector<int> a(vec.begin(), vec.end()), b(vec.begin(), vec.end());

  auto *output = &a;
  auto *aux = &b;

  int divisor = 1;
  for (int i = 0; i < num_digits; ++i) {
    swap(aux, output);
    counting_sort(*aux, *output, divisor);
    divisor *= base;
  }

  for (int i = 0; i < vec.size(); ++i)
    vec[i] = (*output)[i];
}
