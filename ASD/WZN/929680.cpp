#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <set>
#include <algorithm>

using namespace std;
const int MAX_N = 500005;
using ull = unsigned long long;
ull answer = 0;

struct edge {
    int from, to, weight;

    bool operator<(const edge& e) const {
        if (e.weight == weight) {
            return e.to < to;
        }

        return e.weight > weight;
    }
};

vector<edge> G[MAX_N];

void two_bfs(int starter) {
    auto it2 = G[starter].begin();
    for (auto it = G[starter].begin(); it != G[starter].end(); ++it) {
        while (it2 != G[starter].end() && (*it).weight == (*it2).weight) {it2++;}
        answer += distance(it2, G[starter].end());
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    cin >> n >> m;

    while (m--) {
        int from, to, weight;
        cin >> from >> to >> weight;

        G[from].push_back({from, to, weight});
        G[to].push_back({to, from, weight});
    }

    for (int i = 1; i <= n; ++i) {
        sort(G[i].begin(), G[i].end());
        two_bfs(i);
    }

    cout << answer << endl;
}