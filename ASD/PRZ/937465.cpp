#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct interval {
    int s, e;

    bool operator<(const interval& rhs) const {
        if (s == rhs.s) {
            return e < rhs.e;
        }

        return s < rhs.s;
    }
};


int main() {
    ios_base::sync_with_stdio(false);

    int n;
    cin >> n;

    vector<interval> intervals;

    while (n--) {
        int a, b;
        cin >> a >> b;

        intervals.push_back({a, b});
    }

    sort(intervals.begin(), intervals.end());
    int ans = INT_MAX;

    for (int i = 1; i < intervals.size(); ++i) {
        if (intervals[i].s <= intervals[i - 1].e) {
            ans = 0;
        } else {
            ans = min(ans, intervals[i].s - intervals[i - 1].e);
        }

    }

    cout << ans << endl;
}