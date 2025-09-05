/*
An Eertree is a graph where each vertex represents a unique palindromic substring.
Every palindrome is made by adding a letter to the front and the back of a palindrome. The base cases
are the empty palindrome (of length 0) and the imaginary palindrome (of length -1). That is convenient
because it allows us to dynamically (online) extend an eertree only by keeping track of its already
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

About keeping non-unique palindrome count:
Every time we add a character, we find the largest palindromic string that ends on that character. The
palindromic substrings we missed counting are those that end on the new character but are not the largest
palindromic substring that ends there. Namely, we missed the link of the longest palindromic suffix, then
the link of the link of the longest palindromic suffix, and so on. This means that we can keep an additional
vector "depth" that tells us the distance from a vertex to the REAL vertex, and whenever we append a character,
we know that the total amount of palindromic substrings increased by depth[curVertex].
If you need the specific count of each string, you can propagate the count from the last added vertices to
the older ones through the suffix links, but that will be up to O(n):
for (int i = sz-1; i >= 2; i--)
    cnt[link[i]] += cnt[i];
*/
 
struct Eertree {
    const int REAL = 0;
    const int IMAG = 1;
    const static int ALPHABET = 26;

    vector<int> s;
    struct Vertex { 
        int len, start, link, cnt, depth;
        array<int, ALPHABET> to{}; // map<int, int> to;
    };
    vector<Vertex> t;

    // lps stands for longest palindromic substring
    int n=2, sz=2, last=REAL, totalPalindromes=0, lps=0, lpsPos;
 
    Eertree(string const &ss) : s(ss.size()+2, -1), t(ss.size()+2) {
        t[IMAG].len = -1;
        t[IMAG].link = t[REAL].link = IMAG;
        for (char c : ss) addLetter(c-'a');
    }
 
    int getLink(int v) {
        while (s[n - t[v].len - 2] != s[n - 1])
            v = t[v].link;
        return v;
    }
 
    void addLetter(int c) {
        s[n++] = c;
        last = getLink(last);
 
        if (!t[last].to[c]) {
            t[sz] = {
                t[last].len+2,
                n-4-t[last].len,
                t[getLink(t[last].link)].to[c],
            };
            t[sz].depth = t[t[sz].link].depth+1;
            if (t[sz].len > lps)
                lps = t[sz].len, lpsPos = t[sz].start;
            t[last].to[c] = sz++;
        }
        last = t[last].to[c];
        t[last].cnt++;
        totalPalindromes += t[last].depth;
    }
};
