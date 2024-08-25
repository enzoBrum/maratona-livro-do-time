#include <algorithm>
#include <bits/stdc++.h>
#include <chrono>
#include <random>
#include <set>
#include <vector>
#define sz(x) (int(x.size()))
#define fast_io()                                                              \
  {                                                                            \
    ios::sync_with_stdio(0);                                                   \
    cin.tie(NULL);                                                             \
  }

typedef long long ll;
typedef uint64_t ull;
typedef std::vector<int> vi;
typedef std::vector<ll> vll;
typedef std::vector<ull> vull;
typedef std::pair<int, int> pi;
typedef std::pair<ll, ll> pll;
typedef std::pair<ull, ull> pull;
typedef std::vector<pi> vii;
using namespace std;

/*
 *In a movie festival n movies will be shown. You know the starting and ending
 *time of each movie. What is the maximum number of movies you can watch
 *entirely?
 */

int main() {
  fast_io();

  int n;
  cin >> n;

  vii vec;
  while (n--) {
    int b, e;
    cin >> b >> e;

    vec.emplace_back(b, e);
  }

  sort(vec.begin(), vec.end(),
       [](const pi &a, const pi &b) { return a.second < b.second; });

  int curr_ed = 0;
  int ans = 0;

  for (auto &p : vec) {
    if (curr_ed <= p.first) {
      ans++;
      curr_ed = p.second;
    }
  }
  cout << ans << '\n';
}
