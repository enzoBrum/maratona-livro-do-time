'''
Returns list with the "string" indexes on which the pattern "substr" starts.
'''
def build_lps_list(sub):
    lps = [0] * len(sub)

    for i in range(1, len(sub)):
        j = lps[i-1]
        while j > 0 and sub[i] != sub[j]:
            j = lps[j-1]
        if sub[i] == sub[j]:
            j += 1
        lps[i] = j
    return lps


def knuth_morris_pratt(substr, string):
    lps = build_lps_list(substr)
    i = 0
    j = 0
    res = []

    while i < len(string):
        if string[i] == substr[j]:
            i += 1
            j += 1
            if j == len(substr):
                res.append(i-j)
                j = lps[j-1]
        elif j == 0:
            i += 1
        else:
            j = lps[j-1]
    return res
