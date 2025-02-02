// uses Point

bool ccw(Point &a, Point &b, Point &c, bool include_collinear) {
    int o = orientation(a, b, c);
    return o > 0 || (include_collinear && o == 0);
}

void convex_hull(vector<Point> &a, bool include_collinear = false) {
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

void convex_hull_with_repetitions(vector<Point> &a, bool include_collinear = false) {
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
