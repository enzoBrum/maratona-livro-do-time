#include <cmath>
#include <iostream>
#define ll long long
#define ull unsigned long long
/*
 * Your task is to calculate the number of trailing zeros in the factorial n!.
 * For example, 20!=2432902008176640000 and it has 4 trailing zeros.
 */

int main() {

  std::ios::sync_with_stdio(false);

  int n;
  std::cin >> n;

  int ans = 0;
  int num = 1;

  while (num < n) {
    num *= 5;
    ans += n / num;
  }

  std::cout << ans;
}
