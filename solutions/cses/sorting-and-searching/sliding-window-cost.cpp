/*
  *You are given an array of n integers. Your task is to calculate for each
window of k elements, from left to right, the minimum total cost of making all
elements equal. You can increase or decrease each element with cost x where x is
the difference between the new and the original value. The total cost is the sum
of such costs.
  */

using ll = long long;

void debug(ll i, multiset<ll> &lo, multiset<ll> &hi) {
  cout << "I: " << i << '\n';
  cout << "LO: ";
  for (auto &v : lo)
    cout << v << ' ';
  cout << "\nHI: ";
  for (auto &v : hi)
    cout << v << ' ';

  cout << "\nMEDIAN: " << *lo.rbegin() << "\n\n";
}

int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  cout.tie(0);

  ll n, k;
  cin >> n >> k;

  vector<ll> vec(n);
  for (auto &v : vec)
    cin >> v;

  multiset<ll> lo, hi;
  ll l = 0, r = 0;
  ll median_left = k % 2 == 0 ? k / 2 - 1 : k / 2;
  ll median_right = k / 2;
  for (ll i = 0; i < k; ++i) {
    lo.insert(vec[i]);
    l += vec[i];
  }

  for (ll i = 0; i < k / 2; ++i) {
    ll tmp = *lo.rbegin();
    l -= tmp;
    r += tmp;
    lo.erase(prev(lo.end()));
    hi.insert(tmp);
  }

  ll median = *lo.rbegin();
  l -= median;
  if (k == 1)
    cout << "0 ";
  else
    cout << abs(r - median * median_right) + abs(median * median_left - l)
         << ' ';
  l += median;

  for (ll i = k; i < n; ++i) {
    ll tmp = vec[i];

    if (k == 1) {
      cout << 0 << ' ';
      continue;
    }

    if (tmp > median) {
      hi.insert(tmp);
      r += tmp;
    } else {
      lo.insert(tmp);
      l += tmp;
    }

    ll to_remove = vec[i - k];
    if (to_remove > median) {
      hi.erase(hi.find(to_remove));
      r -= to_remove;
    } else {
      lo.erase(lo.find(to_remove));
      l -= to_remove;
    }

    if (hi.size() > lo.size()) {
      ll tmp = *hi.begin();
      hi.erase(hi.find(*hi.begin()));
      lo.insert(tmp);
      r -= tmp;
      l += tmp;
    } else if (lo.size() > hi.size() + 1) {
      ll tmp = *lo.rbegin();
      lo.erase(lo.find(*prev(lo.end())));
      hi.insert(tmp);
      l -= tmp;
      r += tmp;
    }

    median = *lo.rbegin();
    l -= median;
    cout << abs(r - median * median_right) + abs(median * median_left - l)
         << ' ';
    l += median;
  }
  cout << '\n';
}
