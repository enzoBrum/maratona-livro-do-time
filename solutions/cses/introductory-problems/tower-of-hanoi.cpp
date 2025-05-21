#include <cmath>
#include <iostream>

#define ll long long
#define ull uint64_t

/*
The Tower of Hanoi game consists of three stacks (left, middle and right) and n
round disks of different sizes. Initially, the left stack has all the disks, in
increasing order of size from top to bottom. The goal is to move all the disks
to the right stack using the middle stack. On each move you can move the
uppermost disk from a stack to another stack. In addition, it is not allowed to
place a larger disk on a smaller disk. Your task is to find a solution that
minimizes the number of moves.
*/

void print_move(int start, int end) {
  std::cout << start << ' ' << end << '\n';
}

void mv(int n, int start, int end) {
  if (n == 1) {
    print_move(start, end);
    return;
  }

  int mid = 6 - start - end;
  mv(n - 1, start, mid);
  mv(1, start, end);
  mv(n - 1, mid, end);
}

int main() {
  std::ios::sync_with_stdio(0);

  int n;
  std::cin >> n;
  std::cout << (ll)pow(2, n) - 1 << '\n';
  mv(n, 1, 3);
}
