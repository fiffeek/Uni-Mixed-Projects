#include <iostream>
#include <climits>
#include <queue>
#include <set>

using namespace std;

constexpr int MAX_N = 500005;
using ull = unsigned long long;
ull cost[MAX_N];
vector<int> G[MAX_N];

struct q_p {
    int id;
    ull cost;

    bool operator<(const q_p& lhs) const {
        if (cost == lhs.cost) {
            return lhs.id < id;
        }

        return cost < lhs.cost;
    }
};

int main() {
    ios_base::sync_with_stdio(false);

    int n;
    cin >> n;
    set<q_p> q;

    for (int i = 1; i <= n; ++i) {
        cin >> cost[i];
        q.insert({i, cost[i]});
    }

    int m;
    cin >> m;

    ull min_global = ULLONG_MAX;

    while (m--) {
        int a, b, c;
        cin >> a >> b >> c;

        min_global = min(min_global, cost[a] + cost[b] + 2 * c);

        G[a].push_back(b);
        G[b].push_back(a);
    }

    for (int i = 1; i <= n; ++i) {
        q.erase({i, cost[i]});

        for (const int temp: G[i]) {
            q.erase({temp, cost[temp]});
        }

        if (!q.empty()) {
            auto f = q.begin();
            min_global = min(min_global, cost[i] + f->cost);
        }

        for (const int temp: G[i]) {
            q.insert({temp, cost[temp]});
        }

        q.insert({i, cost[i]});
    }

    cout << min_global << endl;
}