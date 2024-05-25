class FenwickTree:
    """
    - Consegue responder range queries em O(log n)
    - Consegue atualizar um elemento por vez em O(log n)
    - O(n log n) pra construir
    - indexada por 1
    - queries tambem devem ser indexadas por 1
    """

    """
    Dado um nodo n
        - posição do filho: n - n&-n
        - posição do pai: n + n&-n
    """
    def __init__(self, n):
        self.ft = [0 for _ in range(n+1)]


    def update(self, i: int, v: int):
        """incremento o item na posição `i` com o valor `v`"""
        while i < len(self.ft):
            self.ft[i] += v
            i += i&-i


    def query(self, L: int, R: int):
        """retorna soma no intervalo [L, R]"""
        return self.__sum_from_beginning(R) - self.__sum_from_beginning(L-1)

    
    def __sum_from_beginning(self, i: int):
        """Retorna soma no intervalo [1, i]"""

        sum = 0
        while i:
            sum += self.ft[i]
            i -= i&-i

        return sum



