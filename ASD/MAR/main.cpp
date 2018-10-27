#include <iostream>
#include <queue>
#include <climits>
#include <cmath>

using namespace std;

#define MAX_STARS_USED 19

struct node {
    int node, s_pow;
};

class two_queue {
private:
    queue<node> normal_vertex;
    queue<node> u_vertex;
    void add_normal_node(node v) {
        normal_vertex.push(v);
    }

    void add_u_node(node v) {
        u_vertex.push(v);
    }

public:
    void add_node(node v, const string &road) {
        if (road[v.node] == 'U') {
            add_u_node(v);
        } else {
            add_normal_node(v);
        }
    }

    node front_and_pop() {
        node aux;

        if (normal_vertex.empty()) {
            aux = u_vertex.front();
            u_vertex.pop();
            return aux;
        }

        aux = normal_vertex.front();
        normal_vertex.pop();
        return aux;
    }

    bool is_empty() {
        return normal_vertex.empty() && u_vertex.empty();
    }
};

int main() {
    int n, k, u;
    cin >> n >> k >> u;

    string road;
    cin >> road;

    int distance[MAX_STARS_USED][n];
    for (int i = 0; i < MAX_STARS_USED; ++i) {
        fill_n(distance[i], n, INT_MAX);
    }

    two_queue tq = two_queue();
    tq.add_node({0, 0}, road);
    distance[0][0] = 0;

    while (!tq.is_empty()) {
        node top_node = tq.front_and_pop();
        int add_u = 0;
        int is_asterisk = 0;

        switch (road[top_node.node]) {
            case '>':
                top_node.node += k;
                if (distance[top_node.s_pow][top_node.node - k ] >= distance[top_node.s_pow][top_node.node]) continue;
                distance[top_node.s_pow][top_node.node] = distance[top_node.s_pow][top_node.node - k];
                break;
            case '<':
                top_node.node -= k;
                if (distance[top_node.s_pow][top_node.node + k ] >= distance[top_node.s_pow][top_node.node]) continue;
                distance[top_node.s_pow][top_node.node] = distance[top_node.s_pow][top_node.node + k];
                break;
            case '#':
                distance[top_node.s_pow][top_node.node] = INT_MAX;
                continue;
            case '*':
                top_node.s_pow++;
                is_asterisk = -1;
                break;
            case 'U':
                add_u = u;
                break;
            case '.':
                break;
            default:
                cerr << "problem" << endl;
        }

        vector<int> vi{1, (1 << (top_node.s_pow + 1))};

        for (const auto &num: vi) {
            if (top_node.s_pow >= MAX_STARS_USED) {
                top_node.s_pow = MAX_STARS_USED - 1;
                is_asterisk = 0;
            }

            int aux = distance[top_node.s_pow + is_asterisk][top_node.node] + 1 + add_u;
            if (top_node.node + num < road.length() && aux < distance[top_node.s_pow][top_node.node + num]) {
                distance[top_node.s_pow][top_node.node + num] = aux;
                tq.add_node({top_node.node + num, top_node.s_pow}, road);
            }
        }
    }

    int ans = INT_MAX;
    for (int i = 0; i < MAX_STARS_USED; ++i) {
        ans = min(ans, distance[i][n - 1]);
    }

    if (ans == INT_MAX) ans = -1;
    if (ans != -1 && road[n - 1] == 'U') ans += u;

    cout << ans << endl;
}