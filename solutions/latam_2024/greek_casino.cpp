#include <bits/stdc++.h>

using namespace std;

//#define int long long
const int INF = 0x3F3F3F3F;
using ii = pair<int, int>;
using ll = long long;
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()

const bool DEBUG = true;
using ftype = double;

/*
Exercise:
There are n slots (from 1 to n). You start at slot 1. Say you are currently at slot 'x'. A random integer
'y' from 1 to n is chosen. If z=lcm(x, y) > n, then, the game ends. Else, you receive 1 coin and you go
to slot z. Print the expected number of coins given the probabilities of each number being selected.

Idea:
Let's build a Markov Chain of the input. To compute the edges, I'll use the fact that a
vertex 'i' only goes to a multiple of 'i'. That bounds the amount of edges by O(nlogn).
For each edge (i, m*i), I wanna see what 'j's randomly selected would lead me to go through it.
That is, which j have the following property:
lcm(i, j) = m*i
i*j/gcd(i, j) = m*i
j = m * gcd(i, j)

I don't know how to solve that mathematically, but I do know how to bruteforce it.
Iterate through every divisor 'g' of 'i' and consider it as a gcd of something. Compute
j = m*g
and see if that makes the equation true. If it does, we found a working 'j'.

Total complexity: O(nlog³n)
*/

vector<vector<int>> divsSieve(int n) {
    vector<vector<int>> divisors(n+1);
    for (int i = 1; i <= n; i++)
        for (int j = i; j <= n; j += i)
            divisors[j].push_back(i);
    return divisors;
}

signed main() {
    int n;
    scanf("%d", &n);

    vector<int> w(n+1);
    int wSum = 0;
    for (int i = 1; i <= n; i++) {
        scanf("%d", &w[i]);
        wSum += w[i];
    }

    vector<vector<pair<int, int>>> adj(n+1);
    vector<vector<int>> divisors = divsSieve(n);

    for (int i = 1; i <= n; i++) {                  // O(n)
        for (int m = 2; i*m <= n; m++) {            // O(logn average) because of harmonic series
            int likelihood = 0;
            for (long long g : divisors[i]) {       // O(logn average) because of sieve
                if (g*m > n) break;
                if (gcd(i, g*m) == g)               // O(logn)
                    likelihood += w[g*m];
            }
            if (likelihood)
                adj[i].push_back({likelihood, i*m});
        }

        // now self edge
        int selfLikelihood = 0;
        for (int g : divisors[i])
            selfLikelihood += w[g];
        adj[i].push_back({selfLikelihood, i});
    }
    
    vector<ftype> expected(n+1, 0);
    for (int i = n; i >= 1; i--)
        for (auto[w, next] : adj[i]) {
            ftype s = ftype(w) / wSum;
            if (next == i)
                expected[i] = (expected[i] + s) / (1-s);
            else
                expected[i] += s * (expected[next]+1);
        }
    printf("%.20f", expected[1]);
    return 0;
}
