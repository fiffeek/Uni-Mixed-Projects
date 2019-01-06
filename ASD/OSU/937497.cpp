#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

struct tile {
   int x, y, cost;

   bool operator<(const tile& rhs) const {
       return cost > rhs.cost;
   }
};

struct move_ {
    int x, y;
};

bool in_bounds(int x, int y, int n, int m) {
    return x >= 0 && x < n && y >= 0 && y < m;
}

int main() {
    ios_base::sync_with_stdio(false);

    int n, m;
    cin >> n >> m;

    char map[1005][1005];
    int distance[1005][1005];
    priority_queue<tile> q;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            char temp;
            cin >> temp;

            map[i][j] = temp;
            distance[i][j] = INT_MAX;

            if (temp == 'A') {
                q.push({i, j, 0});
                distance[i][j] = 0;
            }
        }
    }

    move_ moves[4] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    while (!q.empty()) {
        tile front = q.top();
        q.pop();

        if (map[front.x][front.y] == 'B') {
            cout << distance[front.x][front.y] << endl;
            return 0;
        }

        for (const move_& move_aux: moves) {


            int new_x = front.x + move_aux.x;
            int new_y = front.y + move_aux.y;
            int cost;

            if (!in_bounds(new_x, new_y, n, m)) {
                continue;
            }

            switch (map[new_x][new_y]) {
                case '#':
                    cost = 1;
                    break;
                default:
                    cost = 0;
            }

            if (distance[new_x][new_y] > cost + distance[front.x][front.y]) {
                distance[new_x][new_y] = cost + distance[front.x][front.y];
                q.push({new_x, new_y, distance[new_x][new_y]});
            }
        }
    }
}