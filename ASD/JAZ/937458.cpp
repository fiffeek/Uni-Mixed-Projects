#include <iostream>
#include <vector>

using namespace std;

constexpr int MAX_N = 200005;
vector<int> G[MAX_N];
bool visited[MAX_N];

bool dfs(int curr, int parent) {
    if (visited[curr]) {
        return true;
    }

    visited[curr] = true;

    for (const int child: G[curr]) {
        if (child != parent && dfs(child, curr)) {
            return true;
        }
    }

    return false;
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int n, m;
    cin >> n >> m;

    while (m--) {
        int a, b;
        cin >> a >> b;

        G[a].push_back(b);
        G[b].push_back(a);
    }

    for (int i = 1; i <= n; ++i) {
        if (!visited[i]) {
            if (dfs(i, i)) {
                cout << "TAK" << endl;
                return 0;
            }
        }
    }

    cout << "NIE" << endl;
    return 0;
}