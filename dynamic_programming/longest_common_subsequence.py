'''
Returns the length of the longest common subsequence
between strings "p" and "q". The sequence doesn't need
to be contiguous.
Example:
p -> "ABCXYZ"
q -> "ABXPZ"
longest_common_subsequence(p, q) -> "ABXZ", of length 4.
'''
def longest_common_subsequence(p, q):
    dp = [[0] * (len(q) + 1) for _ in range(len(p) + 1)]
    
    for i in range(len(p)):
        for j in range(len(q)):
            if p[i] == q[j]:
                dp[i+1][j+1] = 1 + dp[i][j]
            else:
                dp[i+1][j+1] = max(dp[i+1][j], dp[i][j+1])
    return dp[len(p)][len(q)]
