'''
Returns ascending list of prime factors of "n".
Repetitions allowed.
'''
def factorize(n):
    factors = []
    while n % 2 == 0:
        factors.append(2)
        n //= 2
    i = 3
    while n > 1:
        while n % i == 0:
            factors.append(i)
            n //= i
        i += 2
    return factors
