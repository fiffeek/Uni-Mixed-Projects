#include <iostream>
#include <map>
#include <climits>

using namespace std;

struct edge {
    int from, to;

    bool operator<(const edge& lhs) const {
        if (from == lhs.from) {
            return lhs.to < to;
        }

        return lhs.from < from;
    }
};

map<edge, int> graph;
constexpr int MAX_N = 100005;

int main() {
    ios_base::sync_with_stdio(false);
    int n, m;
    cin >> n >> m;

    while (m--) {
        int from, to, cost;
        cin >> from >> to >> cost;
        graph.insert({edge{min(from, to), max(from, to)}, cost});
    }

    if (n & 1) {
        cout << -1 << endl;
        return 0;
    }

    int dp[MAX_N];

    dp[0] = 0;
    dp[2] = graph.find(edge{1, 2}) != graph.end() ? graph.find(edge{1, 2})->second : -1;

    for (int i = 4; i <= n; i += 2) {
        int checker = INT_MAX;
        auto find1 = graph.find(edge{i - 1, i});
        auto find2 = graph.find(edge{i - 2, i});
        auto find3 = graph.find(edge{i - 3, i - 1});

        if (dp[i - 2] != -1 && find1 != graph.end()) {
            checker = find1->second + dp[i - 2];
        }

        if (dp[i - 4] != -1 && find2 != graph.end() && find3 != graph.end()) {
            checker = min(checker, dp[i - 4] + find2->second + find3->second);
        }

        dp[i] = checker == INT_MAX ? -1 : checker;
    }

    cout << dp[n] << endl;
}