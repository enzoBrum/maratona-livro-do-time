/*
*
* You are given an array containing n positive integers.
Your task is to divide the array into k subarrays so that the maximum sum in a
subarray is as small as possible.
*/

bool find_sub_arrays(vector<int> &vec, ll max_sum, int x) {
  ll curr = 0;
  int n = 1;

  int i = 0;
  while (i < vec.size()) {
    if (vec[i] > max_sum)
      return false;

    if (curr + vec[i] <= max_sum) {
      curr += vec[i];
    } else {
      n++;
      curr = vec[i];
    }
    ++i;
  }

  return n <= x;
}

int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  cout.tie(0);

  int n, x;
  cin >> n >> x;
  vector<int> vec(n);

  for (auto &v : vec)
    cin >> v;

  ll l = *max_element(vec.begin(), vec.end());
  ll r = accumulate(vec.begin(), vec.end(), 0ll);

  ll ans = 0;
  while (l < r + 1) {
    ll mid = (l + r) / 2;

    bool possible = find_sub_arrays(vec, mid, x);

    if (possible)
      ans = mid;
    // cout << "l=" << l << ",r=" << r << ",mid=" << mid << ",possible=" <<
    // possible << '\n';
    if (possible)
      r = mid - 1;
    else
      l = mid + 1;
  }

  cout << ans << '\n';
}
