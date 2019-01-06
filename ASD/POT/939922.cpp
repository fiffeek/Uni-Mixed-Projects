#include <iostream>
#include <queue>
#include <climits>

using namespace std;

struct position {
    int x, y, t;

    bool operator<(const position& lhs) const {
        return lhs.t < t;
    }
};

bool in_boundaries(int x, int y, int X, int Y) {
    return x >= 0 && x < X && y >= 0 && y < Y;
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int graph[500][500];
    int dist[500][500];

    int X, Y, H;
    cin >> X >> Y >> H;

    for (int i = 0; i < X; ++i) {
        for (int j = 0; j < Y; ++j) {
            cin >> graph[i][j];
            dist[i][j] = INT_MAX;
        }
    }

    priority_queue<position> q;
    q.push({0, 0, 0});
    dist[0][0] = 0;

    while (!q.empty()) {
        position top = q.top();
        q.pop();

        for (int i = -2; i <= 2; ++i) {
            for (int j = -2; j <= 2; ++j) {
                if (i == 0 && j == 0) {
                    continue;
                }

                int new_pos_x = top.x + i;
                int new_pos_y = top.y + j;

                if (!in_boundaries(new_pos_x, new_pos_y, X, Y)) {
                    continue;
                }

                int cost = 1;
                if (abs(graph[top.x][top.y] - graph[new_pos_x][new_pos_y]) > H) {
                    cost += abs(graph[top.x][top.y] - graph[new_pos_x][new_pos_y]) - H;
                }

                if (dist[new_pos_x][new_pos_y] > dist[top.x][top.y] + cost) {
                    dist[new_pos_x][new_pos_y] = dist[top.x][top.y] + cost;

                    q.push({new_pos_x, new_pos_y, dist[new_pos_x][new_pos_y]});
                }
            }
        }
    }

    cout << dist[X - 1][Y - 1] << endl;
}