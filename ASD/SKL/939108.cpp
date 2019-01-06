#include <iostream>
#include <vector>
#include <set>

using namespace std;

constexpr int MAX_N = 1000001;
int parents[MAX_N];
int ranks[MAX_N];

int find(int curr) {
    if (parents[curr] != curr) {
        parents[curr] = find(parents[curr]);
    }

    return parents[curr];
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

    parents[second_root] = first_root;
    if (ranks[first_root] == ranks[second_root]) {
        ++ranks[first_root];
    }
}

int main() {
    ios_base::sync_with_stdio(false);

    int n;
    cin >> n;

    vector<int> numbers;

    for (int i = 0; i < n; ++i) {
        int aux;
        cin >> aux;

        numbers.push_back(aux);
        parents[i] = i;
        ranks[i] = 0;
    }

    for (int i = 0; i < 31; ++i) {
        int temp = 1 << i;
        bool found = false;
        int attacher = 0;

        for (int j = 0; j < numbers.size(); ++j) {
            int numb = numbers[j];

            if (numb & temp && !found) {
                found = true;
                attacher = j;
            } else if (numb & temp && found) {
                union_sets(attacher, j);
            }
        }
    }

    set<int> unique;

    for (int j = 0; j < numbers.size(); ++j) {
        unique.insert(find(j));
    }

    cout << unique.size() << endl;
}