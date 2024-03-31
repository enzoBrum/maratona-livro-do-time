'''
Returns minimum amount of coins from the "coins" list
such that their sum is equal to "val".
Every element on the "coins" list can be used an unlimited
amount of times.
If no sum of coins is equal to "val", returns -1.
'''
def coin_change(coins, val):
    dp = [float("inf")] * (val + 1)
    dp[0] = 0

    for amount in range(1, val + 1):
        for coin in coins:
            if amount - coin >= 0:
                temp = 1 + dp[amount - coin]
                if temp < dp[amount]:
                    dp[amount] = temp
    return dp[val] if dp[val] != float("inf") else -1
