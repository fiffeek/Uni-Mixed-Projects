#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

constexpr int MAX_N = 500005;
vector<int> G[MAX_N];
int depth[MAX_N];

bool dfs(int curr, int d) {
    depth[curr] = d;

    for (const auto& child: G[curr]) {
        if (depth[child] == -1) {
            if (!dfs(child, d + 1)) {
                return false;
            }
        } else {
            if ((depth[child] - d) % 2 == 0) {
                return false;
            }
        }
    }

    return true;
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int n, m;
    cin >> n >> m;

    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;

        G[a].emplace_back(b);
        G[b].emplace_back(a);
    }

    for (int i = 1; i <= n; ++i) {
        depth[i] = -1;
    }

    for (int i = 1; i <= n; ++i) {
        if (depth[i] == -1) {
            if (!dfs(i, 0)) {
                cout << "NIE" << endl;
                return 0;
            }
        }
    }

    cout << "TAK" << endl;
}