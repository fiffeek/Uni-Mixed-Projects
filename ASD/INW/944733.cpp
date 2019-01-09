#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

constexpr int MAX_N = 1000001;
int parent[MAX_N];
int setrank[MAX_N];
vector<int> numbers;
stack<int> stacked;
map<int, vector<int>> values;
map<int, vector<int>> sorted_values;

int find(int curr) {
    if (curr != parent[curr]) {
        parent[curr] = find(parent[curr]);
    }

    return parent[curr];
}

void union_sets(int first, int second) {
    int first_parent = find(first);
    int second_parent = find(second);

    if (setrank[first_parent] < setrank[second_parent]) {
        int temp = second_parent;
        second_parent = first_parent;
        first_parent = temp;
    }

    parent[second_parent] = first_parent;
    if (setrank[first_parent] == setrank[second_parent]) {
        ++setrank[first_parent];
    }
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    numbers.reserve(1000001);

    for (int i = 0; i < n; ++i) {
        int aux;
        cin >> aux;

        parent[aux] = aux;
        setrank[aux] = 0;
        numbers.push_back(aux);
    }

    for (const int& aux: numbers) {
        if (stacked.empty()) {
            stacked.push(aux);
        } else {
            int max_to_push = aux;

            while (!stacked.empty() && stacked.top() > aux) {
                max_to_push = max(max_to_push, stacked.top());

                union_sets(aux, stacked.top());
                stacked.pop();
            }

            stacked.push(max_to_push);
        }
    }

    for (const int& aux: numbers) {
        values[find(aux)].push_back(aux);
    }

    for (auto& p: values) {
        sort(p.second.begin(), p.second.end());
    }

    for (const auto& p: values) {
        sorted_values.insert({p.second[0], p.second});
    }

    cout << sorted_values.size() << endl;
    for (const auto& p: sorted_values) {
        cout << p.second.size();

        for (const int& aux: p.second) {
            cout << " " << aux;
        }

        cout << endl;
    }

}
