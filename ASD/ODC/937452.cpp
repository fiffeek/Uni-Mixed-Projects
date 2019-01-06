#include <iostream>
#include <set>
#include <algorithm>
#include <vector>
#include <unordered_map>

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
    std::ios_base::sync_with_stdio(false);

    int n;
    cin >> n;

    long long ans = 0;

    unordered_map<int, vector<interval>> S;
    set<int> check;

    while (n--) {
        int x, y1, y2;
        cin >> x >> y1 >> y2;

        S[x].push_back({y1, y2});
        check.insert(x);
    }

    for (const int number: check) {
        sort(S.at(number).begin(), S.at(number).end());
    }

    for (const int number: check) {
        for (auto actual = S.at(number).begin(); actual != S.at(number).end(); ++actual) {
            auto it = lower_bound(S[number].begin(), S[number].end(), interval{(*actual).e, 0});
            long long aux = (it - actual) - 1;

            ans += aux;
        }
    }

    cout << ans << endl;
}

