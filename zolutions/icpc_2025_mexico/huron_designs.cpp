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
You are given n <= 20 projects with deadlines, profits for completion and duration
to do. There is a value 'y' such that if you finish the project before time 'y', you
get a bonus 'x' of money. Actually, 'y' nor 'x' are known, but they are given as a range
[ly, ry] and [lx, rx] of real numbers, and you know that each of the numbers lie on their
respective intervals with equiprobable chance.
Compute the maximum expected profit of any subset of projects completed in any order.

Idea:
Do a dp(mask) that computes the maximum profit of completing ALL the projects of the
mask. The transitions are, for each project of the mask, complete it after all of the
other projects (dp[mask] = dp[mask^i] + profit of i) for all i E mask. To compute the
profit of some project 'i' completed on time 'e', you have to compute the probability
that 'y' ended up larger than 'e', and multiply that by the expected x ((lx+rx)/2)).
If its not possible to complete any ordering of the mask, return some negative number.
Finally, grab the mask with the largest profit.
*/

struct Proj {
    int dead, profit, dur, lx, rx, ly, ry;
    double getProfit(int end) {
        assert(end <= dead);

        double yprob;
        if (end <= ly) yprob = 1;
        else if (end >= ry) yprob = 0;
        else yprob = 1.0*(ry-end) / (ry-ly);

        double expectedX = (lx+rx)/2.0;
        return profit + yprob * expectedX;
    }
};

vector<Proj> projects;
int n;
const int MAXN = 20;
double dp[1<<MAXN];
int done[1<<MAXN];
int totalDuration[1<<MAXN];

// returns the maximum profit using exactly this mask
double maxProfit(int mask) {
    if (mask == 0) return 0;
    if (done[mask]) return dp[mask];

    // try each element of the mask as the last one
    double ans = -1;
    for (int i = 0; i < n; i++) {
        if (mask & (1<<i)) {
            int prevMask = mask ^ (1<<i);
            double prev = maxProfit(prevMask); // best ordering without the current project
            if (prev >= 0 && totalDuration[prevMask] + projects[i].dur <= projects[i].dead)
                prev += projects[i].getProfit(totalDuration[prevMask] + projects[i].dur);
            ans = max(ans, prev);
        }
    }

    done[mask] = 1;
    return dp[mask] = ans;
}

signed main() {
    cin.tie(0)->sync_with_stdio(0);

    cin >> n;
    projects.resize(n);
    for (Proj &p : projects)
        cin >> p.dead >> p.profit >> p.dur >> p.lx >> p.rx >> p.ly >> p.ry;

    for (int mask = 0; mask < (1<<n); mask++)
        for (int i = 0; i < n; i++)
            if (mask & (1<<i))
                totalDuration[mask] += projects[i].dur;

    double ans = 0;
    for (int mask = 0; mask < (1<<n); mask++)
        ans = max(ans, maxProfit(mask));
    cout << setprecision(10) << fixed << ans << '\n';
    
    return 0;
}
