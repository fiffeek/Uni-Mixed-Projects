#include <iostream>
#include <queue>
#include <set>

using namespace std;

constexpr int MAX_N = 500005;
bool car[MAX_N];
vector<int> G[MAX_N];
bool visited[MAX_N];
set<int> s;

void dfs(int top) {
    for (const auto& child: G[top]) {
        if (!visited[child]) {
            if (car[child]) {
                s.emplace(child);
            } else {
                visited[child] = true;
                dfs(child);
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int n, m;
    cin >> n >> m;

    fill_n(car, MAX_N, false);
    fill_n(visited, MAX_N, false);

    for (int i = 0; i < n; ++i) {
        int car_i;
        cin >> car_i;

        car[i + 1] = (bool) car_i;
    }

    while (m--) {
        int a, b;
        cin >> a >> b;

        G[a].push_back(b);
        G[b].push_back(a);
    }

    dfs(1);

    for (const auto& item: s) {
        cout << item << endl;
    }
}