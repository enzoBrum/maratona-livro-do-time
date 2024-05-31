#include <bits/stdc++.h>

using namespace std;

struct Point {
  double x, y;
  Point(double x, double y) : x{x}, y{y} {}
};

inline double distance(const Point &p1, const Point &p2) {
  double x = p1.x - p2.x, y = p1.y - p2.y;
  return sqrt(x * x + y * y);
}

// Returns point equidistant to all vertices of the triangle
Point circuncenter(Point &A, Point &B, Point &C) {

  // LINE AB
  // ax + by = c
  double a = B.y - A.y, b = A.x - B.x;
  double c = a * A.x + b * A.y;

  // LINE BC
  double e = C.y - B.y, f = B.x - C.x;
  double g = e * B.x + f * B.y;

  // convert AB to perpendicular bisector
  c = -b * (A.x + B.x) / 2 + a * (A.y + B.y) / 2;
  b = exchange(a, -b);

  // convert BC to perpendicular bisector
  g = -f * (B.x + C.x) / 2 + e * (B.y + C.y) / 2;
  f = exchange(e, -f);

  double determinant = a * f - e * b;
  if (determinant == 0)
    return Point(1e9, 1e9);
  return Point((f * c - b * g) / determinant, (a * g - e * c) / determinant);
}
