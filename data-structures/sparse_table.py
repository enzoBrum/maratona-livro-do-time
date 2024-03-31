
from math import log2
from typing import Callable

class SparseTable:
    """
    - Consegue responder range minimum queries em O(1) [P.s: range minimum queries --> menor número dentro de um intervalo]
    - Não pode ser atualizada
    - construida em O(n log n)

    - como funciona: The main idea behind Sparse Tables is to precompute all answers for range queries with power of two length.
                     Afterwards a different range query can be answered by splitting the range into ranges with power of two lengths, 
                     looking up the precomputed answers, and combining them to receive a complete answer.
    """
    def __init__(self, arr: list, func,max_n: int):
        """
        func: função que depende do tipo de query.
        max_n: maior quantidade possível de elementos
        """

        k = int( log2(max_n) ) + 1

        self.st = [[0 for i in range(k)] for j in range(max_n)]

        self.func = func

        for i, v in enumerate(arr):
            self.st[i][0] = v

        for j in range(1, k):
            for i in range(len(arr)):
                if i + (1 << j) - 1 >= len(arr):
                    break

                self.st[i][j] = func( 
                    self.st[i][j-1], 
                    self.st[i + (1 << (j-1))][j-1] 
                )


    def query(self, L: int, R: int):
        # Assumes 1-indexed range

        R-=1; L-=1
        k = int(log2(max(R-L, 1)))

        return self.func( self.st[L][k], self.st[R - (1 << k) + 1][k] )

