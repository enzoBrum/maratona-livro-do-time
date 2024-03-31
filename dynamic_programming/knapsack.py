'''
Returns largest possible sum of elements' values such that the sum
of the weights of these elements does not exceed "capacity".
"weights" and "values" are 0 indexed (i.e. index 0 is not empty).
Elements can be used only once.
'''
def knapsack(capacity, weights, values, element_count=None):
    if element_count is None:
        element_count = len(weights)
    dp = [0] * (capacity + 1)
    
    for i in range(element_count):
        for w in range(capacity, 0, -1):
            if w >= weights[i]:
                dp[w] = max(dp[w], dp[w-weights[i]] + values[i])
            else:
                break
    return dp[capacity]
