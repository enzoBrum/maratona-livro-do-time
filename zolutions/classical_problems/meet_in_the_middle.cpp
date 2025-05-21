/*
 *
 * Find all combinations C(m, n) where the sum of all elements equals t.
 * For a sufficiently small m, we can use Meet in the Middle.
 *
 * Usually, if you wanted to do this naively, you'd have to test each subset of
 * size N of the given vector, which would give you a complexity of O(2^m)
 *
 * However, if we divide the vector in two, find all subsets of size <= N and
 * them merge both results, we can achieve a complexity of O(2^(m/2))
 */

#include <bits/stdc++.h>

#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()

using namespace std;
using pii = pair<int, int>;
using ll = long long;
using ull = unsigned long long;

int dp[2][31][10'000 * 30 + 2];

void gen_subsets(vector<int> &labels, int begin, int end, int n, int idx) {
  int limit = 1 << (end - begin);

  for (int mask = 0; mask < limit; ++mask) {
    int cur_sum = 0;
    int cnt = 0;
    for (int j = 0; j < end - begin; ++j)
      if ((mask & (1 << j)))
        cur_sum += labels[begin + j], cnt++;

    if (cnt <= n) {
      dp[idx][cnt][cur_sum]++;
    }
  }
}

int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);

  int game;
  cin >> game;

  for (int g = 1; g <= game; ++g) {
    int m;
    cin >> m;

    vector<int> labels(m);
    for (auto &l : labels)
      cin >> l;

    int n, t;
    cin >> n >> t;

    if (!n && !t) {
      cout << "Game " << g << " -- 1 : 0\n";
      continue;
    } else if (!n) {
      cout << "Game " << g << " -- 0 : 1\n";
      continue;
    }

    int sum = accumulate(all(labels), 0);
    memset(dp, 0, sizeof(dp));

    int mid = labels.size() / 2;
    gen_subsets(labels, 0, mid, n, 0);
    gen_subsets(labels, mid, m, n, 1);

    unsigned int win = 0;
    for (int i = 0; i <= n; ++i)
      for (int j = 0; j <= t; ++j)
        if (dp[0][i][j] && dp[1][n - i][t - j])
          win += (dp[0][i][j] * dp[1][n - i][t - j]);

    double all_games = 1;
    for (int i = 1; i <= n; ++i)
      all_games = all_games * (m - n + i) / i;
    unsigned int all_g = (unsigned int)(all_games + 0.01);

    cout << "Game " << g << " -- " << win << " : " << all_g - win << '\n';
  }
}
