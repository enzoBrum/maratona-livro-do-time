// xor from [0, n] (the same as [1, n])
int prefixXor(int n) {
    int mod = n % 4;
    if (mod == 0) return n;
    if (mod == 1) return 1;
    if (mod == 2) return n+1;
    return 0;
}

int rangeXor(int l, int r) {
    return prefixXor(l-1) ^ prefixXor(r);
}
