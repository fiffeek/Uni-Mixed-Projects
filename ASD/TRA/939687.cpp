#include <iostream>
#include <vector>
#include <climits>
#include <queue>

using namespace std;

struct edge {
    int to, cost;

    bool operator<(const edge& lhs) const {
        return lhs.cost < cost;
    }
};

struct graph {
    int to;
    vector<int> cost;
};

constexpr int MAX_N = 100005;
vector<graph> G[MAX_N];
int dist[MAX_N];

void clear_dist(int n) {
    for (int i = 1; i <= n; ++i) {
        dist[i] = INT_MAX;
    }
}

int dijkstra(int s, int e, int n, int which) {
    clear_dist(n);
    priority_queue<edge> q;
    dist[s] = 0;

    q.push({s, 0});

    while (!q.empty()) {
        edge top = q.top();
        q.pop();

        if (top.to == e) {
            return top.cost;
        }

        for (const graph& edge1: G[top.to]) {
            if (dist[edge1.to] > dist[top.to] + edge1.cost[which]) {
                dist[edge1.to] = dist[top.to] + edge1.cost[which];
                q.push({edge1.to, dist[edge1.to]});
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);

    int n, m, a, b, d, n1, n2, c1, p1, c2, p2;
    cin >> n >> m >> a >> b >> d;

    while (m--) {
        cin >> n1 >> n2 >> c1 >> p1 >> c2 >> p2;

        G[n1].push_back({n2, vector<int>{c1, c1 + (d - 1) * p1}});
        G[n2].push_back({n1, vector<int>{c2, c2 + (d - 1) * p2}});
    }

    cout << std::min(dijkstra(a, b, n, 0) + dijkstra(b, a, n, 0),
                     dijkstra(a, b, n, 1) + dijkstra(b, a, n, 1)) << endl;
}