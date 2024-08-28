#include <bits/stdc++.h>

using namespace std;
using pii = pair<int, int>;
using ll = long long;
using ull = unsigned long long;

int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);

  int n, x;
  cin >> n >> x;

  vector<int> price(n), pages(n);
  for (auto &p : price)
    cin >> p;
  for (auto &p : pages)
    cin >> p;

  vector<int> dp(x + 1, 0);
  for (int i = 0; i < n; ++i)
    for (int j = x; j >= price[i]; --j)
      dp[j] = max(dp[j], dp[j - price[i]] + pages[i]);
  cout << dp[x] << '\n';
}
