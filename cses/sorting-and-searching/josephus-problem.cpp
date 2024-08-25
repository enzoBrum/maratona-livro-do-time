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

constexpr const int sz = 2e5 + 5;

/*
*
Consider a game where there are n children (numbered 1,2,\dots,n) in a circle.
During the game, every other child is removed from the circle until there are no
children left. In which order will the children be removed?
*/

int main() {
  fast_io();

  int n;
  int arr[2][sz];

  cin >> n;

  for (int i = 0; i < n; ++i)
    arr[0][i] = i + 1;

  arr[0][n] = 0;

  int count = 0;
  int curr = 0, other = 1;
  bool even = n % 2 == 0;

  while (count != n) {
    int i = even ? 0 : 1;
    int round = even ? 0 : 1;
    int i2 = 0;
    while (arr[curr][i] != 0) {
      if (round) {
        round = 0;
        count++;
        cout << arr[curr][i] << ' ';
      }

      else {
        round = 1;
        arr[other][i2] = arr[curr][i];
        i2++;
      }

      i++;
    }

    if (!even) {
      cout << arr[curr][0] << ' ';
      count++;
    }

    arr[other][i2] = 0;
    // cout << "\nFOOO\n";
    // for ( int i3 = 0; i3 < i2; ++i3 )
    //     cout << arr[other][i3] << ' ';
    // cout << "\nFOOO\n";

    curr = (curr == 1) ? 0 : 1;
    other = (other == 1) ? 0 : 1;

    even = i2 % 2 == 0;
  }
}
