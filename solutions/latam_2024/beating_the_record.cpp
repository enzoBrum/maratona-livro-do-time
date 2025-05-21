#include <bits/stdc++.h>

using namespace std;

#define int long long
const int INF = 0x3F3F3F3F;
using ii = pair<int, int>;
using ll = long long;
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()

const bool DEBUG = 0;

/*
Exercise:
A guy wants to beat a world record of a game. He has 2 strategies per level, each one having a chance of
succeeding and failing, and a time to go through the level on each circumstance. The most interesting thing
is that he can restart the whole thing whenever he wants. Compute the expected play time until the record
is beaten (circular dp dependency of the first level with itself because of restarting).

Idea:
Say a state is determined by its level 'i' and the 'tt' seconds that you have been playing on this run.
Then, you can compute the expected remaining time of gaming simply with states at level 'i+1' and
time tt1 > tt, except for the possibility of restarting. But since restarting depends on the levels 
themselves, you cannot solve for the expected total time. The deal is that this starting expected time is
binary searchable, because:

1) The expected time you guessed must be equal to the expected time you get by computing it from
its children.
2) If you guessed too low, then, it won't be worth it trying and every level will just restart. Then, the
computed expected will be roughly the guess itself (it should be equal, but since we compute it S steps
further, then, the computed value will be probably the guess + S).
3) If you guessed too high, then, the walk itself will be better than the expected, which doesn't make sense.
In that case, you continue the search below (high = mid).

Therefore, the higher the guess, the smaller the computed expected, and you need to equalize them.
Time complexity: O(B2^n), where B = binary search steps.
"Improvable" to O(B*(MAXT)*n) if n was larger (because of dp).

Details about expected computation:
In any level, you can choose between strat1 and strat2, and, for each strat, you can either continue or
restart, depending on whether it failed or not. That's why you will always min(restart, smt) everywhere
(because you can restart anywhere).
*/

using ftype = long double;
int N, S, T;
ftype restart;
ftype G[4][2];
ftype B[4][2];
ftype P[4][2];

ftype expected(int i, int tt) {
    if (tt >= T) return restart;
    if (i == N) return 0;

    ftype ans = restart;
    for (int strat = 0; strat < 2; strat++) {
        ftype good =    P[i][strat] * min(restart, G[i][strat] + expected(i+1, tt+G[i][strat]));
        ftype bad = (1-P[i][strat]) * min(restart, B[i][strat] + expected(i+1, tt+B[i][strat]));
        ans = min(ans, good+bad);
    }

    return ans;
}

signed main() {
    cin.tie(0)->sync_with_stdio(0);

    cin >> N >> T >> S;

    for (int i = 0; i < N; i++) {
        for (int s = 0; s < 2; s++) {
            cin >> P[i][s] >> G[i][s] >> B[i][s];
            P[i][s] /= 100;
        }
    }

    ftype low = S;
    ftype high = 1e18;

    for (int i = 0; i < 150; i++) {
        ftype mid = (low + high) / 2;
        restart = mid;
        ftype computed = expected(0, S) + S;
        if (mid <= computed) low = mid;
        else high = mid;
    }

    cout << setprecision(20) << fixed << low << '\n';

    return 0;
}
