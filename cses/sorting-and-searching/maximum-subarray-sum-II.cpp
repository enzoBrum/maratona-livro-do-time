/*
 *
 *Given an array of n integers, your task is to find the maximum sum of values
 *in a contiguous subarray with length between a and b.
 */
int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  cout.tie(0);

  ll n, a, b;
  cin >> n >> a >> b;

  vector<ll> prefix(n);
  cin >> prefix[0];

  for (ll i = 1; i < n; ++i) {
    ll tmp;
    cin >> tmp;
    prefix[i] = prefix[i - 1] + tmp;
  }

  multiset<ll> window;
  unordered_map<ll, queue<ll>> pref_to_idx;

  ll ans = INT64_MIN;
  ll last_insert = 0;
  for (ll i = a - 1; i < n; ++i) {
    if (i - last_insert > b) {
      window.erase(window.lower_bound(prefix[last_insert++]));
    }

    if (!window.size() || (i < b && *window.begin() == prefix[0]))
      ans = max(ans, prefix[i]);
    else {
      ans = max(ans, prefix[i] - *window.begin());
    }

    if (a == 1)
      ans = max(ans, (i == 0 ? prefix[0] : prefix[i] - prefix[i - 1]));

    window.insert(prefix[i - a + 1]);
  }

  cout << ans << '\n';
}
