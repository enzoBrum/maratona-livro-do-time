// uses Point

// WARNING: requires Point and orientation to use doubles.
// Returns intersection between segment p-q and line A-B.
Point lineIntersectSeg(Point p, Point q, Point A, Point B) {
    double a = B.y-A.y, b = A.x-B.x, c = B.x*A.y - A.x*B.y;
    double u = fabs(a*p.x + b*p.y + c);
    double v = fabs(a*q.x + b*q.y + c);
    return {(p.x*v + q.x*u) / (u+v), (p.y*v + q.y*u) / (u+v)};
}

// Returns the polygon at the left of the line formed by the line A-B.
// Polygon should have its last point equal to the first one.
vector<Point> cutPolygon(Point A, Point B, vector<Point> &Q) {
    vector<Point> P;
    for (int i = 0; i < Q.size(); ++i) {
        double o1 = orientation(A, B, Q[i]);
        double o2 = 0;
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