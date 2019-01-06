#include <iostream>
#include <vector>
#include <cmath>
#include <map>

using namespace std;

class Tree {
private:
    unsigned long s = 1;
    vector<long long> tree;

    long long gcd(long long a, long long b) {
        return b == 0 ? a : gcd(b, a % b);
    }

public:
    Tree(unsigned long n) {
        while (s < n) {
            s *= 2;
        }

        tree.resize(2 * s, 0);
    }

    void update(int index, int value) {
        index += s;
        tree[index] = value;
        index /= 2;

        while (index) {
             if (tree[2 * index] && tree[2 * index + 1]) {
                tree[index] = gcd(tree[2 * index], tree[2 * index + 1]);
            } else if (tree[2 * index]) {
                tree[index] = tree[2 * index];
            } else {
                tree[index] = tree[2 * index + 1];
            }

            index /= 2;
        }
    }

    long long gcd_tree() {
        return tree[1] == 0 ? 1 : tree[1];
    }
};

int main() {
    ios_base::sync_with_stdio(false);

    int n;
    cin >> n;
    Tree t{(unsigned long) n};

    map<int, int> pos;
    int last_pox = 0;

    while (n--) {
        char action; int value;
        cin >> action >> value;

        if (action == '+') {
            t.update(last_pox, value);
            pos.insert({value, last_pox++});
        } else {
            t.update(pos.at(value), 0);
            pos.erase(value);
        }

        cout << t.gcd_tree() << endl;
    }
}