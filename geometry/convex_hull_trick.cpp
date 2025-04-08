using ftype = long double;
using Point = complex<ftype>;
#define x real
#define y imag

// On CHT, dot will measure how aligned vectors are. dot(perpendiculars) = 0, dot(90 < theta < 270) is 
// negative (because they don't even align). While dot(perpendicular of edge, new point) is negative,
// it means that there is a cw turn (so remove the previous point).
ftype dot(Point a, Point b) {
    // a.x * b.x + a.y * b.y
    return (conj(a) * b).x();
}

ftype cross(Point a, Point b) {
    // a.x * b.y - a.y * b.x, the same as orientation of geometry primitives
    // computes orientation of (0, 0) -> a -> b, the same as considering 'a'
    // and 'b' as vectors
    return (conj(a) * b).y();
}

struct CHT {
    const bool isMin;
    vector<Point> hull, vecs;
    CHT(vector<ii> lines, bool isMin) : isMin(isMin) {
        // having more than one 'k' (lines[i].first) is useless. So, if you want a min CHT, just
        // keep the ones with min 'b'. Else, keep the ones with max 'b'.
        if (isMin) sort(all(lines));
        else sort(rall(lines));

        lines.erase(
            unique(all(lines), [](ii a, ii b) {return a.first == b.first;}),
            lines.end()
        );

        int m = isMin ? 1 : -1;
        for (auto[k, b] : lines)
            addLine(k*m, b*m);
    }

    void addLine(ftype k, ftype b) {
        Point nw = {k, b};
        // nw - hull.back() is the vector from the last hull point to the new point
        while (!vecs.empty() && dot(vecs.back(), nw - hull.back()) < 0) {
            hull.pop_back();
            vecs.pop_back();
        }
        // Why 1il * lastVector?
        // last     = (a + bi)
        // 1il*last = (-b + ai)
        // and it is well known that this transformation rotates 90 deg ccw
        if (!hull.empty())
            vecs.push_back(1il * (nw - hull.back()));
        hull.push_back(nw);
    }

    ftype query(ftype x) {
        Point query = {x, 1};
        // find the first vec that is directed ccw from the query. Conveniently, its left
        // point is the desired line. Also conveniently, the index of that point is the same
        // as the index of the found vec.
        auto it = lower_bound(vecs.begin(), vecs.end(), query, [](Point a, Point b) {
            // dont forget this is like the operator<. This will return the first vec
            // such that NOT cross(a, query) > 0
            return cross(a, b) > 0;
        });
        ftype val = dot(query, hull[it - vecs.begin()]);
        if (isMin) return val;
        return -val;
    }
};
