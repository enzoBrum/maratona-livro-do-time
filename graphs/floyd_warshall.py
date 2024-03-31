'''
Returns the distances between all vertex pairs in O(n³)
Overrides the matrix "m" of distances between vertices.

IMPORTANT: the matrix "m" must have value INFINITY for 
vertices that don't have a direct connection between them.
'''

def floyd_warshall(m):
    n = len(m)
    for k in range(n):
        for i in range(n):
            for j in range(n):
                m[i][j] = min(m[i][j], m[i][k] + m[k][j])
    return m
