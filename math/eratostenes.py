#Returns ascending list of primes until "n", inclusive.
def primes(n):
    is_prime = [True] * (n + 1)
    primes = [2]
    
    for i in range(3, n + 1, 2):
        if is_prime[i]:
            primes.append(i)
            for j in range(i + i, n + 1, i):
                is_prime[j] = False
    return primes
