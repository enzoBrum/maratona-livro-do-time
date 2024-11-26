/*
Returns the tallest tower that can be made by stacking any amount of boxes (even repeating some)
as long as the base of a box above another is strictly smaller in both dimensions.
*/

struct Box {
    int h, w, d;
    bool operator< (Box& o) const {
        return d*w > o.d*o.w; // sort by decreasing area
    }

    bool fitsUnder(Box& o) const {
        return w > o.w && d > o.d;
    }
};

int maxStackHeight(vector<Box>& boxes) {
    int n = boxes.size();
    vector<Box> rot(3*n);

    int index = 0;
    for (int i = 0; i < n; i++) {
        rot[3*i] = {
            boxes[i].h,
            max(boxes[i].d, boxes[i].w),
            min(boxes[i].d, boxes[i].w)
        };

        rot[3*i+1] = {
            boxes[i].w,
            max(boxes[i].h, boxes[i].d),
            min(boxes[i].h, boxes[i].d)
        };

        rot[3*i+2] = {
            boxes[i].d,
            max(boxes[i].h, boxes[i].w),
            min(boxes[i].h, boxes[i].w)
        };
    }

    n = 3*n;

    sort(all(rot));

    vector<int> dp(n, 0); // maximum stack height with ith box on top
    for (int i = 0; i < n; i++ )
        dp[i] = rot[i].h;

    for (int i = 1; i < n; i++) 
        for (int j = 0; j < i; j++)
            if (rot[j].fitsUnder(rot[i]))
                dp[i] = max(dp[i], dp[j] + rot[i].h);

    return *max_element(all(dp));
}
