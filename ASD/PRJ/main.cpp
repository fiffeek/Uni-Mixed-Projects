#include <iostream>
#include <vector>
#include <queue>
#include <array>

using namespace std;

#define N_MAX 100001

class projectQueue {
private:
    struct project {
        int id, people;

        bool operator<(const project &rhs) const {
            return people > rhs.people;
        }
    };

    priority_queue<project> q;

public:
    void addToQueue(int id, int people) {
        q.push({id, people});
    }

    bool empty() {
        return q.empty();
    }

    int frontAndPop() {
        project front = q.top();
        q.pop();
        return front.id;
    }
};

int main() {
    int n, m, k;
    cin >> n >> m >> k;

    vector<int> G[N_MAX];
    int people[N_MAX], in[N_MAX];

    int ans = 0;

    for (int i = 0; i < n; ++i) {
        int aux;
        cin >> aux;

        people[i] = aux;
    }

    for (int i = 0; i < m; ++i) {
        int from, to;
        cin >> to >> from;

        G[from - 1].push_back(to - 1);
        in[to - 1]++;
    }

    projectQueue q{};

    for (int i = 0; i < n; ++i) {
        if (in[i] == 0) {
            q.addToQueue(i, people[i]);
        }
    }

    while (!q.empty() && k > 0) {
        int projectID = q.frontAndPop();
        ans = max(ans, people[projectID]);
        k--;

        for (const auto &child: G[projectID]) {
            in[child]--;
            if (in[child] == 0) {
                q.addToQueue(child, people[child]);
            }
        }
    }

    cout << ans << endl;
}