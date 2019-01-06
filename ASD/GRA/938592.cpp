#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

struct edge {
    int to, color, cost;

    bool operator<(const edge& lhs) const {
        return lhs.cost < cost;
    }
};

constexpr int MAX_N = 500005;
vector<edge> G[MAX_N];
int _distance[MAX_N][2];

int main() {
    ios_base::sync_with_stdio(false);

    int n, m;
    cin >> n >> m;

    while (m--) {
        int a, b, c;
        cin >> a >> b >> c;

        G[a].push_back({b, c});
        G[b].push_back({a, c});
    }

    queue<edge> q;
    q.push({1, -1, 0});

    for (int i = 1; i <= n; ++i) {
        _distance[i][0] = INT_MAX;
        _distance[i][1] = INT_MAX;
    }

    _distance[1][0] = 0;
    _distance[1][1] = 0;

    while (!q.empty()) {
        edge front = q.front();
        q.pop();

        for (const edge& e: G[front.to]) {
            if (front.color == -1 || front.color + e.color == 1) {
                if (_distance[e.to][e.color] > _distance[front.to][front.color] + 1) {
                    _distance[e.to][e.color] = _distance[front.to][front.color] + 1;
                    q.push({e.to, e.color, _distance[e.to][e.color]});
                }
            }
        }
    }

    for (int i = 2; i <= n; ++i) {
        int min_temp = min(_distance[i][0], _distance[i][1]);
        cout << (min_temp == INT_MAX ? -1 : min_temp) << endl;
    }
}