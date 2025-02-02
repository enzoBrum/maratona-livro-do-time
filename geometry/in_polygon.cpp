const int BOUNDARY = -1;
const int OUTSIDE = 0;
const int INSIDE = 1;

int inPolygon(Point &p, vector<Point> &poly) {
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
