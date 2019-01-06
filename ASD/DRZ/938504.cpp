#include <iostream>
#include <vector>

using namespace std;

constexpr int MAX_N = 500005;
vector<int> G[MAX_N];
unsigned long long sum[MAX_N];
unsigned long long up[MAX_N];
unsigned long long odp[MAX_N];
unsigned long long how_many[MAX_N];
int n;

unsigned long long dfs(int curr, int parent) {
    unsigned long long sum_ = 0;
    unsigned long long vertexes = 0;

    for (const int temp: G[curr]) {
        if (temp != parent) {
            vertexes += dfs(temp, curr);
            sum_ += sum[temp];
        }
    }

    sum[curr] = sum_ + vertexes;
    how_many[curr] = vertexes + 1;
    return vertexes + 1;
}

void updfs(unsigned long long curr, unsigned long long parent, unsigned long long value) {
    up[curr] = value;
    odp[curr] = up[curr] + sum[curr];

    for (const int temp: G[curr]) {
        if (temp != parent) {
            updfs(temp, curr, odp[curr] - how_many[temp] - sum[temp] + n - how_many[temp]);
        }
    }

}

int main() {
    ios_base::sync_with_stdio(false);
    cin >> n;

    for (int i = 0; i < n - 1; ++i) {
        int a, b;
        cin >> a >> b;

        G[a].push_back(b);
        G[b].push_back(a);
        sum[i + 1] = 0;
    }

    dfs(1, 1);
    updfs(1, 1, 0);
    up[1] = 0;

    for (int i = 1; i <= n; ++i) {
        cout << odp[i] << endl;
    }

}