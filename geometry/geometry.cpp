const long double PI = 4*atanl(1.0);
const long double EPS = 1e-9;

template <typename T>
struct Geometry {
    struct Point {
        T x, y;
    
        bool operator==(Point const &o) const {
            return x == o.x && y == o.y;
        }
    
        bool operator<(Point const &o) const {
            if (x == o.x) return y < o.y;
            return x < o.x;
        }

        Point operator+(Point const &o) const {
            return {x+o.x, y+o.y}; 
        }

        Point operator-(Point const &o) const {
            return {x-o.x, y-o.y}; 
        }
    
        Point operator*(T k) const {
            return {x*k, y*k}; 
        }
    };
    
    static long double dist(Point &a, Point &b) {
        return sqrtl((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
    }
    
    static T dist2(Point &a, Point &b) {
        return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
    }
    
    static int orientation(Point &a, Point &b, Point &c) {
        T o = a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y);
        if (o > 0) return 1; // ccw
        else if (o < 0) return -1; // cw
        return 0;
    }
    
    // Returns whether p lies over segment (a, b)
    static bool liesOver(Point &p, Point &a, Point &b) {
        return (
            orientation(p, a, b) == 0
            && p.x >= min(a.x, b.x)
            && p.x <= max(a.x, b.x)
            && p.y >= min(a.y, b.y)
            && p.y <= max(a.y, b.y)
        );
    }
    
    // dot product of vectors a1->a2 and b1->b2
    static T dot(Point &a1, Point &a2, Point &b1, Point &b2) {
        return (a2.x-a1.x)*(b2.x-b1.x) + (a2.y-a1.y)*(b2.y-b1.y);
    }

    // dot product of vectors a and b
    static T dot(Point &a, Point &b) {
        return a.x * b.x + a.y * b.y;
    }

    // cross product of vectors a and b
    static T cross(Point const &a, Point const &b) { 
        return a.x * b.y - a.y * b.x; 
    }
    
    static double angle(Point &a, Point &o, Point &b) { // Angle aob in rad
        return acos(dot(o, a, o, b) / sqrt(1.0 * dot(o, a, o, a) * dot(o, b, o, b)));
    }
    
    static bool perpendicular(Point &a1, Point &a2, Point &b1, Point &b2) {
        return dot(a1, a2, b1, b2) == 0;
    }
    
    struct Segment {
        Point p1, p2;
        Segment() {}
        Segment(Point p1, Point p2) : p1(p1), p2(p2) {}
        Segment(T x1, T y1, T x2, T y2) {
            p1 = {x1, y1};
            p2 = {x2, y2};
        }
    };
    
    static bool overlaps(Segment &a, Point &p) {
        // supposing this segment is collinear with p
        return (
            p.x >= min(a.p1.x, a.p2.x) && p.x <= max(a.p1.x, a.p2.x)
            && p.y >= min(a.p1.y, a.p2.y) && p.y <= max(a.p1.y, a.p2.y)
        );
    }
    
    static bool intersects(Segment &a, Segment &b) {
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
    
    #warning twicePolygonArea: Polygon should NOT wrap itself.
    static T twicePolygonArea(vector<Point> &p) {
        int n = p.size();
        T area = 0;
        for (int i = 0; i < n-1; i++)
            area += p[i].x * p[i+1].y - p[i+1].x * p[i].y;
        area += p[n-1].x * p[0].y - p[0].x * p[n-1].y;
        return abs(area);
    }
    
    static inline double toRadians(double deg) {
        return deg * PI / 180;
    }
    
    static inline double toDegrees(double rad) {
        return rad * 180 / PI;
    }
    
    static double circleSectorArea(double r, double theta) {
        return r*r*theta/2;
    }
    
    static double triangleArea(double a, double b, double c) {
        double s = (a + b + c) / 2.0;
        return sqrt(s*(s-a)*(s-b)*(s-c));
    }

    // Returns intersection between segment p-q and line A-B.
    static Point lineIntersectSeg(Point p, Point q, Point A, Point B) {
        double a = B.y-A.y, b = A.x-B.x, c = B.x*A.y - A.x*B.y;
        double u = fabs(a*p.x + b*p.y + c);
        double v = fabs(a*q.x + b*q.y + c);
        return {(p.x*v + q.x*u) / (u+v), (p.y*v + q.y*u) / (u+v)};
    }

    // Returns the polygon at the left of the line formed by the line A-B.
    #warning cutPolygon: Polygon should wrap (poly[0] == poly.back()).
    static vector<Point> cutPolygon(Point A, Point B, vector<Point> &Q) {
        vector<Point> P;
        for (int i = 0; i < Q.size(); ++i) {
            int o1 = orientation(A, B, Q[i]);
            int o2 = 0;
            if (i != Q.size()-1)
                o2 = orientation(A, B, Q[i+1]);
            if (o1 > -EPS)
                P.push_back(Q[i]);
            if (o1*o2 < -EPS) // crosses line AB
                P.push_back(lineIntersectSeg(Q[i], Q[i+1], A, B));
        }
        if (!P.empty() && !(P.back() == P.front()))
            P.push_back(P.front());
        return P;
    }

    static bool ccw(Point &a, Point &b, Point &c, bool include_collinear) {
        int o = orientation(a, b, c);
        return o > 0 || (include_collinear && o == 0);
    }

    static void convex_hull(vector<Point> &a, bool include_collinear = false) {
        if (!a.size()) return;
        Point p0 = *min_element(a.begin(), a.end());

        sort(a.begin(), a.end(), [&p0](Point &a, Point &b) {
            int o = orientation(p0, a, b);
            if (o == 0) return dist2(p0, a) < dist2(p0, b);
            return o > 0;
        });

        if (include_collinear) {
            int i = a.size()-1;
            while (i >= 0 && orientation(p0, a[i], a.back()) == 0) i--;
            reverse(a.begin()+i+1, a.end());
        }

        vector<Point> st;
        for (int i = 0; i < a.size(); i++) {
            while (st.size() >= 2 && !ccw(st[st.size()-2], st.back(), a[i], include_collinear))
                st.pop_back();
            st.push_back(a[i]);
        }

        if (!include_collinear && st.size() == 2 && st[0] == st[1])
            st.pop_back();

        a = st;
    }

    static void convex_hull_with_repetitions(vector<Point> &a, bool include_collinear = false) {
        if (!include_collinear) {
            convex_hull(a);
            return;
        }

        map<Point, int> freq;
        vector<Point> noReps;
        for (Point &p : a) {
            freq[p]++;
            if (freq[p] == 1)
                noReps.push_back(p);
        }
        convex_hull(noReps, true);
        a.clear();
        for (Point &p : noReps)
            while (freq[p]--)
                a.push_back(p);
    }

    static const int BOUNDARY = -1;
    static const int OUTSIDE = 0;
    static const int INSIDE = 1;
    
    static int inPolygon(Point &p, vector<Point> &poly) {
        if (poly.size() < 3) return false;
        poly.push_back(poly[0]);
    
        double sum = 0;
        for (int i = 0; i < poly.size()-1; i++) {
            if (liesOver(p, poly[i], poly[i+1])) {
                poly.pop_back();
                return BOUNDARY;
            }
            if (orientation(p, poly[i], poly[i+1]) > 0)
                sum += angle(poly[i], p, poly[i+1]);
            else sum -= angle(poly[i], p, poly[i+1]); 
        }
    
        poly.pop_back();
        if ((fabs(sum)) > PI) return INSIDE;
        return OUTSIDE;
    }

    #warning Line: only tested linePointDistance
    struct Line {
        // ax + by + c = 0
        // b == 0 means vertical
        double a, b, c;
    
        Line() {}
        Line(double slope, double intercept) {
            a = -slope;
            b = 1;
            c = -intercept;
        }
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

    static bool isVertical(Line &l) {
        return l.b == 0;
    }

    static double getSlope(Line &l) {
        assert(!isVertical(l));
        return -l.a;
    }

    static double getIntercept(Line &l) {
        assert(!isVertical(l));
        return -l.c;
    }

    static Line getPerpendicularAtPoint(Line &l, Point &p) {
        double pslope = -1 / getSlope(l);
        double pintercept = p.y - pslope * p.x;
        return Line(pslope, pintercept);
    }

    static long double linePointDistance(Line &l, Point &p) {
        return abs(l.a * p.x + l.b * p.y + l.c) / sqrtl(l.a * l.a + l.b * l.b);
    }
};

using GI = Geometry<int>;
using GF = Geometry<long double>;
using IPoint = GI::Point;
using FPoint = GF::Point;
