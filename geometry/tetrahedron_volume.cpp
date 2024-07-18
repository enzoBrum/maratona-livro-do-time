#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;
using pdd = pair<double, double>;
using ll = long long;
using ull = unsigned long long;

template <typename T> struct Point {
  T x, y, z;
  Point() : x{0}, y{0}, z{0} {}
  Point(T x, T y, T z) : x{x}, y{y}, z{z} {}

  T dot(Point<T> &other) { return x * other.x + y * other.y + z * other.z; }

  T cross(Point<T> &other) {
    return x * other.x + x * other.y + x * other.z + y * other.x + y * other.y +
           y * other.z + z * other.x + z * other.y + z * other.z;
  }

  friend istream &operator>>(istream &is, Point<T> &p) {
    is >> p.x >> p.y >> p.z;
    return is;
  }

  Point<T> operator-(Point<T> &other) {
    return Point<T>(x - other.x, y - other.y, z - other.z);
  }
};

double volume(Point<double> &p1, Point<double> &p2, Point<double> &p3,
              Point<double> &p4) {
  auto pa = p1 - p4, pb = p2 - p4, pc = p3 - p4;

  double determinant = pa.x * (pb.y * pc.z - pc.y * pb.z) -
                       pb.x * (pa.y * pc.z - pc.y * pa.z) +
                       pc.x * (pa.y * pb.z - pb.y * pa.z);
  return abs(determinant) / 6.0;
}

int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);

  int t;
  cin >> t;

  while (t--) {
    Point<double> p1, p2, p3, p4;
    cin >> p1 >> p2 >> p3 >> p4;
    cout << fixed << setprecision(6) << volume(p1, p2, p3, p4) << '\n';
  }
}
