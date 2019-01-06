#include <iostream>
#include <vector>
#include <climits>

using namespace std;

class Tree {
private:
    struct node {
        int first_1, last_3;
        bool has_two;

        node() {
            first_1 = INT_MAX;
            has_two = false;
            last_3 = -1;
        }
    };
    vector<node> tree;
    unsigned long size = 1;

public:
    Tree(int n) {
        while (size < n) {
            size *= 2;
        }

        tree.resize(2 * size);
    }

    void update(int node, int value) {
        int starter = node + (int) size;

        tree[starter].first_1 = value == 1 ? node : INT_MAX;
        tree[starter].has_two = value == 2;
        tree[starter].last_3 = value == 3 ? node : -1;

        starter /= 2;

        while (starter > 0) {
            tree[starter].first_1 = min(tree[starter * 2].first_1, tree[starter * 2 + 1].first_1);
            tree[starter].has_two = (tree[starter * 2].has_two || tree[starter * 2 + 1].has_two);
            tree[starter].last_3 = max(tree[starter * 2].last_3, tree[starter * 2 + 1].last_3);

            starter /= 2;
        }
    }

    bool checkTwo(int s_index, int e_index) {
        if (s_index > e_index) return false;

        s_index += size, e_index += size;
        bool res = false;

        if (tree[s_index].has_two) return true;
        if (e_index != s_index && tree[e_index].has_two) return true;

        while (s_index / 2 < e_index / 2) {
            if (!(s_index & 1) && tree[s_index + 1].has_two) return true;
            if (e_index & 1 && tree[e_index - 1].has_two) return true;

            s_index /= 2;
            e_index /= 2;
        }

        return false;
    }

    void checkTree() {
        if (tree[1].first_1 < tree[1].last_3 && checkTwo(tree[1].first_1, tree[1].last_3)) {
            cout << "TAK" << endl;
        } else {
            cout << "NIE" << endl;
        }
    }
};


int main() {
    int n;
    cin >> n;

    Tree tree(n);

    for (int i = 0; i < n; ++i) {
        int aux;
        cin >> aux;

        tree.update(i, aux);
    }

    tree.checkTree();
    int m;
    cin >> m;

    while (m--) {
        int index, value;
        cin >> index >> value;

        tree.update(index - 1, value);
        tree.checkTree();
    }
}