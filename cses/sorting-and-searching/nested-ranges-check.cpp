#define sz(x) (int(x.size()))
#define fast_io()                                                              \
  {                                                                            \
    ios::sync_with_stdio(0);                                                   \
    cin.tie(NULL);                                                             \
  }
#define mult_vec(name, T, n, m)                                                \
  std::vector<std::vector<T>> name(n, std::vector<T>(m))

using namespace std;

/*
 *
 *Given n ranges, your task is to determine for each range if it contains
 *some other range and if some other range contains it. Range [a,b] contains
 *range [c,d] if a \le c and d \le b.

The first input line has an integer n: the number of ranges.
After this, there are n lines that describe the ranges. Each line has two
integers x and y: the range is [x,y]. You may assume that no range appears more
than once in the input.

 */

struct Fenwick {
  Fenwick(int n) : vec{vi(n + 1, 0)} {}

  void add(int idx, int val) {
    while (idx < vec.size()) {
      vec[idx] += val;
      idx += idx & -idx;
    }
  }

  int query(int l, int r) { return query(r) - query(l - 1); }

  int query(int r) {
    int sum = 0;
    int i = r;
    while (i > 0) {
      sum += vec[i];
      i -= i & -i;
    }

    return sum;
  }

  vi vec;
};

struct Range {
  Range(int l, int r, int i) : left{l}, right{r}, i{i} {}

  int left;
  int right;
  int i;
};

unordered_map<int, int> normalize(vector<Range> vec) {
  unordered_map<int, int> mp;
  int count = 1;

  sort(vec.begin(), vec.end(), [](Range &a, Range &b) {
    return (a.right == b.right) ? a.left < b.left : a.right < b.right;
  });

  for (auto &n : vec) {
    if (!mp.contains(n.right)) {
      mp[n.right] = count;
      count++;
    }
  }

  return mp;
}

vpi count_inv(vector<Range> &vec) {
  Fenwick fwl(vec.size()), fwr(vec.size());
  vpi ret(vec.size());

  auto norm = normalize(vec);

  for (auto &n : vec) {
    ret[n.i].second += fwl.query(norm[n.right], vec.size());
    fwl.add(norm[n.right], 1);
  }
  for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
    auto &n = *it;
    ret[n.i].first += fwr.query(1, norm[n.right]);
    fwr.add(norm[n.right], 1);
  }

  return ret;
}

int main() {
  fast_io();

  int n;
  cin >> n;

  vector<Range> vec;

  for (int i = 0; i < n; ++i) {
    int l, r;
    cin >> l >> r;
    vec.emplace_back(l, r, i);
  }

  sort(vec.begin(), vec.end(), [](Range &a, Range &b) {
    return (a.left == b.left) ? a.right > b.right : a.left < b.left;
  });

  auto v = count_inv(vec);

  for (auto &p : v)
    cout << (p.first > 0) << ' ';
  cout << '\n';

  for (auto &p : v)
    cout << (p.second > 0) << ' ';
}
