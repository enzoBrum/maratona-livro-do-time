/*
  *
  *
Given an array of n integers, your task is to find for each array position the
nearest position to its left having a smaller value.
  */
using namespace std;

int main() {
  int n;
  scanf("%d ", &n);
  stack<pii> st;

  for (int i = 0; i < n; ++i) {
    int num;
    scanf("%d ", &num);

    while (!st.empty() && st.top().first >= num)
      st.pop();

    if (st.empty())
      printf("%d ", 0);
    else
      printf("%d ", st.top().second);

    st.push({num, i + 1});
  }
}
