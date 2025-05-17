const long double BOX_INF = 1e9;

#warning Halfplane keeps its LEFT side
struct Halfplane { 
    // Line that goes through 'p' and with direction vector 'pq'
    FPoint p, pq; 
    long double angle;

    Halfplane() {}
    Halfplane(FPoint &a, FPoint &b) : p(a), pq(b - a) {
        angle = atan2l(pq.y, pq.x);    
    }

    // returns whether 'r' is outside this halfplane
    bool out(FPoint const &r) { 
        return GF::cross(pq, r - p) < -EPS; 
    }

    bool operator<(Halfplane const &o) const { 
        return angle < o.angle;
    }
};

FPoint inter(Halfplane const &s, Halfplane const &t) {
    long double alpha = GF::cross((t.p - s.p), t.pq) / GF::cross(s.pq, t.pq);
    return s.p + (s.pq * alpha);
}

vector<FPoint> halfplaneIntersection(vector<Halfplane>& H) { 
    FPoint box[4] = {  // Bounding box in CCW order
        {BOX_INF, BOX_INF}, 
        {-BOX_INF, BOX_INF},
        {-BOX_INF, -BOX_INF}, 
        {BOX_INF, -BOX_INF}
    };

    for (int i = 0; i < 4; i++) {
        Halfplane aux(box[i], box[(i+1)%4]);
        H.push_back(aux);
    };

    sort(all(H)); // sorted by atan2 angle, that is, just below x axis on the left and then full ccw turnaround

    deque<Halfplane> dq;
    int len = 0;
    for (int i = 0; i < H.size(); i++) {
        while (len > 1 && H[i].out(inter(dq[len-1], dq[len-2]))) {
            dq.pop_back();
            --len;
        }
        while (len > 1 && H[i].out(inter(dq[0], dq[1]))) {
            dq.pop_front();
            --len;
        }

        // Special case check: Parallel half-planes
        if (len > 0 && fabsl(GF::cross(H[i].pq, dq[len-1].pq)) < EPS) {
            // Opposite parallel half-planes that ended up checked against each other.
            if (GF::dot(H[i].pq, dq[len-1].pq) < 0.0)
                return vector<FPoint>();

            // Same direction half-plane: keep only the leftmost half-plane.
            if (H[i].out(dq[len-1].p)) {
                dq.pop_back();
                --len;
            }
            else continue;
        }
        dq.push_back(H[i]);
        ++len;
    }

    // Final cleanup: Check half-planes at the front against the back and vice-versa
    while (len > 2 && dq[0].out(inter(dq[len-1], dq[len-2]))) {
        dq.pop_back();
        --len;
    }

    while (len > 2 && dq[len-1].out(inter(dq[0], dq[1]))) {
        dq.pop_front();
        --len;
    }

    // Report empty intersection if necessary
    if (len < 3) return vector<FPoint>();

    // Reconstruct the convex polygon from the remaining half-planes.
    vector<FPoint> ret(len);
    for(int i = 0; i+1 < len; i++)
        ret[i] = inter(dq[i], dq[i+1]);
    ret.back() = inter(dq[len-1], dq[0]);
    return ret;
}
