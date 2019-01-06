#include <iostream>
#include <vector>

using namespace std;

struct edge {
    int to, cost;
};

constexpr int MAX_N = 300005;
vector<edge> G[MAX_N];
bool visited[MAX_N];
int max_with[MAX_N];
int max_without[MAX_N];

void dfs(int curr, int parent) {
    visited[curr] = true;

    for (const edge& e: G[curr]) {
        if (!visited[e.to]) {
            dfs(e.to, curr);
        }

        if (e.to != parent) {
            max_without[curr] += max(max_with[e.to], max_without[e.to]);
        }
    }

    for (const edge& e: G[curr]) {
        if (e.to != parent) {
            if (max_with[e.to] <= max_without[e.to]) {
                max_with[curr] = max(max_with[curr], max_without[curr] + e.cost);
            } else {
                max_with[curr] = max(max_with[curr], max_without[curr] - max_with[e.to] + max_without[e.to] + e.cost);
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);

    int n;
    cin >> n;

    for (int i = 0; i < n - 1; ++i) {
        int a, b, c;
        cin >> a >> b >> c;

        G[a].push_back({b, c});
        G[b].push_back({a, c});

        max_with[i + 1] = 0;
        max_without[i + 1] = 0;
        visited[i + 1] = false;
    }

    dfs(1, 1);

    cout << max(max_with[1], max_without[1]) << endl;
}