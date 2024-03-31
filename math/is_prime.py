from math import sqrt


def is_prime(n):
    sq = int(sqrt(n))
    
    if n % 2 == 0:
        return False
    for i in range(3, sq + 1, 2):
        if n % i == 0:
            return False
    return True
