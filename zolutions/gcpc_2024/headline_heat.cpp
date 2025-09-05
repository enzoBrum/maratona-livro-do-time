#include <bits/stdc++.h>

using namespace std;

//#define int long long
const int INF = 0x3F3F3F3F;
using ii = pair<int, int>;
using ll = long long;
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()

const bool DEBUG = 0;

/*
Exercise:
You are given 'n' university names and 'k' texts. There are 'm' pairs of rival universities.
You wanna see what texts generate chaos. Chaos is generated on a text iff, for two rival universities
'u' and 'v', the frequency of the name 'u' within the text is different than the frequency of 'v'.
n <= 1e5
m <= 1e5
k <= 1e5
Total university and text length is 1e6

Idea:
Build a suffix array of all texts separated with '#'. Then, for each university name, find its bounds
[l, r] on the suffix array.
If you wanted to count how frequently each university name appears on each text, you would have
to sum:
frequency[textThatOwnThisSuffix[sa[i]]][thisUniversity]++;
for each i E [l, r]
Since each interval [l, r] has size up to the whole sa array, that would be quadratic.

The deal is that those intervals aren't random.
Claim: every interval is either completely contained in another, or doesn't intersect at all.
Proof:
Say the strings are 's' and 'r'. If 's' and 'r' share a prefix, and, WLOG, |s| < |r|, then,
every place where 'r' appears, 's' appears. Then, the range of 'r' is contained within the range
of 's'.
If they don't share a prefix, then, clearly they won't be on the same range on the suffix array, QED.

This means the ranges will look something like this:
|--------------------|
    |----------------|                    |-----------------|
         |------|                   |------------------------------|
            |-|          |---------|

Now, we iterate through every rival pair of universities. If the range of one is contained into the
other, then, we know that the frequency of each university name on the texts that own the suffixes
of their intersection will be the same. Therefore, we don't need to process the intersection of the
intervals.
Also, we know that the remaining left and right ranges invalidate the texts that own those suffixes,
because they will only have the name of the university that owns the larger range.
All of that can be solved in linear time O(sum(|t|)) using the cumulative sum trick: if we wanna say that
the range of suffixes [l, r] is invalid, we do:
violations[l]++;
violations[r+1]--;
And then after we compute all the violations, we know a position violates something iff its prefix sum
is > 0.

After we process the intersecting ranges, we are left with up to 1e5 ranges that pairwise do not
intersect. The fact they don't intersect means their total length is up to sum(|t|), and we can bruteforce
where each university appears and invalidate that text.

Final complexity:
Let T be the sum of the text lengths.
Let U be the sum of the university name lengths.
O(TlogT) suffix array
O(UlogT) to get the range [l, r] of each university
O(T) cumulative sum trick
O(TlogT) by inserting at most once each of the T positions on a map
------
O((U+T)logT) total
*/

// sortCyclicShifts()
// getSuffixArray()
// search()

signed main() {
    cin.tie(0)->sync_with_stdio(0);

    int n, m, k;
    cin >> n >> m >> k;

    vector<string> names(n);
    cin.ignore(); // cin >> n >> m >> k left a '\n'
    for (string &s : names) getline(cin, s);

    vector<ii> collisions(m);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        u--; v--;
        collisions[i] = {u, v};
    }

    string texts;
    cin.ignore(); // cin >> u >> v left a '\n'
    for (int i = 0; i < k; i++) {
        string s; getline(cin, s);
        texts += s;
        if (i != k-1)
            texts += '#';
    }

    vector<int> sa = getSuffixArray(texts);

    // textIdx is the index of the text of the ith character of "texts" (nothing to do with the SA)
    vector<int> textIdx(texts.size());
    int curText = 0;
    for (int i = 0; i < texts.size(); i++) {
        if (texts[i] == '#') {
            textIdx[i] = k; // dummy value to indicate no text
            curText++;
        }
        else textIdx[i] = curText;
    }

    vector<ii> ranges(names.size());
    for (int i = 0; i < names.size(); i++)
        ranges[i] = search(sa, texts, names[i]);

    vector<int> saViolations(texts.size()+1, 0); // +1 because cumulative sum trick does v[r+1]
    vector<ii> nonIsecting; // to process later
    nonIsecting.reserve(m);

    for (auto[u, v] : collisions) {
        auto[lu, ru] = ranges[u];
        auto[lv, rv] = ranges[v];
        if (lu == -1 && lv == -1) continue; // neither appear on any text, no chaos
        if (lu == -1 || lv == -1) { // edge case where a full interval generates chaos
            int outterL = lu == -1 ? lv : lu;
            int outterR = ru == -1 ? rv : ru;
            saViolations[outterL]++;
            saViolations[outterR+1]--;
        }
        else if (lu > rv || lv > ru) // no intersection
            nonIsecting.push_back({u, v});
        else {
            int outterL = min(lu, lv);
            int innerL = max(lu, lv);
            int outterR = max(ru, rv);
            int innerR = min(ru, rv);

            // gotta be careful with edge case where they end on the same left position:
            // there would be no left range to disable
            if (lu != lv) { 
                saViolations[outterL]++;
                saViolations[innerL]--;
            }
            // same for right
            if (ru != rv) {
                saViolations[innerR+1]++;
                saViolations[outterR+1]--;
            }
        }
    }

    // now we are left only with pairwise non-intersecting universities
    // this vector counts in what texts each university appears
    vector<map<int, int>> nonIsectingFreq(names.size());

    vector<int> isValidText(k+1, 1); // k+1 because of dummy text == k

    for (auto[u, v] : nonIsecting) {
        if (!nonIsectingFreq[u].size()) { // bruteforcing where university 'u' appears
            auto[l, r] = ranges[u];
            for (int i = l; i <= r; i++)
                nonIsectingFreq[u][textIdx[sa[i]]]++;
        }
        if (!nonIsectingFreq[v].size()) { // bruteforcing where university 'v' appears
            auto[l, r] = ranges[v];
            for (int i = l; i <= r; i++)
                nonIsectingFreq[v][textIdx[sa[i]]]++;
        }

        std::map<int, int>::iterator itu = nonIsectingFreq[u].begin();
        std::map<int, int>::iterator itv = nonIsectingFreq[v].begin();

        // pair is {textIndex, count} for each university

        while (itu != nonIsectingFreq[u].end() && itv != nonIsectingFreq[v].end()) {
            if (itu->first == itv->first) {
                if (itu->second != itv->second)
                    isValidText[itu->first] = 0;
                // else, its fine because they have equal frequency
                itu++; itv++;
            }
            else if (itu->first < itv->first) { // text itu->first doesn't have university 'v'
                isValidText[itu->first] = 0;
                itu++;
            }
            else { // text itv->first doesn't have university 'u'
                isValidText[itv->first] = 0;
                itv++;
            }
        }

        while (itu != nonIsectingFreq[u].end()) { // all of these texts do not have university 'v'
            isValidText[itu->first] = 0;
            itu++;
        }
        while (itv != nonIsectingFreq[v].end()) { // all of these texts do not have university 'u'
            isValidText[itv->first] = 0;
            itv++;
        }
    }

    // cummulative sum trickery here
    int sum = 0;
    for (int i = 0; i < texts.size(); i++) {
        sum += saViolations[i];
        // text who owns sa[i] is invalid
        if (sum > 0) isValidText[textIdx[sa[i]]] = 0;
    }

    for (int i = 0; i < k; i++)
        cout << (isValidText[i] ? "yes" : "no") << '\n';
    
    return 0;
}