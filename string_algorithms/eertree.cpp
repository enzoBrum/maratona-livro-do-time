/*
An Eertree is a graph where each vertex represents a unique palindromic substring.
Every palindrome is made by adding a letter to the front and the back of a palindrome. The base cases
are the empty palindrome (of length 0) and the imaginary palindrome (of length -1). That is convenient
because it allows us to dinamically (online) extend an eertree only by keeping track of its already
existing palindromes.
 
Claim: every new unique palindromic substring gotten by appending a new character to the string
necessarily is a suffix of the string.
Proof: if it doesn't include the last character, it existed before it. So it must include the last (and
is therefore a suffix).
 
Claim: appending a letter to a string increases the amount of unique palindromic substrings by at most one.
Proof: if the letter is new, clearly, the only new palindrome is itself.
Else, let's assume 2 new palindromes were created. Then, there must be a suffix of the form:
cyczc
Where "c" is the new character added, with both "ycz" and "z" being palindromes, with "y" and "z" possibly
empty and with the 2 new palindromes being "cyczc" and "czc".
 
Let s' be the reverse of a string s. Then, because "ycz" is a palindrome, we have that:
cyczc
= cz'cy'c
But since z is a palindrome, z' = z, so
= czcy'c
And we have that "czc" already appeared before appending "c" (absurd).
 
So, our data structure starts with 2 vertices, the "real" length 0 palindromic substring, and the "imag"
length -1 palindromic substring. Every vertex has a "link" pointing to its longest proper palindromic
suffix and a vector of labeled edges indicating the palindromes that can be made by adding letters on
the start and end of the current string. 
 
We will keep track of the longest palindromic suffix of the current string (from now on called "last",
and initialized as "real"), and will progressively add characters of the string we wanna build the tree of.
There is a function "getLink()" that returns the largest suffix that is also palindromic when appended the
new character.
It is clear that getLink(last) is the only CANDIDATE for being a new vertex, because any other suffix is
contained on getLink(last), and, therefore, falls into the absurd proven before (it is never new). So
we set last = getLink(last) (and from now on I'll refer to the new value as "last").
 
After we found such vertex, last will become c + last + c (that is, to[last][c]). Luckily this is valid
for len[last] = -1, too. If vertex c + last + c didn't exist, we simply create it.
 
Creating a vertex consists of finding its link and plugging the last's "to" to it. Its link is clearly
the longest palindromic substring that is NOT to[last][c], so you simply try to find it from last's link
(v = getLink(link[last])) and then append "c" to that. It is clear that operation is valid if len[v] is 
>= 0 (because to[v][c] is a palindromic suffix contained in to[last][c], and, therefore, is already
initialized). 
But what if len[v] = -1?
If len[last] = -1, then, last == imag, and if we are creating a vertex, it means that
to[last][c] = to[imag][c] is uninitialized. If we set the uninitialized value as REAL, then, everything
will work as intended.
If len[last] >= 0 (and len[v] = -1), it means to[v][c] is a suffix of to[last][c], and, because of our
theorem, it is already initialized. Therefore, it is safe to grab to[v][c].
*/
 
const int MAXN = 1e6;
const int E = 26; // alphabet
 
struct Eertree {
    const static int REAL = 0;
    const static int IMAG = 1;
 
    int len[MAXN+2], start[MAXN+2], link[MAXN+2], cnt[MAXN+2], to[MAXN+2][E];
    //map<int, int> to[MAXN+2];
    int s[MAXN+2]; // the actual string is s[2:n-1]
    int n = 2, sz = 2, last = REAL, totalPalindromes = 0;
    int lps, lpsIdx; // longest palindromic substring
 
    Eertree() {
        memset(len, 0, sizeof(len));
        memset(start, 0, sizeof(start));
        memset(link, 0, sizeof(link));
        memset(cnt, 0, sizeof(cnt));
        memset(to, 0, sizeof(to)); // REMOVE THIS WHEN USING std::map
        len[IMAG] = -1;
        link[IMAG] = link[REAL] = IMAG;
        s[IMAG] = s[REAL] = -1; // value that should not be on alphabet
    }
 
    int getLink(int v) {
        while (s[n - len[v] - 2] != s[n - 1])
            v = link[v];
        return v;
    }
 
    void addLetter(int c) {
        s[n++] = c;
        last = getLink(last);
 
        //if (!to[last].count(c)) {
        if (!to[last][c]) {
            len[sz] = len[last] + 2;
            //if (len[last] == -1 && len[getLink(link[last])] == -1)
            //    to[last][c] = REAL;
            link[sz] = to[getLink(link[last])][c];
            start[sz] = n-2-len[sz];
            to[last][c] = sz++;
        }
        last = to[last][c];
        cnt[last]++;
    }
 
    void setupData() {
        totalPalindromes = lps = lpsIdx = 0;
        // this iterates over every vertex
        for (int i = sz-1; i >= 2; i--) {
            totalPalindromes += cnt[i];
            cnt[link[i]] += cnt[i];
            if (len[i] > lps) {
                lps = len[i];
                lpsIdx = start[i];
            }
        }
    }
 
    inline int palindromeCount() { return totalPalindromes; }
    inline int uniquePalindromeCount() { return sz - 2; }
    inline ii longestPalindromicSubstring() { return {lpsIdx, lps}; }

    /*
    You can dinamically keep track of palindromic substring count by doing something like this:
    
    // v should be to[last][c], that is, "last" after adding a letter.
    int increment = 0;
    while (v != REAL) {
        increment++;
        v = link[v];
    }
    totalPalindromes += increment;

    While also keeping a rollback to undo operations.
    */
};
