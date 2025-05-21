/**

Given an HQ, some hotels H and an attraction A, find shortest path that follows this:

HQ -> First len(H)/2 hotels -> remaining len(H) - len(H)/2 hotels -> A -> First len(H)/2 hotels -> remaining len(H) - len(H)/2 hotels -> HQ

input: N -> len(H) + 2 (includes HQ and A)
       M -> number of edges.

You can pass in front of a hotel without visiting it, so do a floyd-warshall to get all shortest paths from U to V.

Use TSP dp solution.
 */


#include <algorithm>
#include <bits/stdc++.h>

#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()

using namespace std;
using pii = pair<int, int>;
using ll = long long;
using ull = unsigned long long;



int N, M;
int d[21][21];

int dp_first_half[1 << 18][18], dp_second_half[1 << 18][18];

void tsp() {
  int limit = 1 << (N-2);
  for (int mask = 0; mask < limit; ++mask) {
    for (int i = 0; i < N - 2; ++i) {
      if (!(mask & (1 << i)))
        continue;
      for (int j = 0; j < N - 2; ++j) {
        if (!(mask & (1 << j)) || i == j)
          continue;

        dp_first_half[mask][j] = min(
          dp_first_half[mask][j],
          dp_first_half[mask ^ (1 << j)][i] + d[i+1][j+1]
        );
        
        dp_second_half[mask][j] = min(
          dp_second_half[mask][j],
          dp_second_half[mask ^ (1 << j)][i] + d[i+1][j+1]
        );
      }
    }
  }
}

int solve() {
  if (N == 3) {
    return d[0][1] + d[1][2] + d[2][1] + d[1][0];
  }
  tsp();

  int n = N - 2;
  int k = n / 2;

  int mask = (1 << k) - 1, r, c;
  int ans = 1e9;
  while (mask <= (1 << n) - (1 << (n - k))) {
    int first_half = 1e9;
    int not_mask = (~mask) & ((1 << (N - 2)) - 1);
    for (int i = 0; i < N - 2; ++i) {
      if (!(mask & (1 << i)))
        continue;

      for (int j = 0; j < N - 2; ++j) {
        if (!(mask & (1 << j)))
          continue;

        ans = min(
          ans,
          dp_first_half[mask][i] + dp_second_half[not_mask | (1 << i)][i] + dp_second_half[mask][j] + dp_first_half[not_mask | (1 << j)][j]
        );
      }
    }

    c = mask & -mask, r = mask + c, mask = r | (((r ^ mask) >> 2) / c);
  }
  return ans;
}

int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);

  int t = 1;
  while (cin >> N >> M) {
    memset(d, 63, sizeof(d));
    memset(dp_first_half, 63, sizeof(dp_first_half));
    memset(dp_second_half, 63, sizeof(dp_second_half));
    

    for (int i = 0; i < N; ++i)
      d[i][i] = 0;
    while (M--) {
      int u, v, w;
      cin >> u >> v >> w;

      d[u][v] = d[v][u] = w;
    }


    for (int k = 0; k < N; ++k)
      for (int u = 0; u < N; ++u)
        for (int v = 0; v < N; ++v)
          d[u][v] = min(d[u][v], d[u][k] + d[k][v]);
    for (int i = 0; i < N-2; ++i)
      dp_first_half[0][i] = dp_first_half[1 << i][i] = d[0][i+1], dp_second_half[0][i] = dp_second_half[1 << i][i] = d[i+1][N-1];
    
    cout << "Case " << t++ << ": " << solve() << '\n';
  }
}
