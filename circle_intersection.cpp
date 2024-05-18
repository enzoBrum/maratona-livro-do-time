struct Circle {
  int x, y, r;
};

inline bool is_inside(Circle &a, Circle &b) {
  double d = sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));

  return d <= a.r + b.r || d <= a.r - b.r || d <= b.r - a.r;
}
