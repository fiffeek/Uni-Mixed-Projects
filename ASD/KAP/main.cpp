#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <climits>
#include <queue>

using namespace std;

int const N = 200005;

struct island {
    int x, y, id;
};

struct edge {
    int id, value;

    bool operator<(const edge &another) const {
        return another.value < value;
    }
};

int minimal_distance(const island &a, const island &b) {
    return min(abs(a.x - b.x), abs(a.y - b.y));
}

void add_edges(vector<island> &islands,
               const function<bool (const island&, const island&)> &f,
               vector<edge> (&graph)[N]) {
    sort(islands.begin(), islands.end(), f);

    for (int i = 1; i < islands.size(); ++i) {
        graph[islands[i].id].push_back(edge{islands[i - 1].id, minimal_distance(islands[i], islands[i - 1])});
        graph[islands[i - 1].id].push_back(edge{islands[i].id, minimal_distance(islands[i], islands[i - 1])});
    }
}

int dijkstra(vector<edge> (&graph)[N], int last) {
    array<int, N> distance{};
    distance.fill(INT_MAX);
    priority_queue<edge> queue;

    distance[1] = 0;
    queue.push(edge{1, 0});

    while (!queue.empty()) {
        edge top = queue.top();
        queue.pop();

        if (top.id == last) {
            break;
        }

        for (size_t i = 0; i < graph[top.id].size(); ++i) {
            edge road = graph[top.id][i];
            int calculated_distance = distance[top.id] + road.value;

            if (calculated_distance < distance[road.id]) {
                distance[road.id] = calculated_distance;
                queue.push(edge{road.id, distance[road.id]});
            }
        }
    }

    return distance[last];
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int n;
    cin >> n;

    vector<edge> graph[N];
    vector<island> islands;

    for (int i = 0; i < n; ++i) {
        int x, y;
        cin >> x >> y;

        islands.emplace_back(island{x, y, i + 1});
    }

    add_edges(islands, [](const island &a, const island &b) { return a.x < b.x; }, graph);
    add_edges(islands, [](const island &a, const island &b) { return a.y < b.y; }, graph);

    cout << dijkstra(graph, n) << endl;
}