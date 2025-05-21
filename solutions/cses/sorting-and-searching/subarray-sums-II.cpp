/*
 *Given an array of n integers, your task is to count the number of subarrays
 *having sum x.
 */
int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  cout.tie(0);

  ll n, x;
  cin >> n >> x;
  vector<ll> vec(n);
  for (auto &val : vec)
    cin >> val;

  map<ll, ll> mp_sum;
  ll curr = 0;

  ull ans = 0;
  int i = 0;
  for (auto &v : vec) {
    if (v + curr == x)
      ans++;
    if (mp_sum.count(v + curr - x))
      ans += mp_sum[v + curr - x];
    mp_sum[v + curr]++;
    curr += v;
  }

  cout << ans << '\n';
}
