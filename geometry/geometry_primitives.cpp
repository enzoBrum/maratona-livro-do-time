const double PI = 4*atan(1.0);
const double EPS = 1e-9;


struct Point {
    int x, y;

    bool operator==(Point const &o) const {
        return x == o.x && y == o.y;
    }

    bool operator<(Point const &o) const {
        if (x == o.x) return y < o.y;
        return x < o.x;
    }
};

double dist(Point &a, Point &b) {
    return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

int dist2(Point &a, Point &b) {
    return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
}

int orientation(Point &a, Point &b, Point &c) {
    int o = a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y);
    if (o > 0) return 1; // ccw
    else if (o < 0) return -1; // cw
    return 0;
}

// Returns whether p lies over segment (a, b)
bool liesOver(Point &p, Point &a, Point &b) {
    return (
        orientation(p, a, b) == 0
        && p.x >= min(a.x, b.x)
        && p.x <= max(a.x, b.x)
        && p.y >= min(a.y, b.y)
        && p.y <= max(a.y, b.y)
    );
}

// dot product of vectors a1->a2 and b1->b2
int dot(Point &a1, Point &a2, Point &b1, Point &b2) {
    return (a2.x-a1.x)*(b2.x-b1.x) + (a2.y-a1.y)*(b2.y-b1.y);
}

double angle(Point &a, Point &o, Point &b) { // Angle aob in rad
    return acos(dot(o, a, o, b) / sqrt(1.0 * dot(o, a, o, a) * dot(o, b, o, b)));
}

bool perpendicular(Point &a1, Point &a2, Point &b1, Point &b2) {
    return dot(a1, a2, b1, b2) == 0;
}


struct Segment {
    Point p1, p2;
    Segment() {}
    Segment(Point p1, Point p2) : p1(p1), p2(p2) {}
    Segment(int x1, int y1, int x2, int y2) {
        p1 = {x1, y1};
        p2 = {x2, y2};
    }
};

bool overlaps(Segment &a, Point &p) {
    // supposing this segment is collinear with p
    return (
        p.x >= min(a.p1.x, a.p2.x) && p.x <= max(a.p1.x, a.p2.x)
        && p.y >= min(a.p1.y, a.p2.y) && p.y <= max(a.p1.y, a.p2.y)
    );
}

bool intersects(Segment &a, Segment &b) {
    int o1 = orientation(a.p1, a.p2, b.p1); 
    int o2 = orientation(a.p1, a.p2, b.p2); 
    int o3 = orientation(b.p1, b.p2, a.p1); 
    int o4 = orientation(b.p1, b.p2, a.p2); 

    if (o1 != o2 && o3 != o4)
        return true;
    
    if (o1 == 0 && overlaps(a, b.p1)) return true;
    if (o2 == 0 && overlaps(a, b.p2)) return true;
    if (o3 == 0 && overlaps(b, a.p1)) return true;
    if (o4 == 0 && overlaps(b, a.p2)) return true;
    return false;
}

double length(Segment &a) {
    return dist(a.p1, a.p2);
}


// Assumes the first point was NOT inserted into the end.
// WARNING: if Point is double, then area and return type should be doubles too.
int twicePolygonArea(vector<Point> &p) {
    int n = p.size();
    int area = 0;
    for (int i = 0; i < n-1; i++)
        area += p[i].x * p[i+1].y - p[i+1].x * p[i].y;
    area += p[n-1].x * p[0].y - p[0].x * p[n-1].y;
    return abs(area);
}

inline double toRadians(int deg) {
    return deg * PI / 180;
}

inline double toDegrees(int rad) {
    return rad * 180.0 / PI;
}

double circleSectorArea(double r, double theta) {
    return r*r*theta/2;
}

double triangleArea(double a, double b, double c) {
    double s = (a + b + c) / 2.0;
    return sqrt(s*(s-a)*(s-b)*(s-c));
}
