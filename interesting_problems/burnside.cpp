signed main() {
    /*
    Given a crown with k slots for gems, where each slot can be filled in m ways, find the amount
    of different crowns that can be made (considering rotations give the same crown).
    Then, sum it all for all k in [1, n].
    This algorithm is slow, but is didactic for the Burnside's Lemma.
    
    Burnside's Lemma:
    (1/|G|) * sum(for g : G) |X^g|

    |G| is the amount of operations: in this case, k cyclic shifts.
    X^g is the set of elements that are equivalent under the action of g (possibly several times).
    Therefore, |X^g| is the amount of sets of elements that are equivalent under g (applied several times).
    
    In this case, we have k rotations, denoted by p_j (j is the amount of positions moved to the right).

    p_0 -> by not rotating anything, all the arrangements are unique: m^k arrangements
    p_1 -> only all the gems equal are equivalent: m arrangements
    ...
    p_j -> the amount of visited gems is k / gcd(k, j). Therefore, the pattern size is gcd(k, j). Since we can choose
    each of the gems of the pattern in m ways, the total ways to choose the pattern (and therefore the equivalent crowns with
    j rotations) is m^gcd(k, j)
    */

    int n, m;
    cin >> n >> m;
    int ans = 0;

    for (int k = 1; k <= n; k++) {
        int prelim = 0;
        for (int j = 0; j < k; j++) {
            int g = gcd(k, j);
            prelim = (prelim + fastPow(m, g)) % MOD;
        }

        // now dividing by k
        prelim = prelim * multiplicativeInverse(k, MOD) % MOD;
        ans = (ans + prelim) % MOD;
    }

    cout << ans << endl;
    return 0;
}
