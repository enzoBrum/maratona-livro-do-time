#include <bits/stdc++.h>

using namespace std;

#define int long long
const int INF = 0x3F3F3F3F;
using ii = pair<int, int>;
using ll = long long;
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()

const bool DEBUG = true;

/*
Exercise:
You are given a permutation vector with numbers [1, N], some possibly missing (a 0 is placed when some
number is missing). You wanna compute any way to fill those gaps such that the final vector ends up as
a mountain permutation (strictly increasing then strictly decreasing), or tell that it is impossible.

Idea:
Either end of the mountain necessarily is 1. After we place the 1, we recursively build the mountain
for the remaining positions [2, N]. That will be the general idea of the algorithm.
There are a few cases where there is only one way to place the 1, namely:
1) The whole array only has zeroes:
    place the 1 on the left and build recursively (this will end up as an increasing sequence).
2) The 1 is already placed on the mountain:
    if its in one of the ends, alright, continue building the mountain in the opposite direction.
    else, abort (no solution).
3) There is some end that is non-zero:
    you will place the 1 in the end that is zeroed. If no end is zeroed, abort.
4) Both ends are zero:
    grab the leftmost non-zero index 'i' such that i >= l.
    grab the rightmost non-zero index 'j' such that j <= r
    let x = a[i], y = a[j]
    if x < y, then, we know that the mountain peak CANNOT be in a position <= i, because that would imply
    the height decreasing to x but then increasing again to y. If the peak is not before i, then, the segment
    [l, i] is always increasing. Therefore, there is only one position for the 1, namely, at a[l].
    Symmetric argument applies to x > y.
    What if x == y (i == j)? Well, I claim that placing the 1 on the smallest contiguous sequence of zeroes
    is optimal.
    "Proof":
    You need to make 'i' become a border as soon as possible. If you take too long, you will reach the
    case "2)", but without it being on the border - abort. Reaching it early is no problem, because
    every number smaller than it that was not placed will always be placeable on the other side (because
    the other side will be always free).

Therefore, the algorithm iterates from 1 to N and builds the mountain progressively. If something fails
in the middle, abort. Else, you end up with a valid mountain.
*/

vector<int> a, pref, nonZeroRight, nonZeroLeft, placed;

bool buildMountain(int n) {
    auto isAllZeroes = [&](int l, int r) {
        int sum = pref[r];
        if (l != 0) sum -= pref[l-1];
        return sum == 0;
    };

    int l = 0;
    int r = n-1;
    for (int cur = 1; cur <= n; cur++) {
        if (isAllZeroes(l, r)) a[l++] = cur;
        else if (placed[cur]) {
            if (a[l] == cur) l++;
            else if (a[r] == cur) r--;
            else return false;
        }
        else if (a[l] != 0 || a[r] != 0) {
            if (a[l] == 0) a[l++] = cur;
            else if (a[r] == 0) a[r--] = cur;
            else return false;
        }
        else { // both are zero
            int x = a[nonZeroRight[l]];
            int y = a[nonZeroLeft[r]];
            if (x < y) a[l++] = cur;
            else if (x > y) a[r--] = cur;
            else { // x == y
                int dx = nonZeroRight[l] - l;
                int dy = r - nonZeroLeft[r];

                if (dx < dy) a[l++] = cur;
                else a[r--] = cur;
            }
        }
    }
    return true;
}

signed main() {
    cin.tie(0)->sync_with_stdio(0);

    int n;
    cin >> n;

    a.resize(n);
    for (int &i : a) cin >> i;

    pref.resize(n);
    pref[0] = a[0];
    for (int i = 1; i < n; i++)
        pref[i] = pref[i-1]+a[i];

    nonZeroLeft.assign(n, 0);
    nonZeroRight.assign(n, 0);

    nonZeroLeft[0] = 0;
    for (int i = 1; i < n; i++)
        if (a[i] != 0) nonZeroLeft[i] = i;
        else nonZeroLeft[i] = nonZeroLeft[i-1];

    nonZeroRight[n-1] = n-1;
    for (int i = n-2; i >= 0; i--)
        if (a[i] != 0) nonZeroRight[i] = i;
        else nonZeroRight[i] = nonZeroRight[i+1];

    placed.assign(n+1, 0);
    for (int i : a)
        if (i)
            placed[i] = 1;

    if (!buildMountain(n)) cout << "*\n";
    else {
        for (int i : a)
            cout << i << " ";
        cout << '\n';
    }
    
    return 0;
}