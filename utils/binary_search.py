'''
Returns index of target, in ascending iterables. 
For descending iterables, swap "<" with ">".
If number doesn't exist, returns -1.
'''
def binary_search(iterable, target, down_idx=0, top_idx=None):
    if top_idx is None:
        top_idx = len(iterable) - 1
        
    while down_idx <= top_idx:
        cur = (down_idx + top_idx) // 2
        if iterable[cur] == target:
            return cur
        elif iterable[cur] < target: #swap here
            down_idx = cur + 1
        else:
            top_idx = cur - 1
    return -1
            
    
    
'''  
Returns index of smallest number in iterable bigger than or equal
to target, in ascending iterables.
Swapping ">=" with "<=" returns index of biggest number in iterable
smaller than or equal to target, in descending iterables.
If number doesn't exist, returns -1.
'''
def binary_search(iterable, target, down_idx=0, top_idx=None):
    if top_idx is None:
        top_idx = len(iterable) - 1
    res = -1
    
    while down_idx <= top_idx:
        cur = (down_idx + top_idx) // 2
        if iterable[cur] >= target: #swap here
            res = cur
            top_idx = cur - 1
        else:
            down_idx = cur + 1
    return res
  
  
  
'''  
Returns index of smallest number in iterable bigger than or equal
to target, in descending iterables.
Swapping ">=" with "<=" returns index of biggest number in iterable
smaller than or equal to target, in ascending iterables.
If number doesn't exist, returns -1.
'''
def binary_search(iterable, target, down_idx=0, top_idx=None):
    if top_idx is None:
        top_idx = len(iterable) - 1
    res = -1
    
    while down_idx <= top_idx:
        cur = (down_idx + top_idx) // 2
        if iterable[cur] >= target: #swap here
            res = cur
            down_idx = cur + 1
        else:
            top_idx = cur - 1
    return res
