#include <iostream>
#include <vector>

using namespace std;

struct Tree {
private:
    unsigned long size = 1;
    vector<int> tree;
    unsigned long limit;

public:
    Tree(int n) {
        while (size < n) {
            size *= 2;
        }
        tree.resize(2 * size, 0);
        limit = size + n - 1;
    }

    void update(int index, int value) {
        index += size;
        tree[index] += value;
        index /= 2;

        while (index) {
            tree[index] = tree[2 * index] + tree[2 * index + 1];
            index /= 2;
        }
    }

    void update(int start_vertex, bool update_all,
                int lookup_start, int lookup_end,
                int interval_start, int interval_end, int value) {
        if (update_all) {
            tree[start_vertex] = 0;
        }

        if (start_vertex > limit ||  interval_end < lookup_start || interval_start > lookup_end) {
            return;
        }

        if (lookup_start <= interval_start && interval_end <= lookup_end) {
            tree[start_vertex] = value;

            return;
        }

        int middle = (interval_start + interval_end) / 2;
        bool updater = tree[start_vertex] == 0;

        update(start_vertex * 2, updater, lookup_start, lookup_end, interval_start, middle, value);
        update((start_vertex * 2) + 1, updater, lookup_start, lookup_end, middle + 1, interval_end, value);
        tree[start_vertex] += value;
    }

    int lookup(int start_vertex, bool update_all,
               int lookup_start, int lookup_end,
               int interval_start, int interval_end) {
        if (update_all) {
            tree[start_vertex] = 0;
        }

        if (start_vertex > limit ||  interval_end < lookup_start || interval_start > lookup_end) {
            return 0;
        }

        if (lookup_start <= interval_start && interval_end <= lookup_end) {
            int to_ret = tree[start_vertex];
            tree[start_vertex] = 0;

            return to_ret;
        }

        int middle = (interval_start + interval_end) / 2;
        bool updater = tree[start_vertex] == 0;

        int subtrees_lookup =
                lookup(start_vertex * 2, updater, lookup_start, lookup_end, interval_start, middle)
                + lookup((start_vertex * 2) + 1, updater, lookup_start, lookup_end, middle + 1, interval_end);

        tree[start_vertex] -= subtrees_lookup;

        return subtrees_lookup;
    }

    int returnTreeSize() {
        return size;
    }
};

int main() {
    ios_base::sync_with_stdio(false);

    int n, m;
    cin >> n >> m;

    Tree tree = Tree(n);

    while (n--) {
        int aux;
        cin >> aux;

        tree.update(aux - 1, 1);
    }

    while (m--) {
        int a, b, c;
        cin >> a >> b >> c;

        int temp_on_point =  tree.lookup(1, false, c, c, 1, tree.returnTreeSize());
        int temp = tree.lookup(1, false, a, b, 1, tree.returnTreeSize());
        cout << temp << endl;

        tree.update(1, false, c, c, 1, tree.returnTreeSize(), temp + temp_on_point);
    }
}