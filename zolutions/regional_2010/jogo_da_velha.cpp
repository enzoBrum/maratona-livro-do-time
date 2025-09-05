/*
Exercise:
You are given a 1D board with some X's placed. Players alternate placing X's on the board.
The first player to place 3 consecutive X's wins. Tell whether the first player has a
winning strategy.

Idea:
The game, as given, is, "if you end up doing something, you win". To map it to
grundy and stuff, it must be of the form "if you cannot make a move, you are doomed".
We can go a step back from the original game and set the move "placing an x at distance
d <= 2 from another x" as invalid, so whenever you can't NOT do that, it is because you
are about to give up the win to your opponent, and this matches the grundy idea of "if
you can't move, you lose".
This means we can work up with ranges of size up to n that are the positions where you can play.
Whenever you place an 'x', you split that range in two. Then, you can play in either one. Is this
merged state winning or losing? If they were simply 2 nim piles, then, its clear their xor
determines if its winning or losing. Since grundy gives the equivalent nim pile size, then, compute
their grundy and xor them.
The code below uses a "found" vector to compute the mex, but its the same as if it were a set.
*/

const int MAXN = 1e4;
int grundy[MAXN+1];

int top = 0;
int nums[MAXN+1];
int found[MAXN+1];

int ceildiv(int a, int b) { return (a+b-1)/b; }

signed main() {
    cin.tie(0)->sync_with_stdio(0);

    grundy[0] = 0; // no moves, losing. By definition, mex == 0.
    grundy[1] = 1; // one single move, that leads to g[0]. Mex == 1
    grundy[2] = 1; // any move leads to g[0]. Mex == 1

    for (int i = 3; i <= MAXN; i++) {
        top = 0;
        for (int pos = 0; pos < ceildiv(i, 2); pos++) {
            int left = max(0, pos-2);
            int right = max(0, i-1-(pos+2));
            nums[top] = grundy[left] ^ grundy[right];
            found[nums[top]] = 1;
            top++;
        }
        int mex = 0;
        while (found[mex])
            mex++;
        grundy[i] = mex;
        for (int i = 0; i < top; i++)
            found[nums[i]] = 0;
    }

    while (true) {
        int n; cin >> n;
        if (!n) break;
        string s; cin >> s;

        // checking case where game was already given up
        int last = -10;
        bool done = false;
        for (int i = 0; i < n; i++) {
            if (s[i] == 'X') {
                if (i-last <= 2) {
                    done = true;
                    break;
                }
                last = i;
            }
        }

        if (done) {
            cout << "S\n";
            continue;
        }
        
        // this is so I handle the last segment as "in between Xs" (no edge case)
        // this is neutral because the right boundary will be the right X pos - 3.
        s += "..X";
        int accXor = 0;
        int prevX = -3; // dominates -2 and -1, you can play safely from 0 forwards
        for (int i = 0; i < s.size(); i++) {
            if (s[i] == 'X') {
                int sz = max(0, (i-3)-(prevX+3)+1);
                accXor ^= grundy[sz];
                prevX = i;
            }
        }
        cout << (accXor ? "S\n" : "N\n");
    }
    
    return 0;
}