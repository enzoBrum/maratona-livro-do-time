struct Point {
    int x, y;

    bool operator== (Point const &o) const {
        return x == o.x && y == o.y;
    }

    bool operator< (Point const &o) const {
        if (x == o.x)
            return y < o.y;
        return x < o.x;
    }

    double dist(Point const &o) {
        return sqrt((x-o.x)*(x-o.x) + (y-o.y)*(y-o.y));
    }

    int dist2(Point const &o) {
        return (x-o.x)*(x-o.x) + (y-o.y)*(y-o.y);
    }
};

struct Vec {
    int x, y;

    Vec() {}
    Vec(Point &a) : x(a.x), y(a.y) {}
    Vec(Point &a, Point &b) {
        x = b.x - a.x;
        y = b.y - a.y;
    }

    int dot(Vec const &o) {
        return x*o.x + y*o.y;
    }

    int cross(Vec const &o) {
        return y*o.x - x*o.y;
    }

    bool perpendicular(Vec const &o) {
        return dot(o) == 0;
    }

    bool parallel(Vec const &o) {
        return cross(o) == 0;
    }
};

int orientation(Point &p1, Point &p2, Point &p3) {
    int x1 = p2.x - p1.x;
    int y1 = p2.y - p1.y;
    int x2 = p3.x - p1.x;
    int y2 = p3.y - p1.y;
    int res = y1*x2 - x1*y2;
    if (res < 0) return -1;
    else if (res > 0) return 1;
    return 0;
}

struct Line {
    double a, b, c; // coefficients of ax + by + c = 0;

    Line() {}
    Line(Point &p1, Point &p2) {
        if (p1.x == p2.x) { // vertical
            a = 1;
            b = 0;
            c = -p1.x;
        }
        else {
            a = -1.0 * (p1.y - p2.y) / (p1.x - p2.x);
            b = 1;
            c = -1.0 * (a * p1.x) - p1.y;
        }
    }

    bool operator== (Line const &o) {
        return a == o.a && b == o.b && c == o.c;
    }
};

bool parallel(Line &l1, Line &l2) {
    return l1.a == l2.a && l1.b == l2.b;
}

bool intersection(Line &l1, Line &l2, Point &out) {
    if (parallel(l1, l2)) return false;
    out.x = (l2.b * l1.c - l1.b * l2.c) / (l2.a * l1.b - l1.a * l2.b);
    if (l1.b == 0) out.y = -(l2.a * out.x + l2.c); // vertical
    else out.y = -(l1.a * out.x + l1.c);
    return true;
}

struct Segment {
    Point p1, p2;
    Segment() {}
    Segment(Point &p1, Point &p2) : p1(p1), p2(p2) {}
    Segment(int x1, int y1, int x2, int y2) {
        p1 = {x1, y1};
        p2 = {x2, y2};
    }

    bool overlaps(Point &p) {
        // supposing this segment is collinear with p
        return (
            p.x >= min(p1.x, p2.x) && p.x <= max(p1.x, p2.x)
            && p.y >= min(p1.y, p2.y) && p.y <= max(p1.y, p2.y)
        );
    }

    bool intersects(Segment &o) {
        int o1 = orientation(p1, p2, o.p1); 
        int o2 = orientation(p1, p2, o.p2); 
        int o3 = orientation(o.p1, o.p2, p1); 
        int o4 = orientation(o.p1, o.p2, p2); 

        if (o1 != o2 && o3 != o4)
            return true;
        
        if (o1 == 0 && overlaps(o.p1)) return true;
        if (o2 == 0 && overlaps(o.p2)) return true;
        if (o3 == 0 && o.overlaps(p1)) return true;
        if (o4 == 0 && o.overlaps(p2)) return true;
        return false;
    }

    double length() {
        return p1.dist(p2);
    }
};
