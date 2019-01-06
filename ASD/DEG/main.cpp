#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <queue>

using namespace std;

constexpr int MAX_N = 500005;

struct vertex {
    int id, children_size;

    bool operator<(const vertex& lhs) const {
        if (lhs.children_size == children_size) {
            return lhs.id > id;
        }

        return lhs.children_size > children_size;
    }
};

set<int> G[MAX_N];

int main() {
    int n, m;
    cin >> n >> m;

    set<vertex> q;

    while (m--) {
        int a, b;
        cin >> a >> b;

        G[a].emplace(b);
        G[b].emplace(a);
    }

    for (int i = 1; i <= n; ++i) {
        q.insert({i, (int) G[i].size()});
    }

    int ans = 0;
    while (!q.empty()) {
        auto top = q.begin();
        ans = max(ans, top->children_size);

        for (const int v: G[top->id]) {
            q.erase({v, (int) G[v].size()});
            G[v].erase(top->id);
            q.insert({v, (int) G[v].size()});
        }

        q.erase(top);
    }

    cout << ans << endl;
}