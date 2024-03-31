"""
Conta inversões presentes em um array

3,2,4,5

3 e 2 representam uma inversão (2 está a frente de 3, mas é menor que ele)
"""



from fenwick_tree import FenwickTree



def normalize(iteratable) -> dict[any, int]:
    """
    Cria um dicionário mapeando cada elemento do array para um número no intervalo [1, len(iteratable)]
    """
    sorted_iteratable = sorted(iteratable)

    mp = {}
    num = 1

    for val in sorted_iteratable:
        if val not in mp:
            mp[val] = num
            num += 1
    return mp

def inversion_count(iteratable) -> int:
    """
    conta a quantidade total de inversões encontradas no array.
    """

    """
    A fenwick tree conta a frequência de elementos encontrados no array até o momento, permitindo
    a realização de queries para saber quantos valores já apareceram em um determinado intervalo de números. 
    por exemplo: 
        suponha que um loop itere sobre os valores 5 4 3 2 1.

        na terceira iteração do for loop (quando o valor for 3),
        a árvore indicará que foram encontrados dois valores no intervalo [3:5]

    isso permite encontrar inversões de forma rápida, uma vez que tudo que precisamos fazer para encontrar todas 
    as inversões de um número n é descobrir quantos números maiores que ele aparecem antes dele.
    i.e: bast fazer uma query a fenwick tree no intervalo [n:len(iteratable)].
    """
    
    ft = FenwickTree(len(iteratable))
    mp = normalize(iteratable)
    inv = 0
    for val in iteratable:
        inv += ft.query(mp[val], len(iteratable))
        ft.update(mp[val], 1)
    return inv
