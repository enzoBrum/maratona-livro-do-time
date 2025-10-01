/*
This one finds the SUM of the numbers x <= n with popcount(x) == k
The dp also returns how many numbers there are because we will sum 2^i
for each valid number found after the recursive call.
"num" is a vector of the digits of the number.

Random note about exercise "count numbers divisible by their digit sum":
Keeping track of divisibility is easy if you already know the divisor (do the (10*r+digit)%div
trick). But since we don't know the sum of the digits beforehand, we will have to try every
possible digit sum and then check in the dp state if the actual digit sum matched.
*/

vector<int> num;
vector<vector<vector<ii>>> dp;
int k;

// {sum, cnt}
ii count(int i, int sum, int tight) {
    if (sum > k) return {0, 0};
    if (i == num.size()) return {0, sum==k};
    if (dp[i][sum][tight].first != -1) return dp[i][sum][tight];

    int totalSum = 0;
    int totalCnt = 0;
    for (int v = 0; v <= (tight ? num[i] : 1); v++) {
        auto[othersum, cnt] = count(i+1, sum+v, tight && v == num[i]);
        int p = (1ll << (num.size()-1-i))%MOD;
        totalSum = add(totalSum, p*v * cnt % MOD);
        totalSum = add(totalSum, othersum);
        totalCnt = add(totalCnt, cnt);
    }
    return dp[i][sum][tight] = {totalSum, totalCnt};
}
