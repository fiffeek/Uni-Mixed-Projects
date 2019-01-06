#include <iostream>
#include <unordered_set>
#include <vector>
#include <unordered_map>
#include <set>

using namespace std;

unordered_set<int> car;
constexpr int MAX_N = 5000005;
int parent[MAX_N];
int ranks[MAX_N];
vector<int> G[MAX_N];

int find(int curr) {
    if (curr != parent[curr]) {
        parent[curr] = find(parent[curr]);
    }

    return parent[curr];
}

void union_sets(int first, int second) {
    int first_root = find(first);
    int second_root = find(second);

    if (first_root == second_root) {
        return;
    }

    if (ranks[first_root] < ranks[second_root]) {
        int temp = second_root;
        second_root = first_root;
        first_root = temp;
    }

    parent[second_root] = first_root;
    if (ranks[first_root] == ranks[second_root]) {
        ++ranks[first_root];
    }
}

bool busy(int curr) {
    return car.find(curr) != car.end();
}

bool query(int from, int to) {
    if (busy(to)) {
        return false;
    }

    for (const int child: G[from]) {
        if (find(child) == find(to)) {
            union_sets(child, from);
            car.erase(from);

            for (const int child: G[from]) {
                if (busy(child)) {
                    continue;
                }

                union_sets(child, from);
            }

            return true;
        }
    }

    return false;
}

struct ans {
    int from, to;
};

int main() {
    ios_base::sync_with_stdio(false);

    int n, m;
    cin >> n >> m;

    for (int i = 1; i <= n; ++i) {
        int aux;
        cin >> aux;

        if (aux) {
            car.insert(i);
        }

        parent[i] = i;
        ranks[i] = 0;
    }

    while (m--) {
        int a, b;
        cin >> a >> b;

        G[a].push_back(b);
        G[b].push_back(a);

        if (!busy(a) && !busy(b)) {
            union_sets(a, b);
        }
    }

    int q;
    cin >> q;

    vector<ans> tab;
    tab.reserve((unsigned long long) q + 1);
    unordered_map<int, int> seen;
    unordered_set<int> not_valid;
    set<int> diff;

    for (int i = 0; i < q; ++i) {
        int from, to;
        cin >> from >> to;

        tab.push_back({from, to});
        diff.insert(from);

        if (seen.find(from) != seen.end()) {
            not_valid.insert(seen.find(from)->second);
            seen.erase(from);
            seen.insert({from, i});
        } else {
            seen.insert({from, i});
        }
    }


    int index = 0;
    for (const ans& a: tab) {
        if (not_valid.find(index++) == not_valid.end() && query(a.from, a.to)) {
            cout << "TAK" << endl;
        } else {
            cout << "NIE" << endl;
        }
    }
}