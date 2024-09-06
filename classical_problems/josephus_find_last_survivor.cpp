#include <bits/stdc++.h>

using namespace std;
using pii = pair<int, int>;
using ll = long long;
using ull = unsigned long long;

int backtrack(int n, int k) {
  return n == 1 ? 0 : (backtrack(n-1, k) + k) % n;
}

int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);

  int t, k;
  cin >> t >> k;

  cout << backtrack(t, k) << endl;

}
