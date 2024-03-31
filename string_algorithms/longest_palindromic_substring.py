'''
Manacher's algorithm. There are two implementations of it below.
First implementation:
 - Returns the longest palindromic substring of "s" with smallest starting index.
Second implementation:
 - Returns the length of the longest palindromic substring of "s" (a little bit
   faster because doesn't need to slice the substring out of "s").
All of this in O(n) time complexity.
First time writing this algo. Do not ask me how it works.
'''
#Returns the actual substring.
def longest_palindromic_substring(s):
    str = "#" + "#".join(s) + "#"
    c = 0
    r = 0
    lps = [0] * len(str)
    best_length = 0
    best_idx = 0
    
    for i in range(1, len(str) - 1):
        if i < r:
            lps[i] = min(r-i, lps[2*c - i])
        while len(str) - 1 - lps[i] > i and str[i + 1 + lps[i]] == str[i - 1 - lps[i]]:
            lps[i] += 1
        if lps[i] > best_length:
            best_length = lps[i]
            best_idx = i
        if i + lps[i] > r:
            c = i
            r = i + lps[i]
    return s[(best_idx - best_length)//2 : (best_idx + best_length)//2]

#Returns the length of the substring.
def longest_palindromic_substring(s):
    str = "#" + "#".join(s) + "#"
    c = 0
    r = 0
    lps = [0] * len(str)
    best_length = 0
    
    for i in range(1, len(str) - 1):
        if i < r:
            lps[i] = min(r-i, lps[2*c - i])
        while len(str) - 1 - lps[i] > i and str[i + 1 + lps[i]] == str[i - 1 - lps[i]]:
            lps[i] += 1
        if lps[i] > best_length:
            best_length = lps[i]
        if i + lps[i] > r:
            c = i
            r = i + lps[i]
    return best_length
