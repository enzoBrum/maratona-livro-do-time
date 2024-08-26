struct Point {
    double x, y;
};

class MinEnclosingCircle {
    double distance(const Point &a, const Point &b) {
        return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    }
    
    bool isInsideCircle(const Point &p, const Point &c, double r) {
        return distance(p, c) <= r + 1e-9;
    }
    
    Point circleCenter(const Point &a, const Point &b, const Point &c) {
        double ax = a.x, ay = a.y;
        double bx = b.x, by = b.y;
        double cx = c.x, cy = c.y;
    
        double d = 2 * (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by));
        double ux = ((ax * ax + ay * ay) * (by - cy) + (bx * bx + by * by) * (cy - ay) + (cx * cx + cy * cy) * (ay - by)) / d;
        double uy = ((ax * ax + ay * ay) * (cx - bx) + (bx * bx + by * by) * (ax - cx) + (cx * cx + cy * cy) * (bx - ax)) / d;
    
        return {ux, uy};
    }
    
    pair<Point, double> circleFromThreePoints(const Point &a, const Point &b, const Point &c) {
        Point center = circleCenter(a, b, c);
        double radius = distance(center, a);
        return {center, radius};
    }
    
    pair<Point, double> circleFromTwoPoints(const Point &a, const Point &b) {
        Point center = {(a.x + b.x) / 2, (a.y + b.y) / 2};
        double radius = distance(a, b) / 2;
        return {center, radius};
    }

public:
    pair<Point, double> mec(vector<Point> &points, vector<Point> boundary = {}) {
        if (points.empty() || boundary.size() == 3) {
            if (boundary.size() == 0) {
                return {{0, 0}, 0}; // No points
            }
            if (boundary.size() == 1) {
                return {boundary[0], 0}; // Single point
            }
            if (boundary.size() == 2) {
                return circleFromTwoPoints(boundary[0], boundary[1]);
            }
            return circleFromThreePoints(boundary[0], boundary[1], boundary[2]);
        }
    
        // Randomly pick a point
        srand(time(0));
        random_shuffle(points.begin(), points.end());
    
        Point p = points.back();
        points.pop_back();
    
        auto circle = mec(points, boundary);
    
        if (isInsideCircle(p, circle.first, circle.second)) {
            points.push_back(p);
            return circle;
        }
    
        boundary.push_back(p);
        auto result = mec(points, boundary);
        boundary.pop_back();
        points.push_back(p);
        return result;
    }
};
