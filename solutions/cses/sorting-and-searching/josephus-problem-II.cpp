#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

#define sz(x) (int(x.size()))
#define fast_io()                                                              \
  {                                                                            \
    ios::sync_with_stdio(0);                                                   \
    cin.tie(NULL);                                                             \
  }

typedef long long ll;
typedef unsigned long long ull;
typedef std::vector<int> vi;
typedef std::vector<ll> vll;
typedef std::vector<ull> vull;
typedef std::pair<int, int> pi;
typedef std::pair<ll, ll> pll;
typedef std::pair<ull, ull> pull;
typedef std::vector<pi> vii;
using namespace std;
using namespace __gnu_pbds;
/*
Consider a game where there are n children (numbered 1,2,\dots,n) in a circle.
During the game, repeatedly k children are skipped and one child is removed from
the circle. In which order will the children be removed?
*/
int main() {
  // fast_io();

  int n, k;
  cin >> n >> k;

  tree<int, null_type, less<int>, rb_tree_tag,
       tree_order_statistics_node_update>
      tr;
  for (int i = 0; i < n; ++i)
    tr.insert(i + 1);

  auto it = tr.find_by_order(k % tr.size());
  int i = k % tr.size();

  while (true) {
    cout << *it << ' ';
    it = tr.erase(it);
    if (tr.empty())
      break;

    int num = k % tr.size();
    if (i + num >= tr.size()) {
      it = tr.begin();
      num = num - (tr.size() - i);
    } else {
      num += i;
    }

    it = tr.find_by_order(num);
    i = (i + (k % tr.size())) % tr.size();
  }
}
