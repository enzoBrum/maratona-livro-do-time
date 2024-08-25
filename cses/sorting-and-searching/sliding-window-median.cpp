/**
  *
  *
  *You are given an array of n integers. Your task is to calculate the median of
each window of k elements, from left to right. 
The median is the middle element
when the elements are sorted. If the number of elements is even, there are two
possible medians and we assume that the median is the smaller of them.
  */

using namespace std;
using namespace __gnu_pbds;

typedef tree<int, null_type, less_equal<int>, rb_tree_tag,
             tree_order_statistics_node_update>
    ordered_tree;

int get_median(ordered_tree &t, int k) {
  return t.size() % 2 == 0 ? *t.find_by_order(k / 2 - 1)
                           : *t.find_by_order(k / 2);
}

int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  cout.tie(0);

  int n, k;
  cin >> n >> k;

  vector<int> vec(n);
  for (auto &v : vec)
    cin >> v;

  ordered_tree window;
  queue<int> to_remove;

  for (int i = 0; i < k; ++i) {
    window.insert(vec[i]);
    to_remove.push(vec[i]);
  }

  cout << get_median(window, k) << ' ';
  window.erase(window.upper_bound(to_remove.front()));
  to_remove.pop();

  for (int i = k; i < n; ++i) {
    window.insert(vec[i]);
    to_remove.push(vec[i]);
    cout << get_median(window, k) << ' ';
    window.erase(window.upper_bound(to_remove.front()));
    to_remove.pop();
  }

  cout << '\n';
}
