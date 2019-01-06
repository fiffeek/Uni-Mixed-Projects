#include <iostream>
#include <vector>
#include <set>

using namespace std;

struct edge {
    int from, to;
};

constexpr int MAX_N = 300005;
vector<edge> G[MAX_N];
set<int> unvisited;
set<int> started;

void dfs(int curr) {
    started.insert(curr);
    unvisited.erase(curr);

    for (const edge& e: G[curr]) {
        auto it = started.lower_bound(e.from);

        if (it != started.end() && *it <= e.to) {
            cout << "TAK" << endl;
            exit(0);
        }

        it = unvisited.lower_bound(e.from);

        while (it != unvisited.end()) {
            if (*it > e.to) {
                break;
            }

            dfs(*it);
            it = unvisited.lower_bound(e.from);
        }
    }

    started.erase(curr);
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int n, m;
    cin >> n >> m;

    for (int i = 1; i <= n; ++i) {
        unvisited.insert(i);
    }

    while (m--) {
        int a, b, c;
        cin >> a >> b >> c;

        G[a].push_back({b, c});
    }

    for (int i = 1; i <= n; ++i) {
        if (unvisited.find(i) != unvisited.end()) {
            dfs(i);
        }
    }

    cout << "NIE" << endl;
    return 0;
}