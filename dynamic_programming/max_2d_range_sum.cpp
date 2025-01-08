/*
 *
 * Given N x N matrix, find submatrix with greatest sum.
 * Complexity: O(n^3)
 */

int main() {
  int n;
  scanf("%d", &n);
  // square matrix size
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j) {
      scanf("%d", &A[i][j]);
      if (j > 0)
        A[i][j] += A[i][j - 1];
    }
  int maxSubRect = -127 * 100 * 100;
  // lowest possible val
  for (int l = 0; l < n; ++l)
    for (int r = l; r < n; ++r) {
      int subRect = 0;
      for (int row = 0; row < n; ++row) {
        // Max 1D Range Sum on columns of this row
        if (l > 0)
          subRect += A[row][r] - A[row][l - 1];
        else
          subRect += A[row][r];

        // Kadane’s algorithm on rows
        if (subRect < 0)
          subRect = 0;
        // restart if negative
        maxSubRect = max(maxSubRect, subRect);
      }
    }
}
